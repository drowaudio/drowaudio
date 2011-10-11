/*
  ==============================================================================

    TrackInfoComponent.cpp
    Created: 8 Oct 2011 4:26:24pm
    Author:  David Rowland

  ==============================================================================
*/

#include "TrackInfoComponent.h"

TrackInfoComponent::TrackInfoComponent (AudioFilePlayer& audioFilePlayer_)
    : audioFilePlayer (audioFilePlayer_)
{
    audioFilePlayer.addListener (this);
    addAndMakeVisible (&clock);
}

TrackInfoComponent::~TrackInfoComponent()
{
}

void TrackInfoComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    
    clock.setBounds (w - clock.getRequiredWidth(), 0, clock.getRequiredWidth(), 20);
}

void TrackInfoComponent::paint (Graphics& g)
{
    const int w = getWidth();
    const int h = getHeight();

    g.setColour (Colours::white);
    g.setFont (20);
    
    ValueTree trackInfo (audioFilePlayer.getLibraryEntry());
    
    if (trackInfo.isValid())
    {
        String infoText;
        infoText << trackInfo[Columns::columnNames[Columns::Artist]].toString() << "\n";
        infoText << trackInfo[Columns::columnNames[Columns::Song]].toString();
        
        g.drawFittedText (infoText,
                          0, 0, w, h,
                          Justification (Justification::topLeft),
                          2);        
    }
    else
    {
        g.drawFittedText ("No Track Info Available",
                          0, 0, w, h,
                          Justification (Justification::centred),
                          2);
    }
}

void TrackInfoComponent::fileChanged (AudioFilePlayer* player)
{
    if (player == &audioFilePlayer)
    {
        repaint();
    }
}