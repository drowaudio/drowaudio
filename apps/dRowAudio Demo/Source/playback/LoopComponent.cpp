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

#include "LoopComponent.h"
#include "AudioPlaybackDemo.h"

LoopComponent::LoopComponent (AudioFilePlayerExt& audioFilePlayer_)
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
    if (player == &audioFilePlayer)
    {
        if (audioFilePlayer.getLengthInSeconds() > 0)
        {
            const double w = getWidth();
            const double startTime = (jmin (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();
            const double endTime = (jmax (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();
            
            audioFilePlayer.setLoopTimes (startTime, endTime);
        }
    }
}

void LoopComponent::componentMovedOrResized (Component& /*component*/, bool /*wasMoved*/, bool /*wasResized*/)
{
    if (audioFilePlayer.getLengthInSeconds() > 0)
    {
        const double w = getWidth();
        const double startTime = (jmin (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();
        const double endTime = (jmax (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();

        audioFilePlayer.setLoopTimes (startTime, endTime);
    }
    
    AudioPlaybackDemo* demo = dynamic_cast<AudioPlaybackDemo*> (getParentComponent());
    if (demo != nullptr)
    {
        demo->repaint (demo->positionableWaveDisplay->getBounds());
    }
}

void LoopComponent::audioFilePlayerSettingChanged (AudioFilePlayer* player, int /*settingCode*/)
{
    if (player == &audioFilePlayer)
        repaint();
}