/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#if DROWAUDIO_USE_SOUNDTOUCH



AudioFilePlayerExt::AudioFilePlayerExt()
    : AudioFilePlayer()
{
    loopingAudioSource = new LoopingAudioSource (audioTransportSource, false);
    //reversibleAudioSource = new ReversibleAudioSource (audioTransportSource, false);
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

void AudioFilePlayerExt::setPlayDirection (bool shouldPlayForwards)
{
    reversibleAudioSource->setPlayDirection (shouldPlayForwards);

    listeners.call (&Listener::audioFilePlayerSettingChanged, this, PlayDirectionSetting);
}

void AudioFilePlayerExt::setFilterGain (FilteringAudioSource::FilterType type, float newGain)
{
    filteringAudioSource->setGain (type, newGain);

    listeners.call (&Listener::audioFilePlayerSettingChanged, this, FilterGainSetting);
}

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

void AudioFilePlayerExt::setPosition (double newPosition, bool ignoreAnyLoopBounds)
{
    if (ignoreAnyLoopBounds && audioFormatReaderSource != nullptr)
    {
        const double sampleRate = audioFormatReaderSource->getAudioFormatReader()->sampleRate;
        
        if (sampleRate > 0.0)
            loopingAudioSource->setNextReadPositionIgnoringLoop ((int64) (newPosition * sampleRate));
    }
    else
    {
        AudioFilePlayer::setPosition (newPosition);
    }
}

//==============================================================================
bool AudioFilePlayerExt::setSourceWithReader (AudioFormatReader* reader)
{
    SoundTouchProcessor::PlaybackSettings oldSettings;
    if (soundTouchAudioSource != nullptr)
        oldSettings = soundTouchAudioSource->getPlaybackSettings();
    
	audioTransportSource->setSource (nullptr);
    loopingAudioSource = nullptr;
    soundTouchAudioSource = nullptr;
    bufferingAudioSource = nullptr;
    
	if (reader != nullptr)
	{										
		// we SHOULD let the AudioFormatReaderSource delete the reader for us..
		audioFormatReaderSource = new AudioFormatReaderSource (reader, true);
        bufferingAudioSource = new BufferingAudioSource (audioFormatReaderSource,
                                                         bufferingTimeSliceThread,
                                                         false,
                                                         32768);
        soundTouchAudioSource = new SoundTouchAudioSource (bufferingAudioSource);
        loopingAudioSource = new LoopingAudioSource (soundTouchAudioSource, false);
        audioTransportSource->setSource (loopingAudioSource,
                                         0, nullptr,
                                         reader->sampleRate);
        
        listeners.call (&Listener::fileChanged, this);
        setPlaybackSettings (oldSettings);

		return true;
	}
	
    setLibraryEntry (ValueTree::invalid);
    listeners.call (&Listener::fileChanged, this);

    return false;    
}



#endif