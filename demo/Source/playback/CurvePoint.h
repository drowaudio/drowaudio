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

#ifndef CURVE_POINT_H
#define CURVE_POINT_H

#include "../DemoHeader.h"

class CurvePoint : public juce::Component,
                   public juce::ComponentDragger
{
public:
    CurvePoint() :
        mouseIsOver (false)
    {
    }

    //==============================================================================
    void resized() override
    {
        int halfWidth = getWidth() / 2;
        int halfHeight = getHeight() / 2;
        constrainer.setMinimumOnscreenAmounts (halfHeight, halfWidth, halfHeight, halfWidth);
    }

    void paint (juce::Graphics& g) override
    {
        g.setColour (juce::Colours::grey);
        g.fillAll();

        if (mouseIsOver)
        {
            g.setColour (juce::Colours::lightgrey);
            g.drawRect (0, 0, getWidth(), getHeight(), 2);
        }
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        startDraggingComponent (this, e);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        dragComponent (this, e, &constrainer);
    }

    void mouseEnter (const juce::MouseEvent&) override
    {
        mouseIsOver = true;
        repaint();
    }

    void mouseExit (const juce::MouseEvent&) override
    {
        mouseIsOver = false;
        repaint();
    }

private:
    //==============================================================================
    juce::ComponentBoundsConstrainer constrainer;
    bool mouseIsOver;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CurvePoint)
};

#endif //CURVE_POINT_H
