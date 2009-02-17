/*
 *  dRowAudio_OnePoleFilter.h
 *  dRowAudio
 *
 *  Created by David Rowland on 16/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _ONEPOLEFILTER_H_
#define _ONEPOLEFILTER_H_

#include <juce/juce.h>

//==============================================================================
/**	One-Pole Filter
 
	This is a simple filter that uses only one pole. As such it is very
	computationaly efficient especially if used to process a buffer of samples.
 */

class OnePoleFilter
{
public:
	
	OnePoleFilter();
	~OnePoleFilter();
	
	void processSamples(float* const pfSamples,
						const int numSamples);
	
	float processSingleSample(const float sampleToProcess);
	
	void makeLowPass(const double sampleRate,
					 const double frequency);

	void makeHighPass(const double sampleRate,
					  const double frequency);

private:
	CriticalSection lock;
	
	float a1, b0, x, x1;
};


#endif // _ONEPOLEFILTER_H_