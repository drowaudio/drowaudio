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
    soundTouchProcessor.clear();

    const ScopedLock sl (bufferStartPosLock);
    numBuffered = 0;
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
        
        source->prepareToPlay (samplesPerBlockExpected, sampleRate_);
        
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
//    if (source->getNextReadPosition() != nextPlayPos)
//        source->setNextReadPosition (nextPlayPos);
    
    //    if (soundTouchProcessor.getNumReady() < info.numSamples) 
    //    {
    //        DBG("need more samples: " << soundTouchProcessor.getNumReady());
    //        backgroundThread.moveToFrontOfQueue (this);
    //    }
    
    soundTouchProcessor.readSamples (info.buffer->getArrayOfChannels(), info.buffer->getNumChannels(),
                                     info.numSamples, info.startSample);

    const ScopedLock sl (bufferStartPosLock);
    numBuffered -= info.numSamples * soundTouchProcessor.getNumSamplesRequiredRatio();
    nextPlayPos += info.numSamples * soundTouchProcessor.getNumSamplesRequiredRatio();
    
    if (source->isLooping() && nextPlayPos > 0)
        nextPlayPos %= source->getTotalLength();
}

//==============================================================================
void SoundTouchAudioSource::setNextReadPosition (int64 newPosition)
{
    const ScopedLock sl (bufferStartPosLock);
    
    nextPlayPos = newPosition;
    numBuffered = 0;
    soundTouchProcessor.clear();
    buffer.clear();
    backgroundThread.moveToFrontOfQueue (this);
}

int64 SoundTouchAudioSource::getNextReadPosition() const
{
    jassert (source->getTotalLength() > 0);
    return (source->isLooping() && nextPlayPos > 0)
            ? nextPlayPos % source->getTotalLength()
            : nextPlayPos;
}

//==============================================================================
bool SoundTouchAudioSource::readNextBufferChunk()
{
    if (soundTouchProcessor.getNumReady() < numberOfSamplesToBuffer)
    {
        const ScopedLock sl (bufferStartPosLock);

        const int maxChunkSize = 2048;
        const int nextSourceReadPos = nextPlayPos + numBuffered;
        
        if (source->getNextReadPosition() != nextSourceReadPos)
            source->setNextReadPosition (nextSourceReadPos);
        
        AudioSourceChannelInfo info;
        info.buffer = &buffer;
        info.startSample = 0;
        info.numSamples = jlimit (0, maxChunkSize, buffer.getNumSamples());
        
        source->getNextAudioBlock (info);
        
        soundTouchProcessor.writeSamples (buffer.getArrayOfChannels(), buffer.getNumChannels(), info.numSamples);
        numBuffered += info.numSamples;
        
        return true;
    }
    
    return false;
}

int SoundTouchAudioSource::useTimeSlice()
{
    return readNextBufferChunk() ? 1 : 100;
}

END_JUCE_NAMESPACE