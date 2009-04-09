/*
 *  dRowAudio_AllpassFilter.h
 *  dRowAudio_Reverb
 *
 *  Created by David Rowland on 09/04/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_ALLPASSFILTER_H_
#define _DROWAUDIO_ALLPASSFILTER_H_

#include <juce/juce.h>

#define BUFFERSIZE 4096

class AllpassFilter
{
public:
	
	AllpassFilter();
	~AllpassFilter();
	
	void setGain(float newGain);
	void setDelayTime(double sampleRate, float newDelayTime);
	
	float processSingleSample(float newSample);
	void processSamples (float* const samples,
						 const int numSamples);
	
protected:
	
	CriticalSection processLock;
	
	float* delayRegister;
	int registerSize, registerSizeMask;
	float delayTime, delaySamples, gain;
	int bufferWritePos;
	float bufferReadPos;
};

#endif //_DROWAUDIO_ALLPASSFILTER_H_