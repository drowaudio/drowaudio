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

#ifndef DROWAUDIODEMO_FFTDEMO_H
#define DROWAUDIODEMO_FFTDEMO_H

#include "PitchDetectorComponent.h"

//==============================================================================
class FFTDemo : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener
{
public:
    FFTDemo();
    ~FFTDemo();

    //==============================================================================
    void setSampleRate (double sampleRate);
    void processBlock (const float* inputChannelData, int numSamples);

    //==============================================================================
    void resized() override;
    void buttonClicked (juce::Button* button) override;
    void sliderValueChanged (juce::Slider* slider) override;
    
    void visibilityChanged() override;
    bool isCurrentlyShowing = false;
private:
    //==============================================================================
    juce::TimeSliceThread renderThread;
    AudioOscilloscope audioOscilloscope;
    Spectroscope spectroscope;
    PitchDetectorComponent pitchDetector;
    Sonogram sonogram;

    juce::ToggleButton logSpectroscopeButton, logSonogramButton;
    juce::Slider sonogramSpeedSlider;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTDemo)
};

#endif //DROWAUDIODEMO_FFTDEMO_H
