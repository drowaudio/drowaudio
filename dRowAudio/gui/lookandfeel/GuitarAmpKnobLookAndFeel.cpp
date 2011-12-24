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

#include "GuitarAmpKnobLookAndFeel.h"

// creates the colours for components
static const Colour createBaseColour (const Colour& buttonColour,
                                      const bool hasKeyboardFocus,
                                      const bool isMouseOverButton,
                                      const bool isButtonDown) throw()
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const Colour baseColour (buttonColour.withMultipliedSaturation (sat));
	
    if (isButtonDown)
        return baseColour.contrasting (0.2f);
    else if (isMouseOverButton)
        return baseColour.contrasting (0.1f);
	
    return baseColour;
}

//============================================================================
// Create graphical elements
//============================================================================
void MyLookAndFeel::drawRotarySlider (Graphics& g,
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
		
	// set colour
	if (slider.isEnabled())
		g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.8f));
	else
		g.setColour (Colour (0x80808080));
	
	// draw background
	float anglePerSpoke = (rotaryEndAngle - rotaryStartAngle)/10;
	Path dialSpokes;
	dialSpokes.addLineSegment (Line<float> (0.0f, -radius, 0.0f, 0.0f), rw * 0.03f);
	g.setColour(slider.findColour (Slider::trackColourId));
	for(int i = 0; i < 11; i++)
		g.fillPath (dialSpokes, AffineTransform::rotation ( (i * anglePerSpoke)+rotaryStartAngle ).translated (centreX, centreY));
	
	
		// parameters for inner knob
	const float kw = rw * 0.8f;
	const float kx = rx + ((0.2f * rw) * 0.5f);
	const float ky = ry + ((0.2f * rw) * 0.5f);
	
	// dial top
	ColourGradient dialTopGradient (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (1.0f).brighter(1.5f),
								   (float)centreX, (float)centreY,
								   slider.findColour (Slider::rotarySliderFillColourId).withAlpha (1.0f),
								   (float)kx, (float)ky,
								   true);
	g.setGradientFill(dialTopGradient);
	g.fillEllipse (kx, ky, kw, kw);
	
	// dial rim
	ColourGradient dialRimGradient (Colours::white,
								   kw, 0,
								   Colours::black,
								   0, kw,
								   false);
	g.setGradientFill (dialRimGradient);
	g.drawEllipse (kx+(kw * 0.05f)/2, ky+(kw * 0.05f)/2,
				   kw * 0.95f, kw * 0.95f,
				   kw * 0.05f);
	
	// dial needle
	Path needlePath;
	needlePath.addLineSegment (Line<float> (0.0f, -kw*0.5f + kw*0.05f, 0.0f, 0.0f), rw * 0.05f);
	g.setColour(slider.findColour (Slider::thumbColourId).withAlpha (isMouseOver ? 1.0f : 0.8f));
	g.fillPath (needlePath, AffineTransform::rotation (angle).translated (centreX, centreY));
}

void MyLookAndFeel::drawLinearSliderThumb (Graphics& g,
                                         int x, int y,
                                         int width, int height,
                                         float sliderPos,
                                         float minSliderPos,
                                         float maxSliderPos,
                                         const Slider::SliderStyle style,
                                         Slider& slider)
{
    const float sliderRadius = (float) (getSliderThumbRadius (slider) - 2);
	
    Colour knobColour (createBaseColour (slider.findColour (Slider::thumbColourId),
                                         slider.hasKeyboardFocus (false) && slider.isEnabled(),
                                         slider.isMouseOverOrDragging() && slider.isEnabled(),
                                         slider.isMouseButtonDown() && slider.isEnabled()));
	
    const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;
	
    if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
    {
        float kx, ky;
		
        if (style == Slider::LinearVertical)
        {
            kx = x + width * 0.5f;
            ky = sliderPos;
        }
        else
        {
            kx = sliderPos;
            ky = y + height * 0.5f;
        }
		
		if (style == Slider::LinearVertical)
		{
			drawFaderKnob(g,
						  style,
						  kx - (width/2),
						  ky - sliderRadius*0.5f,
						  width,
						  sliderRadius);
		}
		else
//			g.fillRect(kx - sliderRadius*0.5f, ky - sliderRadius, sliderRadius, sliderRadius*2.0f);
			drawFaderKnob(g,
						  style,
						  kx - sliderRadius*0.5f,
						  ky-(height/2),
						  sliderRadius,
						  height);
		
		
//        drawGlassSphere (g,
//                         kx - sliderRadius,
//                         ky - sliderRadius,
//                         sliderRadius * 2.0f,
//                         knobColour, outlineThickness);
    }
    else
    {
        if (style == Slider::ThreeValueVertical)
        {
            drawGlassSphere (g, x + width * 0.5f - sliderRadius,
                             sliderPos - sliderRadius,
                             sliderRadius * 2.0f,
                             knobColour, outlineThickness);
        }
        else if (style == Slider::ThreeValueHorizontal)
        {
            drawGlassSphere (g,sliderPos - sliderRadius,
                             y + height * 0.5f - sliderRadius,
                             sliderRadius * 2.0f,
                             knobColour, outlineThickness);
        }
		
        if (style == Slider::TwoValueVertical || style == Slider::ThreeValueVertical)
        {
            const float sr = jmin (sliderRadius, width * 0.4f);
			
            drawGlassPointer (g, jmax (0.0f, x + width * 0.5f - sliderRadius * 2.0f),
                              minSliderPos - sliderRadius,
                              sliderRadius * 2.0f, knobColour, outlineThickness, 1);
			
            drawGlassPointer (g, jmin (x + width - sliderRadius * 2.0f, x + width * 0.5f), maxSliderPos - sr,
                              sliderRadius * 2.0f, knobColour, outlineThickness, 3);
        }
        else if (style == Slider::TwoValueHorizontal || style == Slider::ThreeValueHorizontal)
        {
            const float sr = jmin (sliderRadius, height * 0.4f);
			
            drawGlassPointer (g, minSliderPos - sr,
                              jmax (0.0f, y + height * 0.5f - sliderRadius * 2.0f),
                              sliderRadius * 2.0f, knobColour, outlineThickness, 2);
			
            drawGlassPointer (g, maxSliderPos - sliderRadius,
                              jmin (y + height - sliderRadius * 2.0f, y + height * 0.5f),
                              sliderRadius * 2.0f, knobColour, outlineThickness, 4);
        }
    }
}

void MyLookAndFeel::drawFaderKnob(Graphics& g,
								  const Slider::SliderStyle style,
								  const float x,
								  const float y,
								  const float width,
								  const float height)
{
	if (style == Slider::LinearHorizontal)
	{
		// main knob face
		g.setColour(Colours::black);
		g.fillRect (x, y, width, height);
		
		// knob outline
		ColourGradient outlineGradient (Colour (0xff616161),
									   x, y+height,
									   Colours::white,
									   x+width, y,
									   false);
		g.setGradientFill(outlineGradient);
		g.drawRect (x, y, width, height, width*0.1f);
		
		// middle line
		ColourGradient middleLineGradient (Colours::black,
										  x, y+height,
										  Colours::white,
										  x+width, y,
										  false);
		g.setGradientFill (middleLineGradient);
		
		g.fillRect (x+(width / 2) - ((width * 0.3f) / 2),
					y+(height / 2) - ((height * 0.8f) / 2),
					(width) * 0.3f,
					(height) * 0.8f);	
	}
	else if (style == Slider::LinearVertical)
	{
		// main knob face
		g.setColour(Colours::black);
		g.fillRect (x, y, width, height);

		// knob outline
		ColourGradient outlineGradient (Colour (0xff616161),
									   x, y+height,
									   Colours::white,
									   x+width, y,
									   false);
		g.setGradientFill (outlineGradient);
		g.drawRect (x, y, width, height, height*0.1f);
		
		// middle line
		ColourGradient middleLineGradient (Colours::black,
										  x+width, y,
										  Colours::white,
										  x, y+height,
										  false);
		g.setGradientFill (middleLineGradient);
		g.fillRect (x+(width / 2) - ((width * 0.8f) / 2),
					y+(height / 2) - ((height * 0.3f) / 2),
					(width) * 0.8f,
					(height) * 0.3f);
	}
}
