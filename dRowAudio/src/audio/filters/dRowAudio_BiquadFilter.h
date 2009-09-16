/*
 *  dRowAudio_BiquadFilter.h
 *
 *  Created by David Rowland on 11/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _BIQUADFILTER_H_
#define _BIQUADFILTER_H_

#include <juce/juce.h>

/**
	A Biquad filter.
 
	This filter is a subclass of the Juce IIR filter but uses
	some additional methods to give more filter designs.
 */
class BiquadFilter : public IIRFilter
{
public:
	
	/**	Makes the filter a Low-pass filter.
	 */
	void makeLowPass(const double sampleRate,
					   const double frequency) throw();
	
	/**	Makes the filter a High-pass filter.
	 */
	void makeHighPass(const double sampleRate,
						const double frequency) throw();
	
	/**	Makes the filter a Band-pass filter.
	 */
	void makeBandPass(const double sampleRate,
						const double frequency,
						const double Q) throw();
	
	/**	Makes the filter a Band-stop filter.
	 */
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
	
    /** Makes this filter duplicate the set-up of another one.
	 */
    void copyCoefficientsFrom (const BiquadFilter& other) throw();
	
	/** Makes this filter duplicate the set-up of another one.
	 */
    void copyOutputsFrom (const BiquadFilter& other) throw();
	
};

#endif //_BIQUADFILTER_H_