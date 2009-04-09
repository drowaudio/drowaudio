/*
 *  dRowAudio_CombFilter.cpp
 *  dRowAudio_Reverb
 *
 *  Created by David Rowland on 06/04/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_CombFilter.h"

CombFilter::CombFilter()
	:	delaySamples(5),
		gain(1),
		ffCoeff(0.5),
		allpassCoeff(1)
{
	registerSize = BUFFERSIZE;
	registerSizeMask = registerSize - 1;
	delayRegister = new float[BUFFERSIZE];
	// zero register
	for (int i = 0; i < BUFFERSIZE; i++)
		delayRegister[i] = 0;
	
	bufferWritePos = 0;
}

CombFilter::~CombFilter()
{
	delete[] delayRegister;
}

void CombFilter::setGain(float newGain)
{
	gain = newGain;
}
void CombFilter::setFFCoeff(float newFFCoeff)
{
	ffCoeff = newFFCoeff;
}
void CombFilter::setFBCoeff(float newFBCoeff)
{
	fbCoeff = newFBCoeff;
}

void CombFilter::setDelayTime(double sampleRate, float newDelayTime)
{
	delayTime = newDelayTime;
	
	delaySamples = (delayTime * (sampleRate * 0.001));
	
	if ((int)delaySamples >= BUFFERSIZE)
	{
		jassert(delaySamples < BUFFERSIZE);
		delaySamples = (float)BUFFERSIZE;
	}
}

void CombFilter::setAllpass(bool isAllpass_)
{
	if (isAllpass_)
		allpassCoeff = -1;
	else
		allpassCoeff = 1;
}

float CombFilter::processSingleSample(float newSample)
{
	bufferWritePos = ++bufferWritePos & registerSizeMask;
	
	bufferReadPos = bufferWritePos - delaySamples;
	if (bufferReadPos < 0)
		bufferReadPos += BUFFERSIZE;

	// feedforward interpolation
	int iPos1 = (int)bufferReadPos;
	int iPos2 = (iPos1 + 1)  & registerSizeMask;
	float fDiff = bufferReadPos - iPos1;
	float fDel = delayRegister[iPos2]*fDiff + delayRegister[iPos1]*(1.0f-fDiff);
	
	float fOut = (gain * newSample) + (fDel);	

	// feedback
	delayRegister[bufferWritePos] = ((allpassCoeff * ffCoeff) * newSample) + (fOut * fbCoeff);
	
	return fOut;
}

void CombFilter::processSamples (float* const samples,
								 const int numSamples)
{
    const ScopedLock sl (processLock);
	
	for (int i = 0; i < numSamples; ++i)
	{
		const float in = samples[i];
		
		bufferWritePos = ++bufferWritePos & registerSizeMask;
		
		bufferReadPos = bufferWritePos - delaySamples;
		if (bufferReadPos < 0)
			bufferReadPos += BUFFERSIZE;
		
		// feedforward interpolation
		int iPos1 = (int)bufferReadPos;
		int iPos2 = (iPos1 + 1)  & registerSizeMask;
		float fDiff = bufferReadPos - iPos1;
		float fDel = delayRegister[iPos2]*fDiff + delayRegister[iPos1]*(1.0f-fDiff);
		
//		float fOut = (ffCoeff * in) + fDel;
//		delayRegister[bufferWritePos] = (fbCoeff * fDel) + in;
		
		float fOut = (gain * in) + (fDel);	
		
		// feedback
		delayRegister[bufferWritePos] = ((allpassCoeff * ffCoeff) * in) + (fOut * fbCoeff);
					
		samples[i] = fOut;
	}
}
