/*
 *  dRowAudio_OnePoleFilter.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 16/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_OnePoleFilter.h"

OnePoleFilter::OnePoleFilter() throw()
	:	y1(0.0), b0(1.0), a1(0.0)
{
	
}

OnePoleFilter::~OnePoleFilter() throw()
{
}

void OnePoleFilter::processSamples(float* const pfSamples,
								   const int numSamples) throw()
{
	// make sure sample values are locked
	lock.enter();
	
	for (int i = 0; i < numSamples; ++i)
	{
		pfSamples[i] = (b0 * pfSamples[i]) + (a1 * y1);
		y1 = pfSamples[i];
	}
	
	lock.exit();
}


void OnePoleFilter::makeLowPass(const double sampleRate,
								const double frequency) throw()
{
	float w0 = 2.0*double_Pi*(frequency/sampleRate);
	float cos_w0 = cos(w0);

	float alpha = (2.0-cos_w0) - sqrt((2.0-cos_w0)*(2.0-cos_w0) - 1.0);
	b0 = 1.0-alpha;
	a1 = alpha;
}

void OnePoleFilter::makeHighPass(const double sampleRate,
								 const double frequency) throw()
{
	float w0 = 2.0*double_Pi*(frequency/sampleRate);
	float cos_w0 = cos(w0);
	
	float alpha = (2.0+cos_w0) - sqrt((2.0+cos_w0)*(2.0+cos_w0) - 1.0);
	b0 = alpha - 1.0;
	a1 = -alpha;
}