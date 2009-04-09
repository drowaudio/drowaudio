/*
 *  dRowAudio_AllpassFilter.h
 *
 *  Created by David Rowland on 09/04/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_ALLPASSFILTER_H_
#define _DROWAUDIO_ALLPASSFILTER_H_

#include <juce/juce.h>

#define BUFFERSIZE 4096

/**
	 Allpass Comb Filter.
	 
	 This allpass filter is a feedback and feedforward comb filter in series.
	 It has a unity frequency response but a complex phase response,typically
	 delaying sharp transients.
 */
class AllpassFilter
{
public:
	
	/** Creates a default filter ready to be used. 
	 */
	AllpassFilter() throw();
	
	/// Destructor
	~AllpassFilter() throw();
	
	/** Sets the feedback coefficient.
		This needs to be kept below 1 for stability.
		The higher the value, the longer the delay will last.
	 */	
	void setGain(float newGain) throw();

	///	Sets the time the samples are delayed for.
	void setDelayTime(double sampleRate, float newDelayTime) throw();
	
	/// Processes a single sample return a new, filtered value.
	float processSingleSample(float newSample) throw();
	
	/// Processes an array of samples which are modified.
	void processSamples (float* const samples,
						 const int numSamples) throw();
	
private:
	
	CriticalSection processLock;
	
	float* delayRegister;
	int registerSize, registerSizeMask;
	float delayTime, gain;
	int delaySamples, bufferWritePos, bufferReadPos;
};

#endif //_DROWAUDIO_ALLPASSFILTER_H_