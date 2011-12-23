/*
  ==============================================================================

    FeedbackMatrixBuffer.h
    Created: 14 Dec 2011 10:25:48am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __FEEDBACKMATRIXBUFFER_H_F75F8394__
#define __FEEDBACKMATRIXBUFFER_H_F75F8394__

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class FeedbackMatrixBuffer
{
public:
    //==============================================================================
    FeedbackMatrixBuffer();
    
    ~FeedbackMatrixBuffer();
    
    //==============================================================================
    void setSampleRate (double newSampleRate);

    void rescaleDelayTimes (double ratio);

    void rescaleGains (double ratio);

    float processSingleSample (float newSample) noexcept;
	
	void processSamples(float* const samples,
						const int numSamples) noexcept;
    
private:
    //==============================================================================
//    class DelayBlock
//    {
//    public:
//        DelayBlock();
//
//        inline void setSize (int size);
//
//        inline void setGain (float newGain);
//        
//        inline float process (const float input);
//        
//    private:
//        float gain, filterCf;
//        
//        IIRFilter filter;
//        HeapBlock<float> buffer;
//        int bufferSize, bufferIndex;
//    };
    
    //==============================================================================
    double sampleRate;
    
    enum { numPaths = 7 };
    
    //DelayBlock paths[numPaths];
    HeapBlock<float> buffers[numPaths];
    int bufferSize, bufferIndex;
    int delays[numPaths];
    float gains[numPaths];
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FeedbackMatrixBuffer);    
};

#endif  // __FEEDBACKMATRIXBUFFER_H_F75F8394__
