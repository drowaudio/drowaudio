/*
 *  FFTDemo.cpp
 *  dRowAudio Demo
 *
 *  Created by David Rowland on 21/12/2011.
 *  Copyright 2011 dRowAudio. All rights reserved.
 *
 */

#include "FFTDemo.h"

FFTDemo::FFTDemo()
    : renderThread ("FFT Render Thread"),
      spectroscope (10)
{
    addAndMakeVisible (&audioOscilloscope);
    addAndMakeVisible (&spectroscope);
    
    renderThread.addTimeSliceClient (&spectroscope);
    renderThread.startThread (3);
}

FFTDemo::~FFTDemo()
{
    renderThread.removeTimeSliceClient (&spectroscope);
    renderThread.stopThread (500);
}

void FFTDemo::paint (Graphics& g)
{
    g.fillAll (Colours::orange);
}

void FFTDemo::resized()
{
    const int h = getHeight();
    const int w = getWidth();
    const int m = 5;
    const int ch = (h - (3 * m)) * 0.5;
    
    audioOscilloscope.setBounds (m, m, w - (2 * m), ch);
    spectroscope.setBounds (m, ch + (2 * m), w - (2 * m), ch);
}

//==============================================================================
void FFTDemo::audioDeviceIOCallback (const float** inputChannelData,
                                     int numInputChannels,
                                     float** outputChannelData,
                                     int numOutputChannels,
                                     int numSamples)
{
    if (outputChannelData[0] != nullptr)
    {
        audioOscilloscope.processBlock (outputChannelData[0], numSamples);
        spectroscope.copySamples (outputChannelData[0], numSamples);
    }
}

void FFTDemo::audioDeviceAboutToStart (AudioIODevice* device)
{
    DBG ("FFTDemo::audioDeviceAboutToStart");
}

void FFTDemo::audioDeviceStopped()
{
    DBG ("FFTDemo::audioDeviceStopped");
}
