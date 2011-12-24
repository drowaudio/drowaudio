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
      formatManager (new AudioFormatManager(), true),
      currentMemoryBlock (nullptr)
{
    bufferingTimeSliceThread.startThread(3);
    
    audioTransportSource = new AudioTransportSource();
    audioTransportSource->addChangeListener (this);
    masterSource = audioTransportSource;
    
	formatManager->registerBasicFormats();
}

AudioFilePlayer::~AudioFilePlayer()
{
	audioTransportSource->setSource (nullptr);
    audioTransportSource->removeChangeListener (this);
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

bool AudioFilePlayer::setFile (const File& newFile)
{
    currentMemoryBlock = nullptr;
    memoryInputStream = nullptr;
    
    file = newFile;
    
    return setSourceWithReader (formatManager->createReaderFor (file));
}

bool AudioFilePlayer::setMemoryBlock (MemoryBlock* inputBlock)
{
    file = File::nonexistent;
    currentMemoryBlock = inputBlock;

    if (currentMemoryBlock != nullptr)
        memoryInputStream = new MemoryInputStream (*currentMemoryBlock, false);
    else 
        memoryInputStream = nullptr;

    return setSourceWithReader (formatManager->createReaderFor (memoryInputStream));
}

MemoryInputStream* AudioFilePlayer::getInputStream()
{   
    if (currentMemoryBlock != nullptr)
        return new MemoryInputStream (*currentMemoryBlock, false);

    return nullptr;
}

bool AudioFilePlayer::sourceIsMemoryBlock()
{
    return currentMemoryBlock != nullptr;
}

void AudioFilePlayer::setAudioFormatManager (AudioFormatManager* newManager,  bool deleteWhenNotNeeded)
{
    OptionalScopedPointer<AudioFormatManager> newFormatManager (newManager, deleteWhenNotNeeded);
	formatManager = newFormatManager;
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

void AudioFilePlayer::setLooping (bool shouldLoop)
{   
    if (audioFormatReaderSource != nullptr)
        audioFormatReaderSource->setLooping (shouldLoop); 
}

void AudioFilePlayer::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == audioTransportSource)
        listeners.call (&Listener::playerStoppedOrStarted, this);
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
    bool shouldBeLooping = isLooping();
	audioTransportSource->setSource (nullptr);
    
	if (reader != nullptr)
	{										
		// we SHOULD let the AudioFormatReaderSource delete the reader for us..
		audioFormatReaderSource = new AudioFormatReaderSource (reader, true);
        audioTransportSource->setSource (audioFormatReaderSource,
                                         32768,
                                         &bufferingTimeSliceThread);
        
        if (shouldBeLooping)
            audioFormatReaderSource->setLooping (true);
        
		// let our listeners know that we have loaded a new file
		audioTransportSource->sendChangeMessage();
        listeners.call (&Listener::fileChanged, this);

		return true;
	}
	
    audioTransportSource->sendChangeMessage();
    listeners.call (&Listener::fileChanged, this);

    return false;    
}

END_JUCE_NAMESPACE