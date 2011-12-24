/*
 *  AudioFilePlayerExt.cpp
 *
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

AudioFilePlayerExt::AudioFilePlayerExt()
    : AudioFilePlayer()
{
    loopingAudioSource = new LoopingAudioSource (audioTransportSource, false);
    filteringAudioSource = new FilteringAudioSource (audioTransportSource, false);
    
    masterSource = filteringAudioSource;
}

AudioFilePlayerExt::~AudioFilePlayerExt()
{
    audioTransportSource->setSource (nullptr);
}

void AudioFilePlayerExt::setPlaybackSettings (SoundTouchProcessor::PlaybackSettings newSettings)
{
    soundTouchAudioSource->setPlaybackSettings (newSettings);
    
    listeners.call (&Listener::audioFilePlayerSettingChanged, this, SoundTouchSetting);
}

//bool AudioFilePlayer::setFile (const String& path)
//{
//    currentMemoryBlock = nullptr;
//    filePath = path;
//    
//    return setSourceWithReader (formatManager->createReaderFor (File (path)));
//}
//
//bool AudioFilePlayer::setInputStream (InputStream* inputStream)
//{
//    filePath = String::empty;
//    
//	AudioFormatReader* reader = formatManager->createReaderFor (inputStream);
//    
//    return setSourceWithReader (reader);
//}
//
//bool AudioFilePlayer::setMemoryBlock (MemoryBlock* inputBlock)
//{
//    filePath = String::empty;
//    currentMemoryBlock = inputBlock;
//    
//    return setSourceWithReader (formatManager->createReaderFor (getInputStream()));
//}

//==============================================================================
void AudioFilePlayerExt::setLoopTimes (double startTime, double endTime)
{
    loopingAudioSource->setLoopTimes (startTime, endTime);
}

void AudioFilePlayerExt::setLoopBetweenTimes (bool shouldLoop)
{
    loopingAudioSource->setLoopBetweenTimes (shouldLoop);
	listeners.call (&Listener::audioFilePlayerSettingChanged, this, LoopBeetweenTimesSetting);
}

void AudioFilePlayerExt::setPositionIgnoringLoop (double newPosition)
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
bool AudioFilePlayerExt::setSourceWithReader (AudioFormatReader* reader)
{
//	stop();
    masterSource = nullptr;
	audioTransportSource->setSource (nullptr);
    soundTouchAudioSource = nullptr;
    
	if (reader != nullptr)
	{										
		// we SHOULD let the AudioFormatReaderSource delete the reader for us..
		audioFormatReaderSource = new AudioFormatReaderSource (reader, true);
        soundTouchAudioSource = new SoundTouchAudioSource (audioFormatReaderSource);
//        loopingAudioSource = new LoopingAudioSource (soundTouchAudioSource, false);
        audioTransportSource->setSource (soundTouchAudioSource,
                                         32768,
                                         &bufferingTimeSliceThread);
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