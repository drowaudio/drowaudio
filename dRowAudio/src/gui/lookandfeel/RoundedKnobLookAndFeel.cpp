/*
 *  dRowLookAndFeel.cpp
 *  haydxn_tutorial
 *
 *  Created by David Rowland on 23/01/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "RoundedKnobLookAndFeel.h"

void dRowLookAndFeel::drawRotarySlider (Graphics& g,
									  int x, int y,
									  int width, int height,
									  float sliderPos,
									  const float rotaryStartAngle,
									  const float rotaryEndAngle,
									  Slider& slider)
{
    const float radius = jmin (width / 2, height / 2) - 2.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
	
	// round top knob
//	if (rotarySliderStyle == roundAmpKnob) 
//	{
		if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.8f));
        else
            g.setColour (Colour (0x80808080));
				
		//======================================================================
		// draw dial top
		GradientBrush bottomShaddowGradient ( slider.findColour(Slider::rotarySliderFillColourId).withBrightness(0.1f),
											  rx, ry+rw,
											  slider.findColour(Slider::rotarySliderFillColourId),
											  rx, (ry+rw)/2,
											  false);
		g.setBrush (&bottomShaddowGradient);
		g.fillEllipse (rx, ry, rw, rw);
		
		// draw rounding highlight
		GradientBrush highlight (Colour (0x91ffffff),
								  rx+(rw/2), ry+(rw * 0.15f),
								  Colour (0x00ffff),
								  rx+(rw/2), ry+(rw/2),
								  true);
		g.setBrush (&highlight);
		g.fillEllipse (rx, ry, rw, rw/2);
		
		
		// draw rim
		g.setColour (Colours::black);
		g.drawEllipse (rx, ry, rw, rw, 0.5f);
		
		// draw thumb
		const float adj = (radius * 0.7f) * cos(angle);
		const float opp = (radius * 0.7f) * sin(angle);
		const float posX = rx + (rw/2) + opp;
		const float posY = ry + (rw/2) - adj;
		
		const float thumbW = rw * 0.15f;
		const float thumbX = posX - (thumbW * 0.5f);
		const float thumbY = posY - (thumbW * 0.5f);
		
		GradientBrush thumbGradient ( (Colours::black),
									  //Colour().greyLevel(-0.2f*cos(0.5f*angle)),
									  thumbX, thumbY,
									  Colour().greyLevel(0.7f),
									  thumbX, thumbY+thumbW,
									  false);
		g.setBrush (&thumbGradient);
		g.fillEllipse (thumbX, thumbY, thumbW, thumbW);
		
		// draw thumb rim
		g.setColour (Colours::black);
		g.drawEllipse (thumbX, thumbY, thumbW, thumbW, thumbW * 0.02f);
		
		//======================================================================
//	}
}
