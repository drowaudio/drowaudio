/*
 *  dRowAudio_OnePoleFilter.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 16/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_OnePoleFilter.h"

OnePoleFilter::OnePoleFilter()
	:	x(0.0), y1(0.0), b0(1.0), a1(0.0)
{
	
}

OnePoleFilter::~OnePoleFilter()
{
}

void OnePoleFilter::processSamples(float* const pfSamples,
								   const int numSamples)
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

float OnePoleFilter::processSingleSample(const float sampleToProcess)
{
	return y1 = (b0 * sampleToProcess) + (a1 * y1);
}

void OnePoleFilter::makeLowPass(const double sampleRate,
								const double frequency)
{
	float w0 = 2.0*double_Pi*(frequency/sampleRate);
	float cos_w0 = cos(w0);
//	float sin_w0 = sin(w0);
	
	// working method
/*	float alpha = exp(-2.0*double_Pi*frequency/sampleRate);
	b0 = 1.0-alpha;
	a1 = -alpha;
*/	
	float alpha = (2.0-cos_w0) - sqrt((2.0-cos_w0)*(2.0-cos_w0) - 1.0);
	b0 = 1.0-alpha;
	a1 = alpha;

//	LP:
//	recursion: tmp = (1-p)*in + p*tmp with output = tmp
//	coefficient: p = (2-cos(x)) - sqrt((2-cos(x))^2 - 1) with x = 2*pi*cutoff/samplerate
//	coeficient approximation: p = (1 - 2*cutoff/samplerate)^2
}

void OnePoleFilter::makeHighPass(const double sampleRate,
								 const double frequency)
{
	float w0 = 2.0*double_Pi*(frequency/sampleRate);
	float cos_w0 = cos(w0);
//	float sin_w0 = sin(w0);
	
	float alpha = (2.0+cos_w0) - sqrt((2.0+cos_w0)*(2.0+cos_w0) - 1.0);
	b0 = alpha - 1.0;
	a1 = -alpha;
	
//	HP:
//	recursion: tmp = (p-1)*in - p*tmp with output = tmp
//	coefficient: p = (2+cos(x)) - sqrt((2+cos(x))^2 - 1) with x = 2*pi*cutoff/samplerate
//	coeficient approximation: p = (2*cutoff/samplerate)^2}
}