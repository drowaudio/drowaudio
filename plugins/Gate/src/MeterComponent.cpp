/*
 *  MeterComponent.cpp
 *  BaseMeteredProject
 *
 *  Created by David Rowland on 03/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "MeterComponent.h"

MeterComponent::MeterComponent()
	:	currentBarLevel(0.0f),
		currentLineLevel(0.0f),
		lineCounter(0),
		dropLineLevel(false)
{
}

MeterComponent::~MeterComponent()
{
	deleteAllChildren();
}

//========================================
void MeterComponent::paint(Graphics &g)
{
	const int clippingLightHeight = 0;
	const int meterHeight = getHeight() - clippingLightHeight;
	
	// draw background
	g.setColour(Colour::greyLevel(0.6f));
	g.fillRoundedRectangle(0, 0, getWidth(), meterHeight, 5);
	
	// draw meter level
	float BarToDraw = pow(currentBarLevel, 0.25);
	ColourGradient meterGradientRaw (Colours::red,
									 0, 0,
									 Colours::green,
									 0, meterHeight*0.5f,
									 false);
	meterGradientRaw.addColour(0.35f, Colours::yellow);
	GradientBrush meterGradientBrush(meterGradientRaw);
	g.setBrush(&meterGradientBrush);
	g.fillRect(0, (uint32)(meterHeight*(1.0f-BarToDraw)),
			   getWidth(), (uint32)(meterHeight*BarToDraw));
	
	ColourGradient highlight (Colour(0x00000000),
						      0, 0,
							  Colour(0x00000000),
							  getWidth(), 0,
							  false);
	highlight.addColour(0.5f, Colours::white.withAlpha(0.5f));
	GradientBrush highlightBrush(highlight);
	g.setBrush(&highlightBrush);
	g.fillRect(0, (uint32)(meterHeight*(1-BarToDraw)),
			   getWidth(), (uint32)(meterHeight*BarToDraw));
	
	
	// draw Line level
	float LineToDraw = pow(currentLineLevel, 0.25);
	g.setColour(Colours::white);
	g.drawLine(0, (uint32)(meterHeight*(1-LineToDraw)),
			   getWidth(), (uint32)(meterHeight*(1-LineToDraw)),
			   2);
	
	// draw clipping lights
//	if (clipped)
		
	
	// draw outine
	g.setColour(Colour::greyLevel(0.4f));
	g.drawRoundedRectangle(0, 0, getWidth(), meterHeight, 5, 1);
}

void MeterComponent::resized()
{
	
}
//========================================

void MeterComponent::setMeterLevel(const float newBarLevel, const float newLineLevel)
{
	prevBarLevel = currentBarLevel;
	currentBarLevel = fabsf(newBarLevel);
	
	// impliment slow decay
	if(currentBarLevel < prevBarLevel)
		currentBarLevel = 0.1f * currentBarLevel + 0.8f * prevBarLevel;
	
	// increase Line level if it is bigger
	if (newLineLevel > currentLineLevel)
	{
		currentLineLevel = fabsf(newLineLevel);
		dropLineLevel = false;
		lineCounter = 0;
	}
	
	// hold line level for 29/30 cycles
	if ( !(++lineCounter % 30) )
		dropLineLevel = true;	
	if (dropLineLevel) {
		currentLineLevel -= (0.2f*currentLineLevel);
		clipped = false;
	}
	
	// show the clipping lights if necessarry
	if (currentBarLevel > 1.0f
		|| currentLineLevel > 1.0f)
		clipped = true;
	
	// update the meter displays
	repaint();	
}