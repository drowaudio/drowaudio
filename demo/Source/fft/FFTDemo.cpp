/*
  ==============================================================================

  This file is part of the dRowAudio JUCE module
  Copyright 2004-13 by dRowAudio.

  ------------------------------------------------------------------------------

  dRowAudio is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ==============================================================================
*/

#include "FFTDemo.h"

FFTDemo::FFTDemo() :
    renderThread ("FFT Render Thread"),
    spectroscope (12),
    sonogram (12)
{
    pitchDetector.setSampleRate (44100.0);

    logSpectroscopeButton.setButtonText (TRANS ("Log Frequency Scale"));
    logSpectroscopeButton.setClickingTogglesState (true);
    logSpectroscopeButton.addListener (this);

    logSonogramButton.setButtonText (TRANS ("Log Frequency Scale"));
    logSonogramButton.setClickingTogglesState (true);
    logSonogramButton.addListener (this);

    sonogramSpeedSlider.setRange (1.0, 10.0, 1.0);
    sonogramSpeedSlider.setValue (sonogram.getBlockWidth());
    sonogramSpeedSlider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    sonogramSpeedSlider.addListener (this);

    renderThread.addTimeSliceClient (&spectroscope);
    renderThread.addTimeSliceClient (&sonogram);
    renderThread.startThread();

    addAndMakeVisible (&audioOscilloscope);
    addAndMakeVisible (&spectroscope);
    addAndMakeVisible (&pitchDetector);
    addAndMakeVisible (&sonogram);
    addAndMakeVisible (&logSpectroscopeButton);
    addAndMakeVisible (&logSonogramButton);
    addAndMakeVisible (&sonogramSpeedSlider);
}

FFTDemo::~FFTDemo()
{
    renderThread.removeTimeSliceClient (&spectroscope);
    renderThread.removeTimeSliceClient (&sonogram);
    renderThread.stopThread (500);
}

//==============================================================================
void FFTDemo::visibilityChanged()
{
    isCurrentlyShowing = isShowing();
}

void FFTDemo::resized()
{
    const int h = getHeight();
    const int w = getWidth();
    const int m = 5;
    const int ch = (h - (5 * m)) / 4;

    audioOscilloscope.setBounds (m, m, w - (2 * m), ch);
    spectroscope.setBounds (m, ch + (2 * m), w - (2 * m), ch);
    pitchDetector.setBounds (spectroscope.getBounds());
    sonogram.setBounds (m, (2 * ch) + (3 * m), w - (2 * m), (2 * ch) + m);

    logSpectroscopeButton.setBounds (spectroscope.getX(), spectroscope.getY(), 150, 18);
    logSonogramButton.setBounds (sonogram.getX(), sonogram.getY(), 150, 18);

    sonogramSpeedSlider.setBounds (logSonogramButton.getRight() + m, logSonogramButton.getY(), 100, 18);
}

void FFTDemo::buttonClicked (juce::Button* button)
{
    if (button == &logSpectroscopeButton)
    {
        spectroscope.setLogFrequencyDisplay (logSpectroscopeButton.getToggleState());
        pitchDetector.setLogFrequencyDisplay (logSpectroscopeButton.getToggleState());
    }
    else if (button == &logSonogramButton)
    {
        sonogram.setLogFrequencyDisplay (logSonogramButton.getToggleState());
    }
}

void FFTDemo::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &sonogramSpeedSlider)
        sonogram.setBlockWidth ((int) sonogramSpeedSlider.getValue());
}

//==============================================================================
void FFTDemo::setSampleRate (const double sampleRate)
{
    pitchDetector.setSampleRate (sampleRate);
}

void FFTDemo::processBlock (const float* inputChannelData, const int numSamples)
{
    if (inputChannelData != nullptr)
    {
        audioOscilloscope.processBlock (inputChannelData, numSamples);
        pitchDetector.processBlock (inputChannelData, numSamples);
        spectroscope.copySamples (inputChannelData, numSamples);
        sonogram.copySamples (inputChannelData, numSamples);
    }
}
