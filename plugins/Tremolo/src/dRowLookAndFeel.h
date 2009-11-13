/*
 *  dRowAudioLookAndFeel.h
 *  LookAndFeel_Test
 *
 *  Created by David Rowland on 27/12/2008.
 *  Copyright 2008 UWE. All rights reserved.
 *
 */

#ifndef _dRowAudioLookAndFeel_H_
#define _dRowAudioLookAndFeel_H_

#include "includes.h"

class dRowAudioLookAndFeel : public LookAndFeel
{
public:
//	dRowAudioLookAndFeel();
//	
//	~dRowAudioLookAndFeel();
		
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
							   const float x,
							   const float y,
							   const float width,
							   const float height);
};

#endif