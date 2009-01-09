/*
 *  dRowAudioLookAndFeel.cpp
 *  LookAndFeel_Test
 *
 *  Created by David Rowland on 27/12/2008.
 *  Copyright 2008 UWE. All rights reserved.
 *
 */

#include "dRowLookAndFeel.h"

//dRowAudioLookAndFeel::dRowAudioLookAndFeel() : LookAndFeel()
//{
//}
//
//dRowAudioLookAndFeel::~dRowAudioLookAndFeel()
//{
//}

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
void dRowAudioLookAndFeel::drawRotarySlider (Graphics& g,
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
		
	// draw large knob
    if (radius > 100.0f)
    {
        if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        else
            g.setColour (Colour (0x80808080));
		
        const float thickness = 0.7f;
		
        {
            Path filledArc;
            filledArc.addPieSegment (rx, ry, rw, rw,
									 rotaryStartAngle,
									 angle,
									 thickness);
			
            g.fillPath (filledArc);
        }
		
        if (thickness > 0)
        {
            const float innerRadius = radius * 0.2f;
            Path p;
            p.addTriangle (-innerRadius, 0.0f,
                           0.0f, -radius * thickness * 1.1f,
                           innerRadius, 0.0f);
			
            p.addEllipse (-innerRadius, -innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
			
            g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));
        }
		
        if (slider.isEnabled())
        {
            g.setColour (slider.findColour (Slider::rotarySliderOutlineColourId));
            Path outlineArc;
            outlineArc.addPieSegment (rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
            outlineArc.closeSubPath();
			
            g.strokePath (outlineArc, PathStrokeType (slider.isEnabled() ? (isMouseOver ? 2.0f : 1.2f) : 0.3f));
        }
    }
	// medium sized
	else if (radius > 10.0f)
	{
		if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.8f));
        else
            g.setColour (Colour (0x80808080));
		
		// draw background
		float rotaryAngle = rotaryEndAngle - rotaryStartAngle;
		float angleDiff = rotaryAngle/10;
		Path p2;
        p2.addLineSegment (0.0f, -radius * 1.0f, 0.0f, 0.0f, rw * 0.03f);
		g.setColour(slider.findColour (Slider::trackColourId));
        for(int i = 0; i < 11; i++)
			g.fillPath (p2, AffineTransform::rotation ((i * angleDiff)+rotaryStartAngle).translated (centreX, centreY));
		
		const float kw = 0.8f * rw;
		const float kx = rx + 0.2f * rw * 0.5f;
		const float ky = ry + 0.2f * rw * 0.5f;
		
		// dial top
		GradientBrush gradient_1 (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (1.0f).brighter(1.5f),
								  (float)centreX, (float)centreY,
								  slider.findColour (Slider::rotarySliderFillColourId).withAlpha (1.0f),
								  (float)kx, (float)ky,
								  true);
		g.setBrush (&gradient_1);
		g.fillEllipse (kx, ky, kw, kw);
		
		// dial rim
		GradientBrush gradient_2 (Colours::white,
								  kw, 0,
								  Colours::black,
								  0, kw,
								  false);
		g.setBrush (&gradient_2);
		g.drawEllipse (kx+(kw * 0.05f)/2, ky+(kw * 0.05f)/2, kw-(kw * 0.05f), kw-(kw * 0.05f), kw * 0.05f);
		
		// dial needle
		Path p;
        p.addLineSegment (0.0f, -kw*0.5f + kw*0.05f, 0.0f, 0.0f, rw * 0.05f);
		g.setColour(slider.findColour (Slider::trackColourId).withAlpha (isMouseOver ? 1.0f : 0.8f));
        g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));
	}
	// small sized
    else
    {
        if (slider.isEnabled())
            g.setColour (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (isMouseOver ? 1.0f : 0.7f));
        else
            g.setColour (Colour (0x80808080));
		
        Path p;
        p.addEllipse (-0.4f * rw, -0.4f * rw, rw * 0.8f, rw * 0.8f);
        PathStrokeType (rw * 0.1f).createStrokedPath (p, p);
		
        p.addLineSegment (0.0f, 0.0f, 0.0f, -radius, rw * 0.2f);
		
        g.fillPath (p, AffineTransform::rotation (angle).translated (centreX, centreY));
    }
}

void dRowAudioLookAndFeel::drawLinearSliderThumb (Graphics& g,
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
		
		g.setColour(Colours::black);
		if (style == Slider::LinearVertical)
		{
			drawFaderKnob(g,
						  kx - (width/2),
						  ky - sliderRadius*0.5f,
						  width,
						  sliderRadius);
		}
		else
//			g.fillRect(kx - sliderRadius*0.5f, ky - sliderRadius, sliderRadius, sliderRadius*2.0f);
			drawFaderKnob(g,
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

void dRowAudioLookAndFeel::drawFaderKnob(Graphics& g,
								  const float x,
								  const float y,
								  const float width,
								  const float height)
{
    // main knob face
	g.fillRect (x, y, width, height);
	
	// knob outline
    GradientBrush gradient_1 (Colour (0xff616161),
                              x, y+height,
                              Colours::white,
                              x+width, y,
                              false);
    g.setBrush (&gradient_1);
    g.drawRect (x, y, width, height, width*0.1f);
	
	// middle line
    GradientBrush gradient_2 (Colours::black,
                              x, y+height,
							  Colours::white,
                              x+width, y,
                              false);
    g.setBrush (&gradient_2);
    g.fillRect (x+(width / 2) - ((width * 0.3f) / 2),
				y+(height / 2) - ((height * 0.8f) / 2),
				(width) * 0.3f,
				(height) * 0.8f);
}
