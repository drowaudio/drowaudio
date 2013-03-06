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



SoundTouchAudioSource::SoundTouchAudioSource (PositionableAudioSource* source_,
                                              bool deleteSourceWhenDeleted,
                                              int numberOfSamplesToBuffer_,
                                              int numberOfChannels_)
    : source (source_, deleteSourceWhenDeleted),
      numberOfSamplesToBuffer (jmax (1024, numberOfSamplesToBuffer_)),
      numberOfChannels (numberOfChannels_),
      buffer (numberOfChannels_, 0),
      nextReadPos (0),
      isPrepared (false)
{
    jassert (source_ != nullptr);

    soundTouchProcessor.clear();
}

SoundTouchAudioSource::~SoundTouchAudioSource()
{
    releaseResources();
}

void SoundTouchAudioSource::setPlaybackSettings (SoundTouchProcessor::PlaybackSettings newSettings)
{
    soundTouchProcessor.setPlaybackSettings (newSettings);
}

//==============================================================================
void SoundTouchAudioSource::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate_)
{
    soundTouchProcessor.initialise (numberOfChannels, sampleRate);
    
    if (sampleRate_ != sampleRate
        || numberOfSamplesToBuffer != buffer.getNumSamples()
        || ! isPrepared)
    {
        isPrepared = true;
        sampleRate = sampleRate_;
        buffer.setSize (numberOfChannels, numberOfSamplesToBuffer);
        
        source->prepareToPlay (numberOfSamplesToBuffer, sampleRate_);
    }
}

void SoundTouchAudioSource::releaseResources()
{
    soundTouchProcessor.clear();
    
    isPrepared = false;
    
    source->releaseResources();
}

void SoundTouchAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    while (soundTouchProcessor.getNumReady() < info.numSamples)
        readNextBufferChunk();

    soundTouchProcessor.readSamples (info.buffer->getArrayOfChannels(), info.buffer->getNumChannels(),
                                     info.numSamples, info.startSample);

    effectiveNextPlayPos += (int64) (info.numSamples * soundTouchProcessor.getEffectivePlaybackRatio());
}

//==============================================================================
void SoundTouchAudioSource::setNextReadPosition (int64 newPosition)
{
    const ScopedLock sl (bufferStartPosLock);

    nextReadPos = effectiveNextPlayPos = newPosition;

    soundTouchProcessor.clear();
}

int64 SoundTouchAudioSource::getNextReadPosition() const
{
    return source->isLooping() ? effectiveNextPlayPos % source->getTotalLength()
                               : effectiveNextPlayPos;
}

//==============================================================================
void SoundTouchAudioSource::readNextBufferChunk()
{
    const ScopedLock sl (bufferStartPosLock);

    if (source->getNextReadPosition() != nextReadPos)
        source->setNextReadPosition (nextReadPos);
    
    AudioSourceChannelInfo info;
    info.buffer = &buffer;
    info.startSample = 0;
    info.numSamples = buffer.getNumSamples();
    
    source->getNextAudioBlock (info);
    nextReadPos += info.numSamples;

    soundTouchProcessor.writeSamples (buffer.getArrayOfChannels(), buffer.getNumChannels(), info.numSamples);
}



#endif