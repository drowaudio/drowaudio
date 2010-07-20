/*
 *  dRowAudio_LBCF.h
 *
 *  Created by David Rowland on 07/04/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_INTERPOLATINGLBCF_H_
#define _DROWAUDIO_INTERPOLATINGLBCF_H_

#include "../../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_OnePoleFilter.h"

#define BUFFERSIZE 4096

/**
	Interpolating, Lowpassed Feedback Comb Filter.
	
	This feeback comb filter has a one-pole lowpass filter in the delay line.
	This can be used for damped effects such as reverb or delays.
	
	This filter also uses linear interolation to allow fractional buffer positions.
	This means the delay time can be swept to give modulating effects such as flangers.
 */
class InterpolatingLBCF
{
public:
	
	/** Creates a default filter ready to be used. 
	 */
	InterpolatingLBCF() throw();
	/// Destructor
	~InterpolatingLBCF() throw();
	
	/// Sets the input gain of the filter
	void setGain(float newGain) throw();
	
	/** Sets the feedback coefficient.
		This needs to be kept below 1 for stability.
		The higher the value, the longer the delay will last.
	 */
	void setFBCoeff(float newFBCoeff) throw();
	
	/**	Sets the time the samples are delayed for.
		Values < 20 will give flanging effects 20 - 90 for reverbs and 90+ to hear distinct echoes.
	 */
	void setDelayTime(double sampleRate, float newDelayTime) throw();
	
	/**	Sets the cutoff frequency of the lowpass filter.
		This can be used to model 'liveliness' in the room.
	 */
	void setLowpassCutoff(double sampleRate, float cutoffFrequency) throw();
	
	/// Processes a single sample return a new, filtered value.
	float processSingleSample(float newSample) throw();
	
	/// Processes an array of samples which are modified.
	void processSamples (float* const samples,
						 const int numSamples) throw();
	
private:
	
	CriticalSection processLock;
	OnePoleFilter lowpassFilter;
	
	float* delayRegister;
	int registerSize, registerSizeMask;
	float delayTime, delaySamples, gain, fbCoeff;
	int bufferWritePos;
	float bufferReadPos;
};

#endif //_DROWAUDIO_INTERPOLATINGLBCF_H_