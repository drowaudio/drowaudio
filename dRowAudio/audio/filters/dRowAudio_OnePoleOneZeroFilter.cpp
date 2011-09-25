/*
 *  dRowAudio_OnePoleOneZeroFilter.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 16/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "../../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_OnePoleOneZeroFilter.h"

OnePoleOneZeroFilter::OnePoleOneZeroFilter() throw()
	:	x1(0.0), y1(0.0), a0(1.0), a1(0.0), b1(0.0)
{
}

OnePoleOneZeroFilter::~OnePoleOneZeroFilter() throw()
{
}

void OnePoleOneZeroFilter::processSamples(float* const pfSamples,
										  const int numSamples) throw()
{
	// make sure sample values are locked
	lock.enter();
	
	for (int i = 0; i < numSamples; ++i)
	{
		y1 = (pfSamples[i] * a0) + (x1 * a1) + (a1 * y1);
		x1 = pfSamples[i];
		pfSamples[i] = y1;
	}
	
	lock.exit();
}


void OnePoleOneZeroFilter::makeLowPass(const double sampleRate,
									   const double frequency) throw()
{
	float w0 = 2.0 * sampleRate;
	float fCut = frequency * twoTimesPi;
	
	float norm = 1.0 / (fCut + w0);
	b1 = (w0 - fCut) * norm;
	a0 = a1 = fCut * norm;
}

void OnePoleOneZeroFilter::makeHighPass(const double sampleRate,
										const double frequency) throw()
{
	float w0 = 2.0 * sampleRate;
	float fCut = frequency  * twoTimesPi;
	
	float norm = 1.0 / (fCut + w0);
	a0 = w0 * norm;
	a1 = -a0;
	b1 = (w0 - fCut) * norm;
}

END_DROWAUDIO_NAMESPACE