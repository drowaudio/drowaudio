/*
 *  FFTDemo.h
 *  dRowAudio Demo
 *
 *  Created by David Rowland on 21/12/2011.
 *  Copyright 2011 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIODEMO_FFTDEMO_H__
#define __DROWAUDIODEMO_FFTDEMO_H__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "Spectroscope.h"
#include "Sonogram.h"

//==============================================================================
class FFTDemo : public Component
{
public:
 	//==============================================================================
    FFTDemo();
    
    ~FFTDemo();
    
    void paint (Graphics& g);
    
    void resized();
    
	//==============================================================================
    void processBlock (const float* inputChannelData, int numSamples);
    
private:
	//==============================================================================
    TimeSliceThread renderThread;
    AudioOscilloscope audioOscilloscope;
    Spectroscope spectroscope;
    Sonogram sonogram;
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTDemo);
};

#endif __DROWAUDIODEMO_FFTDEMO_H__