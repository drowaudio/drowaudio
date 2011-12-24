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

#ifndef _GUITARAMPKNOBLOOKANDFEEL_H_
#define _GUITARAMPKNOBLOOKANDFEEL_H_

#include <juce/juce.h>

class MyLookAndFeel : public LookAndFeel
{
public:
		
	virtual void drawRotarySlider (Graphics& g,
                                   int x, int y,
                                   int width, int height,
                                   float sliderPosProportional,
                                   const float rotaryStartAngle,
                                   const float rotaryEndAngle,
                                   Slider& slider);
	
	virtual void drawLinearSliderThumb (Graphics& g,
									    int x, int y,
									    int width, int height,
									    float sliderPos,
									    float minSliderPos,
									    float maxSliderPos,
									    const Slider::SliderStyle style,
										Slider& slider);
	
	virtual void drawFaderKnob(Graphics& g,
							   const Slider::SliderStyle style,
							   const float x,
							   const float y,
							   const float width,
							   const float height);
};

#endif	//_GUITARAMPKNOBLOOKANDFEEL_H_