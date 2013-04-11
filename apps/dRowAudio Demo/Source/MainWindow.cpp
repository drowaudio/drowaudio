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

#include "MainWindow.h"
#include "MainComponent.h"

//==============================================================================
MainAppWindow::MainAppWindow()
    : DocumentWindow (JUCEApplication::getInstance()->getApplicationName(),
                      Colours::darkgrey,
                      DocumentWindow::allButtons)
{
    ValueTree testTree ("test");
    testTree.setProperty ("prop_1", 10, nullptr);
    testTree.setProperty ("prop_2", "string", nullptr);
    ValueTree childTree ("child");
    childTree.setProperty ("prop_3", 123.0456, nullptr);
    testTree.addChild (childTree, -1, nullptr);
    DBG_OBJ (testTree);
    
    XmlElement xml ("test");
    xml.setAttribute ("prop_1", "value_1");
    xml.setAttribute ("prop_2", "value_2");
    XmlElement* xml2 = new XmlElement ("child");
    xml2->setAttribute ("prop_3", "value_3");
    xml.addChildElement (xml2);
    DBG_OBJ (&xml);

    const double test = 32762.32569;
    const String st ("hello world");
    DBG_VAR (test);
    DBG_OBJ (st);
    
    Point<double> point (45.947, 23239.8236);
    Range<float> range (45.947f, 23239.8236f);
    Line<int> line (4, 5, 6, 7);
    Rectangle<double> rect (636, 546, 456, 345);
    
    DBG_POINT (point);
    DBG_RANGE (range);
    DBG_LINE (line);
    DBG_RECT (rect);

    LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);
    setupColours();

    setContentOwned (new MainComponent, false);
    centreWithSize (800, 600);
    setVisible (true);
    setResizable (true, true);
}

MainAppWindow::~MainAppWindow()
{
}

void MainAppWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

//==============================================================================
void MainAppWindow::setupColours()
{
    LookAndFeel& laf = getLookAndFeel();
    laf.setColour (TextButton::buttonColourId, Colours::lightgrey);
    laf.setColour (TextButton::buttonOnColourId, Colours::grey);
    laf.setColour (ToggleButton::textColourId, Colours::white);
    laf.setColour (Slider::rotarySliderFillColourId, Colours::white);
}
