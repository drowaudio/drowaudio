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
      spectroscope (11),
      sonogram (11)
{
    addAndMakeVisible (&audioOscilloscope);
    addAndMakeVisible (&spectroscope);
    addAndMakeVisible (&sonogram);
    
    renderThread.addTimeSliceClient (&spectroscope);
    renderThread.addTimeSliceClient (&sonogram);
    renderThread.startThread (3);
}

FFTDemo::~FFTDemo()
{
    renderThread.removeTimeSliceClient (&spectroscope);
    renderThread.removeTimeSliceClient (&sonogram);
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
    const int ch = (h - (4 * m)) / 3;
    
    audioOscilloscope.setBounds (m, m, w - (2 * m), ch);
    spectroscope.setBounds (m, ch + (2 * m), w - (2 * m), ch);
    sonogram.setBounds (m, (2 * ch) + (3 * m), w - (2 * m), ch);
}

//==============================================================================
void FFTDemo::processBlock (const float* inputChannelData, int numSamples)
{
    if (inputChannelData != nullptr)
    {
        audioOscilloscope.processBlock (inputChannelData, numSamples);
        spectroscope.copySamples (inputChannelData, numSamples);
        sonogram.copySamples (inputChannelData, numSamples);
    }
}