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

#include "dRowAudio_AllpassFilter.h"

AllpassFilter::AllpassFilter() throw()
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

AllpassFilter::~AllpassFilter() throw()
{
	delete[] delayRegister;	
}

void AllpassFilter::setGain(float newGain) throw()
{
	gain = newGain;
}

void AllpassFilter::setDelayTime(double sampleRate, float newDelayTime) throw()
{
	delayTime = newDelayTime;
	
	delaySamples = (int)(delayTime * (sampleRate * 0.001));
	
	if (delaySamples >= BUFFERSIZE)
	{
		jassert(delaySamples < BUFFERSIZE);
		delaySamples = BUFFERSIZE;
	}	
}

float AllpassFilter::processSingleSample(float newSample) throw()
{
	bufferWritePos = ++bufferWritePos & registerSizeMask;
	
	bufferReadPos = bufferWritePos - delaySamples;
	if (bufferReadPos < 0)
		bufferReadPos += BUFFERSIZE;
	
	float fDel = delayRegister[bufferReadPos];
	delayRegister[bufferWritePos] = (gain * fDel) + newSample;
	float fOut = fDel - (gain * delayRegister[bufferWritePos]);
	
	return fOut;
}

void AllpassFilter::processSamples (float* const samples,
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
		
		float fDel = delayRegister[bufferReadPos];
		delayRegister[bufferWritePos] = (gain * fDel) + in;
		float fOut = fDel - (gain * delayRegister[bufferWritePos]);
		
		samples[i] = fOut;
	}
}

END_DROWAUDIO_NAMESPACE