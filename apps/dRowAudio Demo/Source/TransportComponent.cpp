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
                                               0, 2,
                                               1, 2,
                                               false, false,
                                               true, false);
    settingsComp.setSize (500, 400);
    LookAndFeel settingsLaf;
    settingsLaf.setColour (Label::textColourId, Colours::white);
    settingsLaf.setColour (TextButton::buttonColourId, Colours::white);
    settingsLaf.setColour (TextButton::textColourOffId, Colours::black);
    settingsComp.setLookAndFeel (&settingsLaf);
    
    DialogWindow::showModalDialog ("Audio Settings",
                              &settingsComp, nullptr,
                              Colours::darkgrey,
                              true, true, true);
}
