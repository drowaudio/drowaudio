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

#ifndef _DROWLOOKANDFEEL_H_
#define _DROWLOOKANDFEEL_H_

#include "../../core/dRowAudio_StandardHeader.h"

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
	
	/**
		Draws a line that will look like it is inset in its background.
	 */
	static void drawInsetLine (Graphics& g,
							   const float startX,
							   const float startY,
							   const float endX,
							   const float endY,
							   const float lineThickness);
};

#endif