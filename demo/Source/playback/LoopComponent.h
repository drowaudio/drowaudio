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

#ifndef LOOP_COMPONENT_H
#define LOOP_COMPONENT_H

#include "../DemoHeader.h"

class LoopMarker : public juce::Component
{
public:
    LoopMarker() :
        isMouseOver (false)
    {
    }

    //==============================================================================
    void resized() override
    {
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(), getHeight(), getWidth());
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (isMouseOver ? juce::Colours::red.withAlpha (0.9f) : juce::Colours::red.withAlpha (0.7f));
    }

    void mouseEnter (const juce::MouseEvent&) override
    {
        setMouseCursor (juce::MouseCursor::PointingHandCursor);
        isMouseOver = true;
        repaint();
    }

    void mouseExit (const juce::MouseEvent&) override
    {
        setMouseCursor (juce::MouseCursor::NormalCursor);
        isMouseOver = false;
        repaint();
    }

    void mouseDown (const juce::MouseEvent& e) override
    {
        setMouseCursor (juce::MouseCursor::DraggingHandCursor);
        dragger.startDraggingComponent (this, e);
    }

    void mouseUp (const juce::MouseEvent&) override
    {
        setMouseCursor (juce::MouseCursor::PointingHandCursor);
    }

    void mouseDrag (const juce::MouseEvent& e) override
    {
        dragger.dragComponent (this, e, &constrainer);
    }

private:
    //==============================================================================
    juce::ComponentDragger dragger;
    juce::ComponentBoundsConstrainer constrainer;
    bool isMouseOver;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopMarker)
};

//==============================================================================
class LoopComponent : public juce::Component,
                      public juce::ComponentListener,
                      public AudioFilePlayer::Listener
{
public:
    LoopComponent (AudioFilePlayerExt& audioFilePlayer);

    ~LoopComponent();

    //==============================================================================
    void resized() override;
    void paint (juce::Graphics& g) override;
    void componentMovedOrResized (juce::Component& component, bool wasMoved, bool wasResized) override;
    void fileChanged (AudioFilePlayer* player) override;
    void audioFilePlayerSettingChanged (AudioFilePlayer* player, int settingCode) override;

private:
    //==============================================================================
    AudioFilePlayerExt& audioFilePlayer;
    LoopMarker marker1, marker2;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopComponent)
};

#endif //LOOP_COMPONENT_H
