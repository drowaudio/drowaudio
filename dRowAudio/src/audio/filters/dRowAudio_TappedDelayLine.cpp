/*
 *  dRowAudio_TappedDelayLine.cpp
 *
 *  Created by David Rowland on 13/04/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_TappedDelayLine.h"

TappedDelayLine::TappedDelayLine(int initialBufferSize)
	:	pfDelayBuffer(0),
		bufferSize(initialBufferSize),
		bufferWritePos(0),
		inputGain(1.0f),
		feedbackGain(0.5f)
{
	initialiseBuffer(bufferSize);
}

TappedDelayLine::TappedDelayLine(float bufferLengthMs, double sampleRate)
	:	pfDelayBuffer(0),
		bufferWritePos(0),
		inputGain(1.0f),
		feedbackGain(0.5f)
{
	bufferSize = (int)((bufferLengthMs * 0.001) * sampleRate);
	
	initialiseBuffer(bufferSize);
}

void TappedDelayLine::initialiseBuffer(int bufferSize)
{
	delete[] pfDelayBuffer;
	
	pfDelayBuffer = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++)
		pfDelayBuffer[i] = 0;		
}

TappedDelayLine::~TappedDelayLine()
{
	readTaps.clear();
	delete[] pfDelayBuffer;
	pfDelayBuffer = 0;
}

void TappedDelayLine::addTap(int noDelaySamples, int sampleRate)
{
	jassert(noDelaySamples < bufferSize);
	
	Tap newTap;
	newTap.delaySamples = noDelaySamples;
	newTap.sampleRateWhenCreated = sampleRate;
	newTap.tapGain = 0.15f;
	
	readTaps.add(newTap);
	
	noTaps = readTaps.size();	
}

void TappedDelayLine::addTapAtTime(int newTapPosMs, double sampleRate)
{
	int newTapPosSamples = (int)((newTapPosMs * 0.001) * sampleRate);
	
	addTap(newTapPosSamples, sampleRate);
}

Array<int> TappedDelayLine::getTapSamplePositions()
{
	Array<int> tapSamplePositions;
	
	for (int i = 0; i < readTaps.size(); i++)
		tapSamplePositions.add(readTaps[i].delaySamples);
	
	return tapSamplePositions;
}

bool TappedDelayLine::removeTapAtIndex(int tapIndex)
{
	readTaps.remove(tapIndex);
	if (readTaps.size() < noTaps) {
		noTaps = readTaps.size();
		return true;
	}
	
	return false;
}

bool TappedDelayLine::removeTapAtSample(int sampleForRemovedTap)
{
	for (int i = 0; i < readTaps.size(); i++)
		if (readTaps[i].delaySamples == sampleForRemovedTap) {
			readTaps.remove(i);
			noTaps = readTaps.size();
			return true;
		}
	
	return false;
}

bool TappedDelayLine::removeTapAtMs(int timeMsForRemovedTap, int sampleRate)
{
	int tapsample = timeMsForRemovedTap * 0.001 * sampleRate;
	
	return removeTapAtSample(tapsample);
}

void TappedDelayLine::removeAllTaps()
{
	readTaps.clear();
	noTaps = readTaps.size();
}

void TappedDelayLine::updateDelayTimes(double newSampleRate)
{
	for (int i = 0; i < readTaps.size(); i++)
	{
		if ( (int)readTaps[i].sampleRateWhenCreated != 0
			 && !almostEqual(newSampleRate, readTaps[i].sampleRateWhenCreated) )
		{
			double scale = (newSampleRate / readTaps[i].sampleRateWhenCreated);
			readTaps[i].delaySamples *= scale;
			readTaps[i].sampleRateWhenCreated = newSampleRate;
		}
	}
}

float TappedDelayLine::processSingleSample(float newSample) throw()
{
	// incriment buffer position and store new sample
	if(++bufferWritePos > bufferSize)
		bufferWritePos = 0;
		
	float fOut = (inputGain * newSample);
	for (int i = 0; i < noTaps; i++)
	{
		Tap currentTap = readTaps.getUnchecked(i);

		int tapReadPos = bufferWritePos - currentTap.delaySamples;
		if (tapReadPos < 0)
			tapReadPos += bufferSize;
		
		fOut += (currentTap.tapGain * pfDelayBuffer[tapReadPos]);
	}
	
	pfDelayBuffer[bufferWritePos] = (newSample + (feedbackGain * fOut));

	return fOut;
}
