/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#include "FFTDemo.h"

FFTDemo::FFTDemo()
    : renderThread ("FFT Render Thread"),
      spectroscope (11),
      sonogram (11),
      pitchTracker (14)
{
    addAndMakeVisible (&audioOscilloscope);
    addAndMakeVisible (&spectroscope);
    addAndMakeVisible (&sonogram);
    addAndMakeVisible (&pitchTracker);
    
    renderThread.addTimeSliceClient (&spectroscope);
    renderThread.addTimeSliceClient (&sonogram);
    renderThread.addTimeSliceClient (&pitchTracker);
    renderThread.startThread (3);
}

FFTDemo::~FFTDemo()
{
    renderThread.removeTimeSliceClient (&spectroscope);
    renderThread.removeTimeSliceClient (&sonogram);
    renderThread.removeTimeSliceClient (&pitchTracker);
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
    const int ch = (h - (5 * m)) / 4;
    
    audioOscilloscope.setBounds (m, m, w - (2 * m), ch);
    spectroscope.setBounds (m, ch + (2 * m), w - (2 * m), ch);
    sonogram.setBounds (m, (2 * ch) + (3 * m), w - (2 * m), ch);
    pitchTracker.setBounds (m, (3 * ch) + (4 * m), w - (2 * m), ch);
}

//==============================================================================
void FFTDemo::processBlock (const float* inputChannelData, int numSamples)
{
    if (inputChannelData != nullptr)
    {
        audioOscilloscope.processBlock (inputChannelData, numSamples);
        spectroscope.copySamples (inputChannelData, numSamples);
        sonogram.copySamples (inputChannelData, numSamples);
        pitchTracker.copySamples (inputChannelData, numSamples);
    }
}