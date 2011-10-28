/*
 *  dRowAuidio_SoundTouchAudioSource.cpp
 *
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

SoundTouchAudioSource::SoundTouchAudioSource(PositionableAudioSource* source_,
                                             TimeSliceThread& backgroundThread_,
                                             bool deleteSourceWhenDeleted,
                                             int numberOfSamplesToBuffer_,
                                             int numberOfChannels_)
    : source (source_, deleteSourceWhenDeleted),
      backgroundThread (backgroundThread_),
      numberOfSamplesToBuffer (jmax (1024, numberOfSamplesToBuffer_)),
      numberOfChannels (numberOfChannels_),
      buffer (numberOfChannels_, 0),
      nextPlayPos (0),
      nextReadPos (0),
      isPrepared (false),
      isForwards (true),
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

    setNextReadPosition (getNextReadPosition());
}

void SoundTouchAudioSource::setPlayDirection (bool shouldPlayForwards)
{	
    isForwards = shouldPlayForwards;
    soundTouchProcessor.clear();

    const ScopedLock sl (bufferStartPosLock);
    nextReadPos = getNextReadPosition();
    numBuffered = 0;
    updateNextEffectivePlayPos();
    
    backgroundThread.moveToFrontOfQueue (this);
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
        backgroundThread.removeTimeSliceClient (this);
        
        isPrepared = true;
        sampleRate = sampleRate_;
        buffer.setSize (numberOfChannels, 2048);
        
        source->prepareToPlay (bufferSizeNeeded, sampleRate_);
        
        backgroundThread.addTimeSliceClient (this);
        
        while (soundTouchProcessor.getNumReady() < numberOfSamplesToBuffer)
        {
            backgroundThread.moveToFrontOfQueue (this);
            Thread::sleep (5);
        }
    }
}

void SoundTouchAudioSource::releaseResources()
{
    backgroundThread.removeTimeSliceClient (this);
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
    backgroundThread.moveToFrontOfQueue (this);
}

int64 SoundTouchAudioSource::getNextReadPosition() const
{
    jassert (source->getTotalLength() > 0);

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
        
        if (! isForwards)
        {
            if (info.buffer->getNumChannels() == 1)
            {
                reverseArray (info.buffer->getSampleData (0), info.numSamples);
            }
            if (info.buffer->getNumChannels() == 2) 
            {
                reverseTwoArrays (info.buffer->getSampleData (0), info.buffer->getSampleData (1), info.numSamples);
            }
            else
            {
                for (int c = 0; c < info.buffer->getNumChannels(); c++)
                    reverseArray (info.buffer->getSampleData (c), info.numSamples);
            }
            
            nextReadPos -= 2 * info.numSamples;
        }
        else
        {
            nextReadPos += info.numSamples;
        }
        
        soundTouchProcessor.writeSamples (buffer.getArrayOfChannels(), buffer.getNumChannels(), info.numSamples);
        numBuffered += info.numSamples;
        updateNextEffectivePlayPos();
        
        return true;
    }
    
    return false;
}

int SoundTouchAudioSource::useTimeSlice()
{
    return readNextBufferChunk() ? 1 : 100;
}

inline void SoundTouchAudioSource::updateNextEffectivePlayPos()
{
    effectiveNextPlayPos = nextReadPos - (isForwards ? numBuffered : 2 * -numBuffered);
    
    if (source->isLooping() && effectiveNextPlayPos > 0)
        effectiveNextPlayPos % source->getTotalLength();
}

END_JUCE_NAMESPACE