/*
 *  dRowAudio_CombFilter.h
 *  dRowAudio_Reverb
 *
 *  Created by David Rowland on 06/04/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWAUDIOCOMBFILTER_H_
#define _DROWAUDIOCOMBFILTER_H_

#include <juce/juce.h>

#define BUFFERSIZE 65536

class CombFilter
{
public:
	
	CombFilter();
	~CombFilter();
	
	void setGain(float newGain);
	void setFFCoeff(float newFFCoeff);
	void setFBCoeff(float newFBCoeff);
	void setDelayTime(double sampleRate, float newDelayTime);
	
	void setAllpass(bool isAllpass_);
	
	float processSingleSample(float newSample);
	void processSamples (float* const samples,
						 const int numSamples);

	
protected:

	CriticalSection processLock;

	float* delayRegister;
	int registerSize, registerSizeMask;
	float delayTime, delaySamples, gain, ffCoeff, fbCoeff;
	int bufferWritePos;
	float bufferReadPos;
	
	int allpassCoeff;
};

#endif //_DROWAUDIOCOMBFILTER_H_