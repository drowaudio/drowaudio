/*
  ==============================================================================

    LoopComponent.cpp
    Created: 9 Oct 2011 3:04:02am
    Author:  David Rowland

  ==============================================================================
*/

#include "LoopComponent.h"
#include "AudioPlaybackDemo.h"

LoopComponent::LoopComponent (AudioFilePlayer& audioFilePlayer_)
    : audioFilePlayer (audioFilePlayer_)
{
    setInterceptsMouseClicks (false, true);
    
    audioFilePlayer.addListener (this);
    
    marker1.setBounds (50, 0, 3, getHeight());
    marker2.setBounds (150, 0, 3, getHeight());
    addAndMakeVisible (&marker1);
    addAndMakeVisible (&marker2);
    
    marker1.addComponentListener (this);
    marker2.addComponentListener (this);
}

LoopComponent::~LoopComponent()
{
    audioFilePlayer.removeListener (this);

    marker1.removeComponentListener (this);
    marker2.removeComponentListener (this);
}

void LoopComponent::resized()
{
    marker1.setBounds (marker1.getX(), 0, marker1.getWidth(), getHeight());
    marker2.setBounds (marker2.getX(), 0, marker2.getWidth(), getHeight());
}

void LoopComponent::paint (Graphics& g)
{
    g.setColour (audioFilePlayer.getLoopBetweenTimes() ? Colours::red.withAlpha (0.4f) : Colours::red.withAlpha (0.2f));
    g.fillRect (marker1.getRight(), 0, marker2.getX() - marker1.getRight(), getHeight());
}

void LoopComponent::fileChanged (AudioFilePlayer* player)
{
    if (audioFilePlayer.getLengthInSeconds() > 0)
    {
        const double w = getWidth();
        const double startTime = (jmin (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();
        const double endTime = (jmax (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();
        
        audioFilePlayer.setLoopTimes (startTime, endTime);
        DBG ("start: " << startTime << " - end: " << endTime);
    }
}

void LoopComponent::componentMovedOrResized (Component& component, bool wasMoved, bool wasResized)
{
    if (audioFilePlayer.getLengthInSeconds() > 0)
    {
        const double w = getWidth();
        const double startTime = (jmin (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();
        const double endTime = (jmax (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();

        audioFilePlayer.setLoopTimes (startTime, endTime);
        DBG ("start: " << startTime << " - end: " << endTime);
    }
    
    AudioPlaybackDemo* demo = dynamic_cast<AudioPlaybackDemo*> (getParentComponent());
    if (demo != nullptr)
    {
        demo->repaint (demo->positionalDisplay->getBounds());
    }
}

void LoopComponent::loopBetweenTimesChanged (AudioFilePlayer* player)
{
    repaint();
}