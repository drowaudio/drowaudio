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

#include "DistortionDemo.h"
#include "../DemoLookAndFeel.h"

DistortionDemo::DistortionDemo (BufferTransformAudioSource& bufferTransformAudioSource_)
    : bufferTransformAudioSource (bufferTransformAudioSource_),
      distortionComponent (bufferTransformAudioSource.getBuffer())
{
    addAndMakeVisible (&distortionComponent);

    addAndMakeVisible (&resetButton);
    resetButton.setButtonText ("Reset");
    resetButton.addListener (this);
    
    addAndMakeVisible (&bypassButton);
    bypassButton.setButtonText ("Bypass");
    bypassButton.setClickingTogglesState (true);
    bypassButton.addListener (this);
}

DistortionDemo::~DistortionDemo()
{
    resetButton.removeListener (this);
    bypassButton.removeListener (this);
}

void DistortionDemo::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    const int m = 4;
    
    distortionComponent.setBounds (m, m, (w / 2) - (2 * m), h - (2 * m));
    resetButton.setBounds ((int) (w * 0.75f) - 80 - (m / 2), h - 20 - m, 80, 20);
    bypassButton.setBounds (resetButton.getBounds().translated (resetButton.getWidth() + m, 0));
}

void DistortionDemo::paint (Graphics& g)
{
    const int w = getWidth();
    //const int h = getHeight();
    const int m = 5;

//    g.fillAll (Colours::darkgrey);
    
    GuiHelpers::drawBevel (g, distortionComponent.getBounds().toFloat(), (float) m, Colours::darkgrey);
    
    g.setColour (Colours::white);
    g.setFont (Font (20, Font::underlined));
    g.drawSingleLineText ("Distortion Demo", (int) (w * 0.75f), 20, Justification::horizontallyCentred);
    
    g.setFont (Font (14, Font::plain));
    String text;
    text << "This graph demonstrates the Buffer and BufferTransformAudioSource classes." << newLine << newLine
    << "Drag the points around to change the buffer belonging to the audio chain's BufferTransformAudioSource." << newLine
    << "The Buffer's contents are used as a transform in the source which will distort the audio passing through it." << newLine << newLine
    << "Aditionally, Buffers can have listeners. Resetting the buffer to unity using the button below will update the graph.";
//    g.drawMultiLineText (text,
//                         (w * 0.5) + m, 20 + 28,
//                         (w * 0.5) - (2 * m));
    g.drawFittedText (text,
                      (int) ((w * 0.5f) + m), 28,
                      (int) ((w * 0.5f) - (2 * m)), resetButton.getY() - (28) - m,
                      Justification::left,
                      100, 1.0f);
}

void DistortionDemo::buttonClicked (Button* button)
{
    if (button == &resetButton)
    {
        distortionComponent.resetBuffer();
    }
    else if (button == &bypassButton)
    {
        bufferTransformAudioSource.setBypass (bypassButton.getToggleState());
    }
}