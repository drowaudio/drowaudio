/*
  ==============================================================================

    dRowAudio_SampleRateConverter.h
    Created: 6 Feb 2011 1:24:29am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_SAMPLERATECONVERTER_H_3ECB8901__
#define __DROWAUDIO_SAMPLERATECONVERTER_H_3ECB8901__

#include "../JuceLibraryCode/JuceHeader.h"


class SampleRateConverter
{
public:
	SampleRateConverter();
	
	~SampleRateConverter();
	
	void setSampleRates(double sourceSampleRate, double destSampleRate);
		
private:
	
	ResamplingAudioSource *resampler;
	float resamplingRatio;
};



#endif  // __DROWAUDIO_SAMPLERATECONVERTER_H_3ECB8901__
