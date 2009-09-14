/*
 *  dRowAudio_OnePoleOneZeroFilter.h
 *  dRowAudio
 *
 *  Created by David Rowland on 16/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _ONEPOLEONEZEROFILTER_H_
#define _ONEPOLEONEZEROFILTER_H_

#include <juce/juce.h>
#include "../../utility/dRowAudio_Constants.h"

//==============================================================================
/**	One-Pole, One-Zero Filter
 
	This is a simple filter that uses one pole and one zero. As such it is very
	computationaly efficient especially if used to process a buffer of samples.
 */
class OnePoleOneZeroFilter
{
public:
	
	/**	Create an unititialised filter.
		This will not perform any filtering yet, call a make... method
		to turn it into that particular type of filter.
	 */
	OnePoleOneZeroFilter() throw();
	
	/// Destructor
	~OnePoleOneZeroFilter() throw();
	
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
		y1 = (sampleToProcess * a0) + (x1 * a1) + (y1 * b1);
		x1 = sampleToProcess;
		
		return y1;
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
	
	float x1, y1, a0, b1, a1;
};


#endif // _ONEPOLEONEZEROFILTER_H_