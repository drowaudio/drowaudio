/*
  ==============================================================================

    dRowAudio_SampleRateConverter.cpp
    Created: 6 Feb 2011 1:24:29am
    Author:  David Rowland

  ==============================================================================
*/

#include "dRowAudio_SampleRateConverter.h"

SampleRateConverter::SampleRateConverter()
:	resamplingRatio(1.0f)
{
	resampler = new ResamplingAudioSource(0, false);
}

SampleRateConverter::~SampleRateConverter()
{
}

void SampleRateConverter::setSampleRates(double sourceSampleRate, double destSampleRate)
{
	resamplingRatio = sourceSampleRate / destSampleRate;
}
