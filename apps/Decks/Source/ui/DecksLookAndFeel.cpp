/*
 *  DecksLookAndFeel.cpp
 *  Decks
 *
 *  Created by David Rowland on 04/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "DecksLookAndFeel.h"

juce_ImplementSingleton(DecksLookAndFeel)

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

//==============================================================================
DecksLookAndFeel::DecksLookAndFeel()
{
	for (int i = 0; i < noCustomColours; i++) {
		decksColours.add(Colour());
	}
	
	decksColours.getReference(backgroundColour) = Colours::grey;
	decksColours.getReference(panelColour) = Colour::greyLevel(0.2);
	decksColours.getReference(panelLineColour) = Colour::greyLevel(0.6);
	decksColours.getReference(meterLineColour) = Colour::greyLevel(0.5);
}

//==============================================================================
int DecksLookAndFeel::getSliderThumbRadius (Slider& slider)
{
    return jmin (5,
                 slider.getHeight() / 2,
                 slider.getWidth() / 2) + 2;
}

void DecksLookAndFeel::drawRotarySlider (Graphics& g,
										 int x, int y,
										 int width, int height,
										 float sliderPos,
										 const float rotaryStartAngle,
										 const float rotaryEndAngle,
										 Slider& slider)
{
    float radius = jmin (width / 2, height / 2) - 2.0f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();
	
	Colour baseColour;
	Colour outlineColour;
	if (slider.isEnabled()) {
		baseColour = (slider.findColour (Slider::rotarySliderFillColourId).withMultipliedBrightness(isMouseOver ? 1.2f : 1.0f));
		outlineColour = (slider.findColour (Slider::rotarySliderOutlineColourId));//.withAlpha (isMouseOver ? 1.0f : 0.7f));
	}
	else {
		baseColour = (slider.findColour (Slider::rotarySliderFillColourId).withAlpha (0.3f));
		outlineColour = (slider.findColour (Slider::rotarySliderOutlineColourId)).withAlpha (0.3f);
	}
	
	// tapered sides
	ColourGradient g1(baseColour.withBrightness(0.05),
					  centreX,
					  height,
					  baseColour.withBrightness(0.6),
					  centreX,
					  y,//centreY,
					  false);
	g.setGradientFill(g1);
	Path p2;
//	p2.addEllipse(-0.465f * rw, -0.465f * rw, 0.93*rw, 0.93*rw);
	p2.addEllipse (-0.5f * rw, -0.5f * rw, rw, rw);
	g.fillPath(p2, AffineTransform::translation (centreX, centreY));

	// top
	const float topRadius = 0.8*rw;
	g.setColour(baseColour);
	Path p3;
	p3.addEllipse(-topRadius*0.5, -topRadius*0.5, topRadius, topRadius);
//	p3.addEllipse(-0.4f * rw, -0.4f * rw, 0.8*rw, 0.8*rw);
	g.fillPath(p3, AffineTransform::identity.translated (centreX, centreY-(0.1*radius)));
	
	// outline
	Path p;
	p.addEllipse (-0.5f * rw, -0.5f * rw, rw, rw);
	PathStrokeType (rw * 0.04f).createStrokedPath (p, p);
	g.setColour(outlineColour);
	g.fillPath (p, AffineTransform::translation (centreX, centreY));
	
	// needle
	g.setColour(outlineColour);
	Path p4;
	p4.startNewSubPath(0.0f, 0.0f);
	p4.lineTo( 0.0f, -radius);
	g.strokePath(p4, PathStrokeType (2.0000f, PathStrokeType::mitered, PathStrokeType::rounded), AffineTransform::rotation (angle).translated (centreX, centreY));
}

void DecksLookAndFeel::drawLinearSliderThumb (Graphics& g,
											  int x, int y,
											  int width, int height,
											  float sliderPos,
											  float minSliderPos,
											  float maxSliderPos,
											  const Slider::SliderStyle style,
											  Slider& slider)
{
    const float sliderRadius = (float) jmin (10,
											 slider.getHeight() / 2,
											 slider.getWidth() / 2) + 2;//(getSliderThumbRadius (slider) - 2);
	
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

void DecksLookAndFeel::drawFaderKnob(Graphics& g,
									 const Slider::SliderStyle style,
									 const float x,
									 const float y,
									 const float width,
									 const float height)
{
	float cy = y+(height/2);
	float cx = x+(width/2);
	float stripWidth;
	float kx = x, ky = y;
	
	if (style == Slider::LinearVertical)
	{
		stripWidth = height/4;
	}
	else if (style == Slider::LinearHorizontal)
	{
		stripWidth = width/5;
	}
	
	if (style == Slider::LinearVertical)
	{
		Colour bgColour(Colour::greyLevel(0.1));
	
		// main knob face
		
		ColourGradient g1(bgColour.brighter(0.5f),
						  x,
						  y,
						  Colours::black,
						  x,
						  y+height,
						  false);
		g1.addColour(0.15, bgColour);
		g1.addColour(0.85, bgColour);
//		g1.addColour(1.0, Colours::black);
		g.setGradientFill(g1);// setColour(Colours::black);
		g.fillRect (x, y, width, height);
		
	
		// middle line
		
		ColourGradient g2(Colour::greyLevel(0.4),
						  x,
						  cy-stripWidth/2,
						  Colours::white,
						  x,
						  cy+stripWidth/2,
						  false);
		g.setGradientFill(g2);
		if (stripWidth < 2) {
			g.setColour(Colours::white);
		}
//		g.setColour (Colours::white);
		g.fillRect ((int)kx,
					(int)(cy-stripWidth/2),
					(int)width,
					(int)stripWidth);	
	}
	else if (style == Slider::LinearHorizontal)
	{
		// main knob face
		Colour bgColour(Colours::black);
		
		// knob outline
		ColourGradient g1 (bgColour.brighter(0.5f),
						   x, y,
						   bgColour,
						   x, y+2,
						   false);
		g.setGradientFill (g1);
		g.fillRect (x, y, width, height);
		
		// middle line
//		ColourGradient middleLineGradient (Colours::black,
//										   x+width, y,
//										   Colours::white,
//										   x, y+height,
//										   false);
//		g.setGradientFill (middleLineGradient);
		g.setColour(Colours::white);
		g.fillRect (cx-stripWidth/2, ky,
					stripWidth, height);
	}
}

//==============================================================================
void DecksLookAndFeel::drawButtonBackground (Graphics& g,
											 Button& button,
											 const Colour& backgroundColour,
											 bool isMouseOverButton,
											 bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();
	
    const float outlineThickness = 1.2f;//button.isEnabled() ? ((isButtonDown || isMouseOverButton) ? 1.2f : 0.7f) : 0.4f;
    const float halfThickness = outlineThickness * 0.5f;
	
    const float indentL = button.isConnectedOnLeft()   ? 0.1f : halfThickness;
    const float indentR = button.isConnectedOnRight()  ? 0.1f : halfThickness;
    const float indentT = button.isConnectedOnTop()    ? 0.1f : halfThickness;
    const float indentB = button.isConnectedOnBottom() ? 0.1f : halfThickness;
	
    const Colour baseColour (createBaseColour (backgroundColour,
                                               button.hasKeyboardFocus (true),
                                               isMouseOverButton, isButtonDown)
							 .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
	
//    if (width == height)
//		drawGlassLozenge (g,
//						  indentL,
//						  indentT,
//						  width - indentL - indentR,
//						  height - indentT - indentB,
//						  baseColour, outlineThickness, -1.0f,
//						  button.isConnectedOnLeft(),
//						  button.isConnectedOnRight(),
//						  button.isConnectedOnTop(),
//						  button.isConnectedOnBottom());
    if (width == height)
		drawSquareButton (g,
						  indentL,
						  indentT,
						  width - indentL - indentR,
						  height - indentT - indentB,
						  baseColour, outlineThickness, width*0.5);
	else
		drawSquareButton (g,
						  indentL,
						  indentT,
						  width - indentL - indentR,
						  height - indentT - indentB,
						  baseColour, outlineThickness, 5);
}

void DecksLookAndFeel::drawSquareButton (Graphics& g,
										const float x, const float y,
										const float width, const float height,
										const Colour& colour,
										const float outlineThickness,
										const float cornerSize) throw()
{
	g.setColour(colour);
	g.fillRoundedRectangle(x, y, width, height, cornerSize);

	int cx = width/2;
	int highlightThickness = outlineThickness;//2;
//	ColourGradient grd1 (Colours::white.withAlpha(0.2f),
//						 cx, y,
//						 Colours::black.withAlpha(0.2f),
//						 cx, height,
//						 false);
	ColourGradient grd1 (colour.brighter(),
						 cx, y,
						 colour.darker(),
						 cx, height,
						 false);
	g.setGradientFill(grd1);
	g.drawRoundedRectangle(x+highlightThickness/2, y+highlightThickness/2, width-highlightThickness, height-highlightThickness, cornerSize, highlightThickness);
}

//==============================================================================
void DecksLookAndFeel::drawDocumentWindowTitleBar (DocumentWindow& window,
												   Graphics& g, int w, int h,
												   int titleSpaceX, int titleSpaceW,
												   const Image* icon,
												   bool drawTitleTextOnLeft)
{
    const bool isActive = window.isActiveWindow();
	
//    g.setGradientFill (ColourGradient (window.getBackgroundColour(),
//                                       0.0f, 0.0f,
//                                       window.getBackgroundColour().contrasting (isActive ? 0.15f : 0.05f),
//                                       0.0f, (float) h, false));
//    g.fillAll();
	
    Font font (h * 0.65f, Font::bold);
    g.setFont (font);
	
    int textW = font.getStringWidth (window.getName());
    int iconW = 0;
    int iconH = 0;
	
    if (icon != 0)
    {
        iconH = (int) font.getHeight();
        iconW = icon->getWidth() * iconH / icon->getHeight() + 4;
    }
	
    textW = jmin (titleSpaceW, textW + iconW);
    int textX = drawTitleTextOnLeft ? titleSpaceX
	: jmax (titleSpaceX, (w - textW) / 2);
	
    if (textX + textW > titleSpaceX + titleSpaceW)
        textX = titleSpaceX + titleSpaceW - textW;
	
    if (icon != 0)
    {
        g.setOpacity (isActive ? 1.0f : 0.6f);
        g.drawImageWithin (*icon, textX, (h - iconH) / 2, iconW, iconH,
                           RectanglePlacement::centred, false);
        textX += iconW;
        textW -= iconW;
    }
	
    if (window.isColourSpecified (DocumentWindow::textColourId) || isColourSpecified (DocumentWindow::textColourId))
        g.setColour (findColour (DocumentWindow::textColourId));
    else
        g.setColour (window.getBackgroundColour().contrasting (isActive ? 0.7f : 0.4f));
	
    g.drawText (window.getName(), textX, 0, textW, h, Justification::centredLeft, true);
}

//==============================================================================
