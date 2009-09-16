/*
 *  dRowAudio_CascadedBiquadFilter.h
 *
 *  Created by David Rowland on 11/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _CASCADEDBIQUADFILTER_H_
#define _CASCADEDBIQUADFILTER_H_

#include <juce/juce.h>
#include "dRowAudio_BiquadFilter.h"

/**
	A cascaded set of Biquad filters.
 
	This class can be used to easily cascade a series of biquad filters
	in order to create sharp cutoffs.
 */
class CascadedBiquadFilter
{
public:
	CascadedBiquadFilter();
		
	/// Destructor
	~CascadedBiquadFilter();
	
	void setOrder(int newOrder);

	/// Returns the current order of the filter.
	int getOrder()	{	return currentOrder;	}

	/**	Returns the current roll-off slope of the filter is in one of the pass modes;
		This is in dB per octave
	 */
	int getRolloff()	{	return (currentOrder * 12);	}
	
	/** Performs the filter operation on the given set of samples.
	 */
    void processSamples (float* const samples,
                         const int numSamples) throw();
	
    /** Processes a single sample, without any locking or checking.
	 
		Use this if you need fast processing of a single value, but be aware that
		this isn't thread-safe in the way that processSamples() is.
	 */
    float processSingleSampleRaw (const float sample) throw();
	
	///	Makes the filter a Low-pass filter.
	void makeLowPass(const double sampleRate,
					   const double frequency) throw();
	
	///	Makes the filter a High-pass filter.
	void makeHighPass(const double sampleRate,
						const double frequency) throw();
	
	///	Makes the filter a Band-pass filter.
	void makeBandPass(const double sampleRate,
						const double frequency,
						const double Q) throw();
	
	///	Makes the filter a Band-stop filter.
	void makeBandStop(const double sampleRate,
						const double frequency,
						const double Q) throw();
	
	/**	Makes the filter a peak/notch filter. This type of filter
		adds or subtracts from the unfiltered signal.
	 */
	void makePeakNotch(const double sampleRate,
					   const double frequency,
					   const double Q,
					   const float gainFactor) throw();	
	
	/**	Makes the filter an Allpass filter.
		This type of filter has a complex phase response so will give a comb 
		filtered effect when combined with an unfilterd copy of the signal.
	 */
	void makeAllpass(const double sampleRate,
					   const double frequency,
					   const double Q) throw();
	
private:
		
	OwnedArray <BiquadFilter> filterArray;
	double currentFrequency;
	int currentOrder;
};

#endif //_CASCADEDBIQUADFILTER_H_