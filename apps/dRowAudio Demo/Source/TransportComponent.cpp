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

TransportComponent::TransportComponent (AudioFilePlayerExt& audioFilePlayer_)
    : audioFilePlayer (audioFilePlayer_)
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
    buttons[reverse]->setButtonText ("Reverse");
    
    buttons[loop]->setClickingTogglesState (true);
    buttons[reverse]->setClickingTogglesState (true);
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
        buttons[i]->setBounds (0, 20 * i + 1, w, 20);
    }
}

void TransportComponent::buttonClicked (Button* button)
{
    if (button == buttons[play]) 
    {
        DBG ("play");
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
        audioFilePlayer.setPlayDirection (! button->getToggleState());
    }
}
