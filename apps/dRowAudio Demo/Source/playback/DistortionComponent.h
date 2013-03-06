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

#ifndef __DROWAUDIO_DISTORTIONCOMPONENT_H__
#define __DROWAUDIO_DISTORTIONCOMPONENT_H__

#include "../DemoHeader.h"
#include "CurvePoint.h"

//==============================================================================
class DistortionComponent : public Component,
                            public Buffer::Listener,
                            public ComponentListener
{
public:
    //==============================================================================
    DistortionComponent (Buffer& bufferToControl);

    ~DistortionComponent();
    
    void resized();
    
    void paint (Graphics& g);
    
    void bufferChanged (Buffer* buffer);
    
    void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
        
    void resetBuffer();
    
private:
    //==============================================================================
	enum CurvePoints
    {
		pointX1,
		pointY1,
		pointX2,
		pointY2,
		numPoints,
	};    
    
    //==============================================================================
    Buffer& buffer;
    Path path;
    
    OwnedArray<CurvePoint> curvePoints;
    OwnedArray<Value> values;
    
    void refreshPath();
    void refillBuffer (float x1, float y1, float x2, float y2);
    void resetPoints();

    Image background;
    bool isInitialised;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionComponent);
};

#endif // __DROWAUDIO_DISTORTIONCOMPONENT_H__