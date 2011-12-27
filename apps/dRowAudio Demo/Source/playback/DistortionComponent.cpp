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

#include "DistortionComponent.h"

DistortionComponent::DistortionComponent (Buffer& bufferToControl)
    : buffer (bufferToControl),
      isInitialised (false)
{
    for (int i = 0; i < 2; ++i)
    {
        curvePoints.add (new CurvePoint());
        curvePoints[i]->addComponentListener (this);
        addAndMakeVisible (curvePoints[i]);
    }
        
    buffer.addListener (this);
}

DistortionComponent::~DistortionComponent()
{
    for (int i = 0; i < 2; ++i)
        curvePoints[i]->removeComponentListener (this);
        
    buffer.removeListener (this);
}

void DistortionComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();

    if (! isInitialised && w > 0 && h > 0)
    {
        const int bufferSize = buffer.getSize();
        const float* bufferData = buffer.getData();
        
        float x1 = w * 0.25;
        float y1 = h * linearInterpolate (bufferData, bufferSize, bufferSize * 0.75);
        
        float x2 = w * 0.75;
        float y2 = h * linearInterpolate (bufferData, bufferSize, bufferSize * 0.25);
        
        curvePoints[0]->setBounds (x1 - 5, y1 - 5, 10, 10);
        curvePoints[1]->setBounds (x2 - 5, y2 - 5, 10, 10);
        
        isInitialised = true;
    }
    
    background = Image (Image::RGB, jmax (1, w), jmax (1, h), false);
    Graphics g (background);
    g.fillAll (Colours::black);
    
    g.setColour (Colour::greyLevel (0.25));
    const float xScale = w / 10.0f;
    const float yScale = h / 10.0f;
    for (int i = 1; i < 10; ++i)
    {
        g.drawHorizontalLine (i * yScale, 0, w);
        g.drawVerticalLine (i * xScale, 0, h);
    }
    g.drawLine (0, h, w, 0);
    
    refreshPath();
}

void DistortionComponent::paint (Graphics& g)
{
    g.drawImageAt (background, 0, 0);
    
    g.setColour (Colours::white);
    g.strokePath (path, PathStrokeType (2.0f));
}

void DistortionComponent::bufferChanged (Buffer* changedBuffer)
{
    if (changedBuffer == &buffer)
    {
        refreshPath();
    }
}

void DistortionComponent::componentMovedOrResized (Component& component, bool wasMoved, bool wasResized)
{
    if (&component == curvePoints[0] || &component == curvePoints[1])
    {
        float x1 = (curvePoints[0]->getX() + (0.5 * curvePoints[0]->getWidth())) / (float)getWidth();
        float y1 = ((getHeight() - curvePoints[0]->getY()) - (0.5 * curvePoints[0]->getHeight())) / (float)getHeight();

        float x2 = (curvePoints[1]->getX() + (0.5 * curvePoints[1]->getWidth())) / (float)getWidth();
        float y2 = ((getHeight() - curvePoints[1]->getY()) - (0.5 * curvePoints[1]->getHeight())) / (float)getHeight();
        
        refillBuffer (x1, y1, x2, y2);
    }
}

//==============================================================================
void DistortionComponent::refreshPath()
{
    const int bufferSize = buffer.getSize();
    const int w = getWidth();
    const int h = getHeight();
    
    const float xScale = w / (float)bufferSize;
    const float yScale = h;
    
    path.clear();
    path.startNewSubPath (0.0f, h);
    
    for (int i = 0; i < bufferSize; ++i)
    {
        path.lineTo (i * xScale, h - (buffer[i] * yScale));
    }
    
    repaint();
}

void DistortionComponent::refillBuffer (float x1, float y1, float x2, float y2)
{
    float* bufferData = buffer.getData();
    const int bufferSize = buffer.getSize();
	const float bufferScale = 1.0f / (float) bufferSize;

	for (int i = 0; i < bufferSize; ++i)
	{
		float x = jlimit (0.0f, 1.0f, i * bufferScale);
		bufferData[i] = BezierCurve::cubicBezierNearlyThroughTwoPoints (x,
                                                                        x1, y1,
                                                                        x2, y2);
	}
    
    buffer.updateListeners();
}
