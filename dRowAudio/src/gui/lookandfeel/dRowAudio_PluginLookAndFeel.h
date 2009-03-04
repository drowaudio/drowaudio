/*
 *  dRowLookAndFeel.h
 *
 *  Created by David Rowland on 23/01/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWLOOKANDFEEL_H_
#define _DROWLOOKANDFEEL_H_

#include <juce/juce.h>

class dRowLookAndFeel : public LookAndFeel
	{
	public:
		/**
		 Draws a shiny, rounded-top knob rotary slider.
		 */
		virtual void drawRotarySlider (Graphics& g,
									   int x, int y,
									   int width, int height,
									   float sliderPosProportional,
									   const float rotaryStartAngle,
									   const float rotaryEndAngle,
									   Slider& slider);
		
		/**
		 Draws a label.
		 If the label's background is not transparent then it will draw a 3D label.
		 */
		virtual void drawLabel (Graphics& g, Label& label);
	};

#endif