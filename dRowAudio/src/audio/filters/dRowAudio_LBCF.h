/*
 *  dRowAudio_LBCF.h
 *  dRowAudio_Reverb
 *
 *  Created by David Rowland on 07/04/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_LBCF_H_
#define _DROWAUDIO_LBCF_H_

#include <dRowAudio/dRowAudio.h>

#define BUFFERSIZE 4096

class LBCF
{
public:
	
	LBCF();
	~LBCF();
	
	void setGain(float newGain);
	void setFBCoeff(float newFBCoeff);
	void setDelayTime(double sampleRate, float newDelayTime);	
	void setLowpassCutoff(double sampleRate, float cutoffFrequency);
	
	float processSingleSample(float newSample);
	void processSamples (float* const samples,
						 const int numSamples);
	
private:
	
	CriticalSection processLock;
	OnePoleFilter lowpassFilter;
	
	float* delayRegister;
	int registerSize, registerSizeMask;
	float delayTime, delaySamples, gain, fbCoeff;
	int bufferWritePos;
	float bufferReadPos;
};

#endif //_DROWAUDIO_LBCF_H_