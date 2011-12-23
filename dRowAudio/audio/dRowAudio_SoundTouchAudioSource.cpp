/*
 *  SoundTouchAudioSource.cpp
 *
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

SoundTouchAudioSource::SoundTouchAudioSource(PositionableAudioSource* source_,
                                             bool deleteSourceWhenDeleted,
                                             int numberOfSamplesToBuffer_,
                                             int numberOfChannels_)
    : source (source_, deleteSourceWhenDeleted),
      numberOfSamplesToBuffer (jmax (1024, numberOfSamplesToBuffer_)),
      numberOfChannels (numberOfChannels_),
      buffer (numberOfChannels_, 0),
      nextPlayPos (0),
      nextReadPos (0),
      isPrepared (false),
      numBuffered (0)
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

    //setNextReadPosition (getNextReadPosition());
}

//==============================================================================
void SoundTouchAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate_)
{
    soundTouchProcessor.initialise (numberOfChannels, sampleRate);
    
    const int bufferSizeNeeded = 2048;
    
    if (sampleRate_ != sampleRate
        || bufferSizeNeeded != buffer.getNumSamples()
        || ! isPrepared)
    {
        isPrepared = true;
        sampleRate = sampleRate_;
        buffer.setSize (numberOfChannels, 2048);
        
        source->prepareToPlay (bufferSizeNeeded, sampleRate_);
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
    {
        readNextBufferChunk();
    }

    soundTouchProcessor.readSamples (info.buffer->getArrayOfChannels(), info.buffer->getNumChannels(),
                                     info.numSamples, info.startSample);

    numBuffered -= info.numSamples * soundTouchProcessor.getEffectivePlaybackRatio();

    updateNextEffectivePlayPos();
}

//==============================================================================
void SoundTouchAudioSource::setNextReadPosition (int64 newPosition)
{
    const ScopedLock sl (bufferStartPosLock);
    
    nextReadPos = nextPlayPos = newPosition;
    numBuffered = 0;
    updateNextEffectivePlayPos();
    
    soundTouchProcessor.clear();
    buffer.clear();
}

int64 SoundTouchAudioSource::getNextReadPosition() const
{
    return effectiveNextPlayPos;
}

//==============================================================================
bool SoundTouchAudioSource::readNextBufferChunk()
{
    if (soundTouchProcessor.getNumReady() < numberOfSamplesToBuffer)
    {
        const ScopedLock sl (bufferStartPosLock);

        const int maxChunkSize = 2048;
        const int nextSourceReadPos = nextReadPos;
        
        if (source->getNextReadPosition() != nextSourceReadPos)
            source->setNextReadPosition (nextSourceReadPos);
        
        AudioSourceChannelInfo info;
        info.buffer = &buffer;
        info.startSample = 0;
        info.numSamples = jlimit (0, maxChunkSize, buffer.getNumSamples());
        
        source->getNextAudioBlock (info);
        nextReadPos += info.numSamples;
                
        soundTouchProcessor.writeSamples (buffer.getArrayOfChannels(), buffer.getNumChannels(), info.numSamples);
        numBuffered += info.numSamples;
        updateNextEffectivePlayPos();
        
        return true;
    }
    
    return false;
}

inline void SoundTouchAudioSource::updateNextEffectivePlayPos()
{
    effectiveNextPlayPos = nextReadPos - numBuffered;
    
    if (source->isLooping() && effectiveNextPlayPos > 0)
        effectiveNextPlayPos % source->getTotalLength();
}

END_JUCE_NAMESPACE
