/*
 *  AudioFilePlayer.cpp
 *
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

AudioFilePlayer::AudioFilePlayer()
    : bufferingTimeSliceThread ("Shared Buffering Thread"),
      formatManager (new AudioFormatManager(), true)
{
    bufferingTimeSliceThread.startThread(3);
    
    audioTransportSource = new AudioTransportSource();
    audioTransportSource->setSource (soundTouchAudioSource);
    loopingAudioSource = new LoopingAudioSource (audioTransportSource, false);
    filteringAudioSource = new FilteringAudioSource (audioTransportSource, false);
    
    masterSource = filteringAudioSource;
    
	formatManager->registerBasicFormats();
}

AudioFilePlayer::~AudioFilePlayer()
{
	audioTransportSource->setSource (nullptr);
}

void AudioFilePlayer::start()
{
    audioTransportSource->start();

    listeners.call (&Listener::playerStoppedOrStarted, this);
}

void AudioFilePlayer::stop()
{
    audioTransportSource->stop();
    
    listeners.call (&Listener::playerStoppedOrStarted, this);
}

void AudioFilePlayer::startFromZero()
{
	if (audioFormatReaderSource == nullptr)
        return;
	
	audioTransportSource->setPosition (0.0);
	audioTransportSource->start();
    
    listeners.call (&Listener::playerStoppedOrStarted, this);
}

void AudioFilePlayer::pause()
{
	if (audioTransportSource->isPlaying())
		audioTransportSource->stop();
	else
		audioTransportSource->start();
    
    listeners.call (&Listener::playerStoppedOrStarted, this);
}

void AudioFilePlayer::setPlaybackSettings (SoundTouchProcessor::PlaybackSettings newSettings)
{
    soundTouchAudioSource->setPlaybackSettings (newSettings);
    
    listeners.call (&Listener::playbackSettingsChanged, this);
}

bool AudioFilePlayer::setFile (const String& path)
{
    filePath = path;
    
    return setSourceWithReader (formatManager->createReaderFor (File (path)));
}

bool AudioFilePlayer::setInputStream (InputStream* inputStream)
{
    filePath = String::empty;
    
	AudioFormatReader* reader = formatManager->createReaderFor (inputStream);
    
    return setSourceWithReader (reader);
}

#if JUCE_IOS
bool AudioFilePlayer::setAVAssetURL (String avAssetNSURLAsString)
{
    filePath = avAssetNSURLAsString;
	AudioFormatReader* reader = AVAssetAudioFormat().createReaderFor (avAssetNSURLAsString);
    
    return setSourceWithReader (reader);
}
#endif

void AudioFilePlayer::setAudioFormatManager(AudioFormatManager* newManager,  bool deleteWhenNotNeeded)
{
    OptionalScopedPointer<AudioFormatManager> newFormatManager (newManager, deleteWhenNotNeeded);
	formatManager = newFormatManager;
}

//==============================================================================
void AudioFilePlayer::setLoopTimes (double startTime, double endTime)
{
    loopingAudioSource->setLoopTimes (startTime, endTime);
}

void AudioFilePlayer::setLoopBetweenTimes (bool shouldLoop)
{
    loopingAudioSource->setLoopBetweenTimes (shouldLoop);
	listeners.call (&Listener::loopBetweenTimesChanged, this);
}

void AudioFilePlayer::setPositionIgnoringLoop (double newPosition)
{
    if (audioFormatReaderSource != nullptr)
    {
        const double sampleRate = audioFormatReaderSource->getAudioFormatReader()->sampleRate;
        
        if (sampleRate > 0.0)
        {
            loopingAudioSource->setNextReadPositionIgnoringLoop ((int64) (newPosition * sampleRate));
        }
    }
}

//==============================================================================
void AudioFilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    if (masterSource != nullptr)
        masterSource->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void AudioFilePlayer::releaseResources()
{
    if (masterSource != nullptr)
        masterSource->releaseResources();
}

void AudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (masterSource != nullptr)
        masterSource->getNextAudioBlock (bufferToFill);
}

//==============================================================================
void AudioFilePlayer::addListener (AudioFilePlayer::Listener* const listener)
{
    listeners.add (listener);
}

void AudioFilePlayer::removeListener (AudioFilePlayer::Listener* const listener)
{
    listeners.remove (listener);
}

//==============================================================================
bool AudioFilePlayer::setSourceWithReader (AudioFormatReader* reader)
{
//	stop();
    masterSource = nullptr;
	audioTransportSource->setSource (nullptr);
    soundTouchAudioSource = nullptr;
    
	if (reader != nullptr)
	{										
		// we SHOULD let the AudioFormatReaderSource delete the reader for us..
		audioFormatReaderSource = new AudioFormatReaderSource (reader, true);
        soundTouchAudioSource = new SoundTouchAudioSource (audioFormatReaderSource,
                                                           bufferingTimeSliceThread,
                                                           false,
                                                           32768);
        loopingAudioSource = new LoopingAudioSource (soundTouchAudioSource, false);
        audioTransportSource->setSource (loopingAudioSource);
        masterSource = filteringAudioSource;
        
		// let our listeners know that we have loaded a new file
		audioTransportSource->sendChangeMessage();
        listeners.call (&Listener::fileChanged, this);

		return true;
	}
	
    setLibraryEntry (ValueTree::invalid);

    audioTransportSource->sendChangeMessage();
    listeners.call (&Listener::fileChanged, this);

    return false;    
}

END_JUCE_NAMESPACE