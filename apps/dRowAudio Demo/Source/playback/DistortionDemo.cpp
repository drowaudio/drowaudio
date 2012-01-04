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

#include "DistortionDemo.h"
#include "../DemoLookAndFeel.h"

DistortionDemo::DistortionDemo (Buffer& bufferToControl)
    : distortionComponent (bufferToControl)
{
    addAndMakeVisible (&distortionComponent);

    addAndMakeVisible (&resetButton);
    resetButton.setButtonText ("Reset");
    resetButton.addListener (this);
}

DistortionDemo::~DistortionDemo()
{
    resetButton.removeListener (this);
}

void DistortionDemo::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    const int m = 5;
    
    distortionComponent.setBounds (m, m, (w * 0.5f) - (2 * m), h - (2 * m));
    resetButton.setBounds ((w * 0.75f) - 40, h - 20 - m, 80, 20);
}

void DistortionDemo::paint (Graphics& g)
{
    const int w = getWidth();
    //const int h = getHeight();
    const int m = 5;

//    g.fillAll (Colours::darkgrey);
    
    GuiHelpers::drawBevel (g, distortionComponent.getBounds().toFloat(), m, Colours::darkgrey);
    
    g.setColour (Colours::white);
    g.setFont (20, Font::underlined);
    g.drawSingleLineText ("Distortion Demo", w * 0.75f, 20, Justification::horizontallyCentred);
    
    g.setFont (14);
    String text;
    text << "This graph demonstrates the Buffer and BufferTransferAudioSource classes." << newLine << newLine
    << "Drag the points around to change the buffer belonging to the audio chain's BufferTransforAudioSource." << newLine
    << "The Buffer's contents are used as a transform in the source which will distort the audio passing through it." << newLine << newLine
    << "Aditionally Buffers can have listeners. Resetting the buffer to unity using the button below will update the graph.";
//    g.drawMultiLineText (text,
//                         (w * 0.5) + m, 20 + 28,
//                         (w * 0.5) - (2 * m));
    g.drawFittedText (text,
                      (w * 0.5f) + m, 28,
                      (w * 0.5f) - (2 * m), resetButton.getY() - (28) - m,
                      Justification::left,
                      100, 1.0f);
}

void DistortionDemo::buttonClicked (Button* button)
{
    if (button == &resetButton)
    {
        distortionComponent.resetBuffer();
    }
}