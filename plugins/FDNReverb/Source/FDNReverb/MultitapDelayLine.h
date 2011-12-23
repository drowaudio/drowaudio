/*
  ==============================================================================

    MultitapDelayLine.h
    Created: 14 Dec 2011 10:24:48am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MULTITAPDELAYLINE_H_EEA7713__
#define __MULTITAPDELAYLINE_H_EEA7713__

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class MultitapDelayLine
{
public:
    //==============================================================================
    struct Tap
    {
        int delaySamples, originalDelaySamples;
        float gain, originalGain;
        float feedback, originalFeedback;
    };

    //==============================================================================
    MultitapDelayLine();
    
    ~MultitapDelayLine();
    
    void addTap (double tapDelayMs, float outputGain = 0.5f, float feedbackGain = 0.5f);
    
    void setBufferSize (int numSamples);
    
    void setSampleRate (double newSampleRate);
    
    void rescaleTapPositions (double ratio);

    void rescaleTapFeedbacks (double ratio);

    void rescaleTapGains (double ratio);

    /// Processes a single sample returning a new sample with summed delays.
	float processSingleSample(float newSample) noexcept;
	
	/// Processes a number of samples in one go.
	void processSamples(float* const samples,
						const int numSamples) noexcept;
    
private:
    //==============================================================================
    double sampleRate;
    Array<Tap> taps;
    
    HeapBlock<float> buffer;
    int bufferSize, bufferWritePos;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultitapDelayLine);
};

#endif  // __MULTITAPDELAYLINE_H_EEA7713__
