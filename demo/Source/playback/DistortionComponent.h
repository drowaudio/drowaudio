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

#ifndef DISTORTION_COMPONENT_H
#define DISTORTION_COMPONENT_H

#include "CurvePoint.h"

class DistortionComponent : public juce::Component,
                            public Buffer::Listener,
                            public juce::ComponentListener
{
public:
    DistortionComponent (Buffer& bufferToControl);

    ~DistortionComponent();

    void resetBuffer();

    //==============================================================================
    void resized() override;
    void paint (juce::Graphics& g) override;
    void bufferChanged (Buffer* buffer) override;
    void componentMovedOrResized (juce::Component& component, bool wasMoved, bool wasResized) override;

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
    juce::Path path;

    juce::OwnedArray<CurvePoint> curvePoints;
    juce::OwnedArray<juce::Value> values;

    void refreshPath();
    void refillBuffer (float x1, float y1, float x2, float y2);
    void resetPoints();

    juce::Image background;
    bool isInitialised;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionComponent)
};

#endif //DISTORTION_COMPONENT_H
