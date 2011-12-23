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

//==============================================================================
class FFTDemo : public Component,
                public AudioIODeviceCallback
{
public:
 	//==============================================================================
    FFTDemo();
    
    ~FFTDemo();
    
    void paint (Graphics& g);
    
    void resized();
    
	//==============================================================================
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    
private:
	//==============================================================================
    TimeSliceThread renderThread;
    AudioOscilloscope audioOscilloscope;
    Spectroscope spectroscope;
    
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTDemo);
};

#endif __DROWAUDIODEMO_FFTDEMO_H__