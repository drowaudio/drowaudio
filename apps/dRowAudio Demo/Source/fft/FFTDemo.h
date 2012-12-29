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

#ifndef __DROWAUDIODEMO_FFTDEMO_H__
#define __DROWAUDIODEMO_FFTDEMO_H__

#include "../DemoHeader.h"
#include "PitchDetectorComponent.h"

//==============================================================================
class FFTDemo : public Component,
                public Button::Listener,
                public Slider::Listener
{
public:
 	//==============================================================================
    FFTDemo();
    
    ~FFTDemo();
    
    void paint (Graphics& g);
    
    void resized();
    
    void buttonClicked (Button* button);
    
    void sliderValueChanged (Slider* slider);
    
	//==============================================================================
    void setSampleRate (double sampleRate);
    
    void processBlock (const float* inputChannelData, int numSamples);
    
private:
	//==============================================================================
    TimeSliceThread renderThread;
    AudioOscilloscope audioOscilloscope;
    Spectroscope spectroscope;
    PitchDetectorComponent pitchDetector;
    Sonogram sonogram;

	//==============================================================================
    ToggleButton logSpectroscopeButton, logSonogramButton;
    Slider sonogramSpeedSlider;
    
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTDemo);
};

#endif //__DROWAUDIODEMO_FFTDEMO_H__