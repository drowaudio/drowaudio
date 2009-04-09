/*
 *  dRowAudio_InterpolatingAllpassFilter.cpp
 *
 *  Created by David Rowland on 09/04/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_InterpolatingAllpassFilter.h"

InterpolatingAllpassFilter::InterpolatingAllpassFilter() throw()
	:	gain(0.1f),
		bufferWritePos(0)
{
	registerSize = BUFFERSIZE;
	registerSizeMask = registerSize - 1;
	
	delayRegister = new float[BUFFERSIZE];
	// zero register
	for (int i = 0; i < BUFFERSIZE; i++)
		delayRegister[i] = 0;	
}

InterpolatingAllpassFilter::~InterpolatingAllpassFilter() throw()
{
	delete[] delayRegister;	
}

void InterpolatingAllpassFilter::setGain(float newGain) throw()
{
	gain = newGain;
}

void InterpolatingAllpassFilter::setDelayTime(double sampleRate, float newDelayTime) throw()
{
	delayTime = newDelayTime;
	
	delaySamples = (delayTime * (sampleRate * 0.001));
	
	if ((int)delaySamples >= BUFFERSIZE)
	{
		jassert(delaySamples < BUFFERSIZE);
		delaySamples = (float)BUFFERSIZE;
	}	
}

float InterpolatingAllpassFilter::processSingleSample(float newSample) throw()
{
	bufferWritePos = ++bufferWritePos & registerSizeMask;
	
	bufferReadPos = bufferWritePos - delaySamples;
	if (bufferReadPos < 0)
		bufferReadPos += BUFFERSIZE;
	
	// interpolation
	int iPos1 = (int)bufferReadPos;
	int iPos2 = (iPos1 + 1) & registerSizeMask;
	float fDiff = bufferReadPos - iPos1;
	float fDel = delayRegister[iPos2]*fDiff + delayRegister[iPos1]*(1.0f-fDiff);
	
	delayRegister[bufferWritePos] = (gain * fDel) + newSample;
	float fOut = fDel - (gain * delayRegister[bufferWritePos]);
	
	return fOut;
}

void InterpolatingAllpassFilter::processSamples (float* const samples,
												 const int numSamples) throw()
{
	
}
