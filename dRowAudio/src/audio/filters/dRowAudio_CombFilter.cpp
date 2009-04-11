/*
 *  dRowAudio_CombFilter.cpp
 *
 *  Created by David Rowland on 06/04/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_CombFilter.h"

CombFilter::CombFilter(int bufferSize) throw()
	:	delayRegister(0),
		delaySamples(5),
		gain(1),
		ffCoeff(0.5),
		fbCoeff(0.5),
		bufferWritePos(0),
		bufferReadPos(0),
		allpassCoeff(1)
{
	setBufferSize(bufferSize);
}

CombFilter::~CombFilter() throw()
{
	delete[] delayRegister;
	delayRegister = 0;
}

void CombFilter::setBufferSize (int newBufferSize) throw()
{
	registerSize = pow(2, ((int)log2(newBufferSize)) + 1);
	registerSizeMask = registerSize - 1;

	delete[] delayRegister;
	delayRegister = new float[registerSize];
	// zero register
	for (int i = 0; i < registerSize; i++)
		delayRegister[i] = 0;	
}

void CombFilter::setMaxDelayTime(double sampleRate, float maxDelayTimeMs) throw()
{
	int newBufferSize = (maxDelayTimeMs * 0.001 ) * sampleRate;
	setBufferSize(newBufferSize );
}

void CombFilter::setGain(float newGain) throw()
{
	gain = newGain;
}
void CombFilter::setFFCoeff(float newFFCoeff) throw()
{
	ffCoeff = newFFCoeff;
}
void CombFilter::setFBCoeff(float newFBCoeff) throw()
{
	fbCoeff = newFBCoeff;
}

void CombFilter::setDelayTime(double sampleRate, float newDelayTime) throw()
{
	delayTime = newDelayTime;
	
	delaySamples = (delayTime * (sampleRate * 0.001));
	
	if ((int)delaySamples >= registerSize)
	{
		jassert(delaySamples < registerSize);
		delaySamples = (float)registerSize;
	}
}

void CombFilter::setAllpass(bool isAllpass_) throw()
{
	if (isAllpass_)
		allpassCoeff = -1;
	else
		allpassCoeff = 1;
}

float CombFilter::processSingleSample(float newSample) throw()
{
	bufferWritePos = ++bufferWritePos & registerSizeMask;
	
	bufferReadPos = bufferWritePos - delaySamples;
	if (bufferReadPos < 0)
		bufferReadPos += registerSize;

	// feedforward interpolation
	int iPos1 = (int)bufferReadPos;
	int iPos2 = (iPos1 + 1)  & registerSizeMask;
	float fDiff = bufferReadPos - iPos1;
	float fDel = delayRegister[iPos2]*fDiff + delayRegister[iPos1]*(1.0f-fDiff);
	
	// feedback
	float fBuff = (gain * newSample) + (fDel * fbCoeff);
	delayRegister[bufferWritePos] = fBuff;
	float fOut = fDel + (allpassCoeff * ffCoeff * fBuff);	

	return fOut;
}

void CombFilter::processSamples (float* const samples,
								 const int numSamples) throw()
{
    const ScopedLock sl (processLock);
	
	for (int i = 0; i < numSamples; ++i)
	{
		const float in = samples[i];
		
		bufferWritePos = ++bufferWritePos & registerSizeMask;
		
		bufferReadPos = bufferWritePos - delaySamples;
		if (bufferReadPos < 0)
			bufferReadPos += registerSize;
		
		// feedforward interpolation
		int iPos1 = (int)bufferReadPos;
		int iPos2 = (iPos1 + 1)  & registerSizeMask;
		float fDiff = bufferReadPos - iPos1;
		float fDel = delayRegister[iPos2]*fDiff + delayRegister[iPos1]*(1.0f-fDiff);
				
		float fBuff = (gain * in) + (fDel * fbCoeff);
		delayRegister[bufferWritePos] = fBuff;
		float fOut = fDel + (allpassCoeff * ffCoeff * fBuff);	
		
		samples[i] = fOut;
	}
}
