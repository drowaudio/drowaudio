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

#include "../../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_InterpolatingLBCF.h"

InterpolatingLBCF::InterpolatingLBCF() throw()
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

InterpolatingLBCF::~InterpolatingLBCF() throw()
{
	delete[] delayRegister;
}

void InterpolatingLBCF::setGain(float newGain) throw()
{
	gain = newGain;
}

void InterpolatingLBCF::setFBCoeff(float newFBCoeff) throw()
{
	fbCoeff = newFBCoeff;
}

void InterpolatingLBCF::setDelayTime(double sampleRate, float newDelayTime) throw()
{
	delayTime = newDelayTime;
	
	delaySamples = (delayTime * (sampleRate * 0.001));
	
	if ((int)delaySamples >= BUFFERSIZE)
	{
		jassert(delaySamples < BUFFERSIZE);
		delaySamples = (float)BUFFERSIZE;
	}
}

void InterpolatingLBCF::setLowpassCutoff(double sampleRate, float cutoffFrequency) throw()
{
	lowpassFilter.makeLowPass(sampleRate, cutoffFrequency);
}

float InterpolatingLBCF::processSingleSample(float newSample) throw()
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

void InterpolatingLBCF::processSamples (float* const samples,
										const int numSamples) throw()
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

END_DROWAUDIO_NAMESPACE