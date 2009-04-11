/*
 *  dRowAudio_CombFilter.h
 *
 *  Created by David Rowland on 06/04/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIOCOMBFILTER_H_
#define _DROWAUDIOCOMBFILTER_H_

#include <juce/juce.h>

/**
	This is an interpolating Comb Filter with both feedforward and feedback delay lines.
	This filter can be used as a building block of many effects.
 */
class CombFilter
{
public:
	
	/** Creates a default CombFilter.
		You can optionally set the size of the buffer in samples.
	 */
	CombFilter(int bufferSize =4096) throw();

	///	Destructor.
	~CombFilter() throw();

	/**	Resizes the buffer to a specified sample size.
	 
		Please note that as the buffer size has to be a power of 2, the maximum delay time
		possible may be larger than that specified. Use getMaxDelayTime() to find the
		actual maximum possible.
	 */
	void setBufferSize (int newBufferSize) throw();
	
	/// Returns the current size of the buffer.
	int getBufferSize ()	{	return registerSize;	}
	
	/**	Resizes the buffer to a specified size based on a given sample rate and delay time in Milliseconds.
		
		Please note that as the buffer size has to be a power of 2, the maximum delay time
		possible may be larger than that specified. Use getMaxDelayTime() to find the
		actual maximum possible.
	 */
	void setMaxDelayTime(double sampleRate, float maxDelayTimeMs) throw();
	
	/** Returns the maximum delay time possible for a given sample rate.
	 */
	float getMaxDelayTime(double sampleRate) {	return (registerSize / sampleRate) * 1000.0f;	}
	
	/// Sets the input gain of the filter.
	void setGain(float newGain) throw();
	
	/// Sets the feedforward coefficient.
	void setFFCoeff(float newFFCoeff) throw();

	/// Sets the feedback coefficient.
	void setFBCoeff(float newFBCoeff) throw();
	
	/**	Sets the delay time for the filter to use.
	 
		As the delay buffer is interpolated from the delay time can be
		any positive number as long as its within the buffer size.
		This parameter can also be swept for modulating effects such as
		chorus or flange.
	 */
	void setDelayTime(double sampleRate, float newDelayTime) throw();
	
	/** Setting this to true inverts the feedforward coefficient making
		the filter a posible allpass.
		
		To use the filter as an allpass set this to true and then use the
		same coefficient for feedforward and feedback. This will give unity
		gain across all frequencies but will have a complex frequency response
		typically delaying sharp transients.
	 */
	void setAllpass(bool isAllpass_) throw();
	
	/// Processes a single sample and returns a new, filtered value.
	float processSingleSample(float newSample) throw();
	
	/// Processes an array of samples which are modified.
	void processSamples (float* const samples,
						 const int numSamples) throw();

	
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