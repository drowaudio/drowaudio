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

#include "TransportComponent.h"

TransportComponent::TransportComponent (AudioDeviceManager& audioDeviceManager_, AudioFilePlayerExt& audioFilePlayer_)
    : audioDeviceManager (audioDeviceManager_),
      audioFilePlayer (audioFilePlayer_)
{
    for (int i = 0; i < numButtons; i++)
    {
        buttons.add (new TextButton());
        addAndMakeVisible (buttons[i]);
        buttons[i]->addListener (this);
    }
    
    buttons[play]->setButtonText ("Play");
    buttons[stop]->setButtonText ("Stop");
    buttons[loop]->setButtonText ("Loop");
    
    // we'll just use this as a settings button for now while
    // the reverse feature is being developed
    buttons[reverse]->setButtonText ("Settings");
    
    buttons[loop]->setClickingTogglesState (true);
    //buttons[reverse]->setClickingTogglesState (true);
}

TransportComponent::~TransportComponent()
{
    for (int i = 0; i < numButtons; i++)
    {
        buttons[i]->removeListener (this);
    }    
}

void TransportComponent::resized()
{
    const int w = getWidth();
//    const int h = getHeight();
    
    for (int i = 0; i < numButtons; i++)
    {
        buttons[i]->setBounds (0, 20 * i + 1, w, 18);
    }
}

void TransportComponent::buttonClicked (Button* button)
{
    if (button == buttons[play]) 
    {
        audioFilePlayer.start();
    }
    else if (button == buttons[stop]) 
    {
        audioFilePlayer.stop();
    }
    else if (button == buttons[loop]) 
    {
        audioFilePlayer.setLoopBetweenTimes (button->getToggleState());
    }
    else if (button == buttons[reverse]) 
    {
        showAudioSettings();
        //audioFilePlayer.setPlayDirection (! button->getToggleState());
    }
}

//==============================================================================
void TransportComponent::showAudioSettings()
{
    AudioDeviceSelectorComponent settingsComp (audioDeviceManager,
                                               1, 2,
                                               1, 2,
                                               false, false,
                                               true, false);
    settingsComp.setSize (500, 400);
    LookAndFeel_V3 settingsLaf;
    settingsLaf.setColour (Label::textColourId, Colours::white);
    settingsLaf.setColour (TextButton::buttonColourId, Colours::white);
    settingsLaf.setColour (TextButton::textColourOffId, Colours::black);
    settingsComp.setLookAndFeel (&settingsLaf);
    
    DialogWindow::showModalDialog ("Audio Settings",
                              &settingsComp, nullptr,
                              Colours::darkgrey,
                              true, true, true);
}
