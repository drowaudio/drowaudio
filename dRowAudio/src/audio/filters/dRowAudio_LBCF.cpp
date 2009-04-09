/*
 *  dRowAudio_LBCF.cpp
 *  dRowAudio_Reverb
 *
 *  Created by David Rowland on 07/04/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_LBCF.h"

LBCF::LBCF()
	:	gain(1),
		fbCoeff(0.5),
		bufferWritePos(0)
{
	registerSize = BUFFERSIZE;
	registerSizeMask = registerSize - 1;
	
	delayRegister = new float[BUFFERSIZE];
	// zero register
	for (int i = 0; i < BUFFERSIZE; i++)
		delayRegister[i] = 0;
}

LBCF::~LBCF()
{
	delete[] delayRegister;
}

void LBCF::setGain(float newGain)
{
	gain = newGain;
}

void LBCF::setFBCoeff(float newFBCoeff)
{
	fbCoeff = newFBCoeff;
}

void LBCF::setDelayTime(double sampleRate, float newDelayTime)
{
	delayTime = newDelayTime;
	
	delaySamples = (delayTime * (sampleRate * 0.001));
	
	if ((int)delaySamples >= BUFFERSIZE)
	{
		jassert(delaySamples < BUFFERSIZE);
		delaySamples = (float)BUFFERSIZE;
	}
}

void LBCF::setLowpassCutoff(double sampleRate, float cutoffFrequency)
{
	lowpassFilter.makeLowPass(sampleRate, cutoffFrequency);
}

float LBCF::processSingleSample(float newSample)
{
	bufferWritePos = ++bufferWritePos & registerSizeMask;
	
	bufferReadPos = bufferWritePos - delaySamples;
	if (bufferReadPos < 0)
		bufferReadPos += BUFFERSIZE;
	
	// interpolation
	int iPos1 = (int)bufferReadPos;
	int iPos2 = (iPos1 + 1)  & registerSizeMask;
	float fDiff = bufferReadPos - iPos1;
	float fDel = delayRegister[iPos2]*fDiff + delayRegister[iPos1]*(1.0f-fDiff);
	
	float fOut = (gain * newSample) + fDel;	

	// feedback and lowpass
	delayRegister[bufferWritePos] = lowpassFilter.processSingleSample(fbCoeff *fOut);
	
	return fOut;
}

void LBCF::processSamples (float* const samples,
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
		
		// interpolation
		int iPos1 = (int)bufferReadPos;
		int iPos2 = (iPos1 + 1)  & registerSizeMask;
		float fDiff = bufferReadPos - iPos1;
		float fDel = delayRegister[iPos2]*fDiff + delayRegister[iPos1]*(1.0f-fDiff);
				
		float fOut = (gain * in) + fDel;	
		
		// feedback and lowpass
		delayRegister[bufferWritePos] = lowpassFilter.processSingleSample(fbCoeff * fOut);
		
		samples[i] = fOut;
	}	
}
