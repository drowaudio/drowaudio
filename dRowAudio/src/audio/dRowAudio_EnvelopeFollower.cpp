/*
 *  dRowAudio_EnvelopeFollower.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 16/09/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */
#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_EnvelopeFollower.h"

EnvelopeFollower::EnvelopeFollower()
	:	envelope(0),
		holdCount(0),
		hold(false)
{
}

EnvelopeFollower::~EnvelopeFollower()
{
}

void EnvelopeFollower::processEnvelope(float *inputBuffer, float *outputBuffer, int numSamples) throw()
{
	for(int i = 0; i < numSamples; ++i)
	{
		float envIn = inputBuffer[i] * inputBuffer[i];
		envIn = filter0.processSingleSample(envIn);	
		envIn = filter1.processSingleSample(envIn);	
		outputBuffer[i] = sqrt(envIn * 2.0f);
	}	
}

void EnvelopeFollower::processEnvelopeExp (float *inputBuffer, float *outputBuffer, int numSamples) throw()
{
	for(int i = 0; i < numSamples; ++i)
	{
		float envIn = fabsf(inputBuffer[i]);
		
		if(envelope < envIn)
		{
			envelope *= envAttack;
			envelope += (1.0f - envAttack) * envIn;
		}
		else
		{
			envelope *= envRelease;
			envelope += (1.0f - envRelease) * envIn;
		}
		
		outputBuffer[i] = envelope;
	}
}

void EnvelopeFollower::processEnvelopeExpHold (float *inputBuffer, float *outputBuffer, int numSamples) throw()
{
	for(int i = 0; i < numSamples; ++i)
	{
		float envIn = fabsf(inputBuffer[i]);
		
		if(envelope < envIn)
		{
			envelope *= envAttack;
			envelope += (1.0f - envAttack) * envIn;
			
			hold = true;
			holdCount = 0;
		}
		else
		{
			if (hold == true)
			{
				if (holdCount++ == holdLimit) {
					hold = false;
					holdCount = 0;
				}
			}
			else
			{
				envelope *= envRelease;
				envelope += (1.0f - envRelease) * envIn;
			}
		}
		
		outputBuffer[i] = envelope;
	}
}

void EnvelopeFollower::setCutoffFrequency(float sampleRate, float frequency) throw()
{
	cutoffFrequency = frequency;
	
	filter0.makeLowPass(sampleRate, cutoffFrequency);
	filter1.makeLowPass(sampleRate, cutoffFrequency);
}

void EnvelopeFollower::setTimes(float sampleRate, float attack, float release, float hold) throw()
{
	envAttack = (float) exp(-1/(attack * sampleRate * 0.001));
	envRelease = (float) exp(-1/(release * sampleRate * 0.001));
	holdLimit = hold * sampleRate * 0.001;	
}

END_DROWAUDIO_NAMESPACE