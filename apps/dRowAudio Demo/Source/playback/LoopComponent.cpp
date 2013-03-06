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
    const int startX = jmin (marker1.getRight(), marker2.getRight());
    const int endX = jmax (marker1.getX(), marker2.getX());

    g.setColour (audioFilePlayer.getLoopBetweenTimes() ? Colours::red.withAlpha (0.4f) : Colours::red.withAlpha (0.2f));
    g.fillRect (startX, 0, endX - startX, getHeight());
}

void LoopComponent::fileChanged (AudioFilePlayer* player)
{
    if (player == &audioFilePlayer)
    {
        if (audioFilePlayer.getLengthInSeconds() > 0)
        {
            const double w = getWidth();
            const double startTime = (jmin (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();
            const double endTime = (jmax (marker1.getRight(), marker2.getRight()) / w) * audioFilePlayer.getLengthInSeconds();
            
            audioFilePlayer.setLoopTimes (startTime, endTime);
        }
    }
}

void LoopComponent::componentMovedOrResized (Component& /*component*/, bool /*wasMoved*/, bool /*wasResized*/)
{
    if (audioFilePlayer.getLengthInSeconds() > 0.0)
    {
        const double w = getWidth();
        const double startTime = (jmin (marker1.getX(), marker2.getX()) / w) * audioFilePlayer.getLengthInSeconds();
        const double endTime = (jmax (marker1.getRight(), marker2.getRight()) / w) * audioFilePlayer.getLengthInSeconds();

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