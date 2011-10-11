/*
  ==============================================================================

    TransportComponent.cpp
    Created: 8 Oct 2011 7:36:32pm
    Author:  David Rowland

  ==============================================================================
*/

#include "TransportComponent.h"

TransportComponent::TransportComponent (AudioFilePlayer& audioFilePlayer_)
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
    const int h = getHeight();
    
    for (int i = 0; i < numButtons; i++)
    {
        buttons[i]->setBounds (w - 80, 20 * i + 1, 80, 18);
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
