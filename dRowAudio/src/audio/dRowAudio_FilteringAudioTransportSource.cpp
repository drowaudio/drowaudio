/*
 *  FilteringAudioTransportSource.cpp
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_FilteringAudioTransportSource.h"

//========================================================================
FilteringAudioTransportSource::FilteringAudioTransportSource()
: source (0),
resamplerSource (0),
bufferingSource (0),
positionableSource (0),
masterSource (0),
gain (1.0f),
lastGain (1.0f),
playing (false),
stopped (true),
sampleRate (44100.0),
sourceSampleRate (0.0),
resamplingRatio (1.0),
blockSize (128),
readAheadBufferSize (0),
isPrepared (false),
inputStreamEOF (false),
lowEQGain(1.0),
midEQGain(1.0),
highEQGain(1.0),
filterSource(true)
{
	// instantiate the filters
	lowEQFilterLeft = new IIRFilter();
	lowEQFilterRight = new IIRFilter();
	midEQFilterLeft = new IIRFilter();
	midEQFilterRight = new IIRFilter();
	highEQFilterLeft = new IIRFilter();
	highEQFilterRight = new IIRFilter();
	
	lowEQFilterLeft->makeLowShelf(sampleRate, 70, 1.5, lowEQGain);
	lowEQFilterRight->makeLowShelf(sampleRate, 70, 1.5, lowEQGain);
	midEQFilterLeft->makeBandPass(sampleRate, 1000, 1.5, midEQGain);
	midEQFilterRight->makeBandPass(sampleRate, 1000, 1.5, midEQGain);
	highEQFilterLeft->makeHighShelf(sampleRate, 1300, 1.5, highEQGain);
	highEQFilterRight->makeHighShelf(sampleRate, 1300, 1.5, highEQGain);	
}

FilteringAudioTransportSource::~FilteringAudioTransportSource()
{
    setSource (0);
	
	deleteAndZero(lowEQFilterLeft);
	deleteAndZero(midEQFilterLeft);
	deleteAndZero(highEQFilterLeft);
	deleteAndZero(lowEQFilterRight);
	deleteAndZero(midEQFilterRight);
	deleteAndZero(highEQFilterRight);
	
    releaseResources();
}

void FilteringAudioTransportSource::setSource (PositionableAudioSource* const newSource,
                                      int readAheadBufferSize_,
                                      double sourceSampleRateToCorrectFor)
{
    if (source == newSource)
    {
        if (source == 0)
            return;
		
        setSource (0, 0, 0); // deselect and reselect to avoid releasing resources wrongly
    }
	
    readAheadBufferSize = readAheadBufferSize_;
    sourceSampleRate = sourceSampleRateToCorrectFor;
	
    ResamplingAudioSource* newResamplerSource = 0;
    BufferingAudioSource* newBufferingSource = 0;
    PositionableAudioSource* newPositionableSource = 0;
    AudioSource* newMasterSource = 0;
	
    ResamplingAudioSource* oldResamplerSource = resamplerSource;
    BufferingAudioSource* oldBufferingSource = bufferingSource;
    AudioSource* oldMasterSource = masterSource;
	
    if (newSource != 0)
    {
        newPositionableSource = newSource;
		
        if (readAheadBufferSize_ > 0)
            newPositionableSource = newBufferingSource
			= new BufferingAudioSource (newPositionableSource, false, readAheadBufferSize_);
		
        newPositionableSource->setNextReadPosition (0);
		
        if (sourceSampleRateToCorrectFor != 0)
            newMasterSource = newResamplerSource
			= new ResamplingAudioSource (newPositionableSource, false);
        else
            newMasterSource = newPositionableSource;
		
        if (isPrepared)
        {
            if (newResamplerSource != 0 && sourceSampleRate > 0 && sampleRate > 0)
                newResamplerSource->setResamplingRatio (sourceSampleRate / sampleRate);
			
            newMasterSource->prepareToPlay (blockSize, sampleRate);
        }
    }
	
    {
        const ScopedLock sl (callbackLock);
		
        source = newSource;
        resamplerSource = newResamplerSource;
        bufferingSource = newBufferingSource;
        masterSource = newMasterSource;
        positionableSource = newPositionableSource;
		
        playing = false;
    }
	
    if (oldMasterSource != 0)
        oldMasterSource->releaseResources();
	
    if (oldResamplerSource != 0)
        delete oldResamplerSource;
	
    if (oldBufferingSource != 0)
        delete oldBufferingSource;
}

void FilteringAudioTransportSource::start()
{
    if ((! playing) && masterSource != 0)
    {
        callbackLock.enter();
        playing = true;
        stopped = false;
        inputStreamEOF = false;
        callbackLock.exit();
		
        sendChangeMessage (this);
    }
}

void FilteringAudioTransportSource::stop()
{
    if (playing)
    {
        callbackLock.enter();
        playing = false;
        callbackLock.exit();
		
        int n = 500;
        while (--n >= 0 && ! stopped)
            Thread::sleep (2);
		
        sendChangeMessage (this);
    }
}

void FilteringAudioTransportSource::setPosition (double newPosition)
{
    if (sampleRate > 0.0)
        setNextReadPosition (roundDoubleToInt (newPosition * sampleRate));
}

double FilteringAudioTransportSource::getCurrentPosition() const
{
    if (sampleRate > 0.0)
        return getNextReadPosition() / sampleRate;
    else
        return 0.0;
}

void FilteringAudioTransportSource::setNextReadPosition (int newPosition)
{
    if (positionableSource != 0)
    {
        if (sampleRate > 0 && sourceSampleRate > 0)
            newPosition = roundDoubleToInt (newPosition * sourceSampleRate / sampleRate);
		
        positionableSource->setNextReadPosition (newPosition);
    }
}

int FilteringAudioTransportSource::getNextReadPosition() const
{
    if (positionableSource != 0)
    {
        const double ratio = (sampleRate > 0 && sourceSampleRate > 0) ? sampleRate / sourceSampleRate : 1.0;
		
        return roundDoubleToInt (positionableSource->getNextReadPosition() * ratio);
    }
	
    return 0;
}

int FilteringAudioTransportSource::getTotalLength() const
{
    const ScopedLock sl (callbackLock);
	
    if (positionableSource != 0)
    {
        const double ratio = (sampleRate > 0 && sourceSampleRate > 0) ? sampleRate / sourceSampleRate : 1.0;
		
        return roundDoubleToInt (positionableSource->getTotalLength() * ratio);
    }
	
    return 0;
}

bool FilteringAudioTransportSource::isLooping() const
{
    const ScopedLock sl (callbackLock);
	
    return positionableSource != 0
	&& positionableSource->isLooping();
}

void FilteringAudioTransportSource::setGain (const float newGain) throw()
{
    gain = newGain;
}

void FilteringAudioTransportSource::setLowEQGain(float newLowEQGain)
{
	lowEQGain = newLowEQGain;
	
	lowEQFilterLeft->makeLowShelf(sampleRate, 500, 1, lowEQGain);
	lowEQFilterRight->makeLowShelf(sampleRate, 500, 1, lowEQGain);
}

void FilteringAudioTransportSource::setMidEQGain(float newMidEQGain)
{
	midEQGain = newMidEQGain;
	
	midEQFilterLeft->makeBandPass(sampleRate, 2000, 1, midEQGain);
	midEQFilterRight->makeBandPass(sampleRate, 2000, 1, midEQGain);
}

void FilteringAudioTransportSource::setHighEQGain(float newHighEQGain)
{
	highEQGain = newHighEQGain;
	
	highEQFilterLeft->makeHighShelf(sampleRate, 3500, 1, highEQGain);
	highEQFilterRight->makeHighShelf(sampleRate, 3500, 1, highEQGain);	
}

void FilteringAudioTransportSource::setFilterSource(bool shouldFilter)
{
	filterSource = shouldFilter;
}

void FilteringAudioTransportSource::setResamplingRatio (const double samplesInPerOutputSample)
{
	resamplingRatio = samplesInPerOutputSample;
	
	if (resamplerSource != 0)
		resamplerSource->setResamplingRatio ((sourceSampleRate / sampleRate) * resamplingRatios);
}

void FilteringAudioTransportSource::prepareToPlay (int samplesPerBlockExpected,
												   double sampleRate_)
{
    const ScopedLock sl (callbackLock);
	
    sampleRate = sampleRate_;
    blockSize = samplesPerBlockExpected;
	
    if (masterSource != 0)
        masterSource->prepareToPlay (samplesPerBlockExpected, sampleRate);
	
    if (resamplerSource != 0 && sourceSampleRate != 0)
        resamplerSource->setResamplingRatio (sourceSampleRate / sampleRate);
	
    isPrepared = true;
}

void FilteringAudioTransportSource::releaseResources()
{
    const ScopedLock sl (callbackLock);
	
    if (masterSource != 0)
        masterSource->releaseResources();
	
    isPrepared = false;
}

void FilteringAudioTransportSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    const ScopedLock sl (callbackLock);
	
    inputStreamEOF = false;
	
    if (masterSource != 0 && ! stopped)
    {
        masterSource->getNextAudioBlock (info);
		
        if (! playing)
        {
            // just stopped playing, so fade out the last block..
            for (int i = info.buffer->getNumChannels(); --i >= 0;)
                info.buffer->applyGainRamp (i, info.startSample, jmin (256, info.numSamples), 1.0f, 0.0f);
			
            if (info.numSamples > 256)
                info.buffer->clear (info.startSample + 256, info.numSamples - 256);
        }
		
        if (positionableSource->getNextReadPosition() > positionableSource->getTotalLength() + 1
			&& ! positionableSource->isLooping())
        {
            playing = false;
            inputStreamEOF = true;
            sendChangeMessage (this);
        }
		
        stopped = ! playing;
		
        for (int i = info.buffer->getNumChannels(); --i >= 0;)
        {
            info.buffer->applyGainRamp (i, info.startSample, info.numSamples,
                                        lastGain, gain);
        }
		
		if (filterSource)
		{
			// filter samples
			const int bufferNumSamples = info.numSamples;
			float *leftSampleData = info.buffer->getSampleData(0, info.startSample);
			float *rightSampleData = info.buffer->getSampleData(1, info.startSample);
			
			lowEQFilterLeft->processSamples(leftSampleData, bufferNumSamples);
			lowEQFilterRight->processSamples(rightSampleData, bufferNumSamples);
			midEQFilterLeft->processSamples(leftSampleData, bufferNumSamples);
			midEQFilterRight->processSamples(rightSampleData, bufferNumSamples);
			highEQFilterLeft->processSamples(leftSampleData, bufferNumSamples);
			highEQFilterRight->processSamples(rightSampleData, bufferNumSamples);
		}
    }
    else
    {
        info.clearActiveBufferRegion();
        stopped = true;
    }
	
    lastGain = gain;
}

