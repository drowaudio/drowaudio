/*
 *  dRowAudio_BiquadFilter.h
 *
 *  Created by David Rowland on 11/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

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
	void createLowPass(const double sampleRate,
					   const double frequency);
	
	/**	Makes the filter a High-pass filter.
	 */
	void createHighPass(const double sampleRate,
						const double frequency);
	
	/**	Makes the filter a Band-pass filter.
	 */
	void createBandPass(const double sampleRate,
						const double frequency,
						const double Q);
	
	/**	Makes the filter a Band-stop filter.
	 */
	void createBandStop(const double sampleRate,
						const double frequency,
						const double Q);
	
	/**	Makes the filter an Allpass filter.
		This type of filter has a complex phase response so will give a comb 
		filtered effect when combined with an unfilterd copy of the signal.
	 */
	void createAllpass(const double sampleRate,
					   const double frequency,
					   const double Q);
};