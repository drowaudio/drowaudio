/*
 *  dRowBufferView.cpp
 *  haydxn_tutorial
 *
 *  Created by David Rowland on 27/12/2008.
 *  Copyright 2008 UWE. All rights reserved.
 *
 */

#include "dRowBufferView.h"
#include <cmath>

#define pi 3.14159265358979323846264338327950288

//==============================================================================
dRowBufferView::dRowBufferView (float* buffer, int size, int bufferOffset)
	:	tremoloBuffer(buffer),
		tremoloBufferSize(size)
		
{
//	tremoloBufferSize = size;
//	tremoloBuffer = buffer;
	fBufferOffset = bufferOffset;
}

dRowBufferView::~dRowBufferView ()
{
	deleteAllChildren();
}

//==============================================================================
void dRowBufferView::resized ()
{
	float height = getHeight();
	
	// recalculate display scale
	float displayScaleX = (tremoloBufferSize+1)/(getWidth()-4.0f);
	float displayScaleY = (height-4.0f);
	
	
	displayPath.clear();	
    displayPath.startNewSubPath (2.0f, height-(displayScaleY*tremoloBuffer[0])-2);
	for (uint32 i = 25; i < tremoloBufferSize; i+=25)
	{
		displayPath.lineTo((i/displayScaleX)+2, height-(displayScaleY*tremoloBuffer[i])-2);
	}
	displayPath.lineTo(getWidth()-2, height-(displayScaleY*tremoloBuffer[0])-2);
	displayPath.lineTo(getWidth()-2, height);
	displayPath.lineTo(2.0f, height);
	displayPath.closeSubPath();
	
	repaint();
}

void dRowBufferView::paint (Graphics& g)
{
	g.fillAll(Colour::greyLevel(0.4f));
	g.setColour(Colour(0xA100A0FF));
	g.fillPath(displayPath);
	g.setColour(Colour(0xB1002DFF).withAlpha(0.5f));
	g.strokePath (displayPath, PathStrokeType (4.0f));
}

//==============================================================================