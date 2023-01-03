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

#include "DemoLookAndFeel.h"

DemoLookAndFeel::DemoLookAndFeel()
{
}

void DemoLookAndFeel::drawButtonBackground (juce::Graphics& g,
                                            juce::Button& button,
                                            const juce::Colour& backgroundColour,
                                            bool isMouseOverButton,
                                            bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();

    const juce::Colour baseColour (GuiHelpers::createBaseColour (backgroundColour,
                                                           button.hasKeyboardFocus (true),
                                                           isMouseOverButton, isButtonDown)
                             .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    juce::ColourGradient cg (baseColour.brighter (0.5f), 0.0f, 0.0f,
                       baseColour.darker (0.5f), 0.0f, (float)height,
                       false);

    juce::Rectangle<float> bounds (g.getClipBounds().toFloat().reduced (0.0f, 1.0f));
    g.setGradientFill (cg);
    g.fillRoundedRectangle (bounds, 4.0f);

    bounds.setX (bounds.getX() + 0.5f);
    bounds.setWidth (bounds.getWidth() - 1.0f);
    bounds.setY (bounds.getY() - 0.5f);
    bounds.setHeight (bounds.getHeight());

    g.setColour (juce::Colours::black);
    g.drawRoundedRectangle (bounds, 4.0f, 1.0f);

    juce::ColourGradient highlight (juce::Colours::white.withAlpha (0.1f), 2.0f, (float)height,
                                    juce::Colours::white.withAlpha (0.1f), width - 2.0f, (float)height,
                              false);
    highlight.addColour (2.0f / (width - 4.0f),
                         juce::Colours::white.withAlpha (0.5f));
    highlight.addColour (1.0f - (2.0f / (width - 4.0f)),
                         juce::Colours::white.withAlpha (0.5f));
    g.setGradientFill (highlight);
    g.drawLine (2.0f, height - 0.5f, width - 2.0f, height - 0.5f, 0.5f);
}
