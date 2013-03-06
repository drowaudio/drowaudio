/*
  ==============================================================================

  This file is part of the dRowAudio JUCE module
  Copyright 2004-13 by dRowAudio.

  ------------------------------------------------------------------------------

  dRowAudio is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
  SOFTWARE.

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
        resetPoints();
        isInitialised = true;
    }
    
    background = Image (Image::RGB, jmax (1, w), jmax (1, h), false);
    Graphics g (background);
    g.fillAll (Colours::black);
    
    g.setColour (Colour::greyLevel (0.25f));
    const float xScale = w / 10.0f;
    const float yScale = h / 10.0f;
    for (int i = 1; i < 10; ++i)
    {
        g.drawHorizontalLine ((int) (i * yScale), 0.0f, (float) w);
        g.drawVerticalLine ((int) (i * xScale), 0.0f, (float) h);
    }
    g.drawLine (0.0f, (float) h, (float) w, 0.0f);
    
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

void DistortionComponent::componentMovedOrResized (Component& component, bool /*wasMoved*/, bool /*wasResized*/)
{
    if (&component == curvePoints[0] || &component == curvePoints[1])
    {
        float x1 = (curvePoints[0]->getX() + (0.5f * curvePoints[0]->getWidth())) / (float) getWidth();
        float y1 = ((getHeight() - curvePoints[0]->getY()) - (0.5f * curvePoints[0]->getHeight())) / (float) getHeight();

        float x2 = (curvePoints[1]->getX() + (0.5f * curvePoints[1]->getWidth())) / (float) getWidth();
        float y2 = ((getHeight() - curvePoints[1]->getY()) - (0.5f * curvePoints[1]->getHeight())) / (float) getHeight();
        
        refillBuffer (x1, y1, x2, y2);
    }
}

//==============================================================================
void DistortionComponent::refreshPath()
{
    const int bufferSize = buffer.getSize();
    const int w = getWidth();
    const int h = getHeight();
    
    const float xScale = (float) w / (float) bufferSize;
    const float yScale = (float) h;
    
    path.clear();
    path.startNewSubPath (0.0f, (float) h);
    
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

void DistortionComponent::resetBuffer()
{
    float* bufferData = buffer.getData();
    const int bufferSize = buffer.getSize();
    const float bufferScale = 1.0f / bufferSize;
    
	for (int i = 0; i < bufferSize; ++i)
        bufferData[i] = bufferScale * i;
    
    resetPoints();
    buffer.updateListeners();
}

void DistortionComponent::resetPoints()
{
    const int w = getWidth();
    const int h = getHeight();

    const int bufferSize = buffer.getSize();
    const float* bufferData = buffer.getData();
    
    float x1 = w * 0.25f;
    float y1 = h * linearInterpolate (bufferData, bufferSize, bufferSize * 0.75f);
    
    float x2 = w * 0.75f;
    float y2 = h * linearInterpolate (bufferData, bufferSize, bufferSize * 0.25f);
    
    curvePoints[0]->setBounds ((int) (x1 - 5), (int) (y1 - 5), 10, 10);
    curvePoints[1]->setBounds ((int) (x2 - 5), (int) (y2 - 5), 10, 10);    
}