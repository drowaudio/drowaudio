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
dRowBufferView::dRowBufferView (float* buffer, int size)
{
	tremoloBufferSize = size;
	tremoloBuffer = buffer;
}

dRowBufferView::~dRowBufferView ()
{
	deleteAllChildren();
}

//==============================================================================
void dRowBufferView::resized ()
{
	// recalculate display scale
	float displayScaleX = (tremoloBufferSize+1)/(getWidth()-4.0f);
	float displayScaleY = (getHeight()-4.0f) * 0.7f;
	
	displayPath.clear();	
    displayPath.startNewSubPath (2.0f, 2+displayScaleY*tremoloBuffer[0]);
	for (uint32 i = 10; i < tremoloBufferSize; i+=10)
	{
		displayPath.lineTo((i/displayScaleX)+2, (displayScaleY*tremoloBuffer[i])+2);
	}
	displayPath.lineTo(getWidth()-2, (displayScaleY*tremoloBuffer[0])+2);
	displayPath.closeSubPath();
	
	repaint();
}

void dRowBufferView::paint (Graphics& g)
{
	g.setColour(Colour(0xA1AC0000));
	g.fillPath(displayPath);
	g.setColour(Colour(0xB1F10000));
	g.strokePath (displayPath, PathStrokeType (4.0000f));
}

//==============================================================================