/*
 *  dRowAudio_OnePoleFilter.h
 *  dRowAudio
 *
 *  Created by David Rowland on 16/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _ONEPOLEFILTER_H_
#define _ONEPOLEFILTER_H_

#include "../../core/dRowAudio_StandardHeader.h"

//==============================================================================
/**	One-Pole Filter
 
	This is a simple filter that uses only one pole. As such it is very
	computationaly efficient especially if used to process a buffer of samples.
	It has a slope of -6dB/octave.
 */
class OnePoleFilter
{
public:
	
	/**	Create an unititialised filter.
		This will not perform any filtering yet, call a make... method
		to turn it into that particular type of filter.
	 */
	OnePoleFilter() throw();
	
	/// Destructor
	~OnePoleFilter() throw();
	
	/**	Process a number of samples in one go.
		This is the most effecient method of filtering.
		Note that the samples passed to it actually get changed.
	 */
	void processSamples(float* const pfSamples,
						const int numSamples) throw();
	
	/**	Process a single sample.
		Less efficient method but leaves the sample unchanged,
		returning a filterd copy of it.
	 */
	forcedinline float processSingleSample(const float sampleToProcess) throw()
	{
		return y1 = (b0 * sampleToProcess) + (a1 * y1);
	}
	
	/**	Turns the filter into a Low-pass.
	 */
	void makeLowPass(const double sampleRate,
					 const double frequency) throw();
	/**	Turns the filter into a High-pass.
	 */
	void makeHighPass(const double sampleRate,
					  const double frequency) throw();

private:
	CriticalSection lock;
	
	float y1, b0, a1;
};

#endif // _ONEPOLEFILTER_H_