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

#include "TrackInfoComponent.h"

TrackInfoComponent::TrackInfoComponent (AudioFilePlayerExt& audioFilePlayer_)
    : audioFilePlayer (audioFilePlayer_)
{
    audioFilePlayer.addListener (this);
    
    addAndMakeVisible (&bpmLabel);
    addAndMakeVisible (&remainLabel);

    bpmLabel.setJustificationType (Justification::centredRight);
    remainLabel.setJustificationType (Justification::centredRight);

    bpmLabel.setText ("...", dontSendNotification);
    remainLabel.setText ("--:--", dontSendNotification);
    
    bpmLabel.setColour (Label::textColourId, Colours::white);
    remainLabel.setColour (Label::textColourId, Colours::white);
    
    startTimer (40);
}

TrackInfoComponent::~TrackInfoComponent()
{
    audioFilePlayer.removeListener (this);
}

void TrackInfoComponent::resized()
{
    const int w = getWidth();
    //const int h = getHeight();
    const int m = 5;
    
    bpmLabel.setBounds (w - 50, m, 50, 20);
    remainLabel.setBounds (w - 100, bpmLabel.getBottom() + m, 100, 20);
}

void TrackInfoComponent::paint (Graphics& g)
{
    const int w = getWidth();
    const int h = getHeight();
    const int m = 5;

    g.setColour (Colours::white);
    g.setFont (20);
    
    ValueTree trackInfo (audioFilePlayer.getLibraryEntry());
    
    if (trackInfo.isValid())
    {
        String infoText;
        infoText << trackInfo[MusicColumns::columnNames[MusicColumns::Artist]].toString() << "\n";
        infoText << trackInfo[MusicColumns::columnNames[MusicColumns::Song]].toString() << "\n";
        infoText << trackInfo[MusicColumns::columnNames[MusicColumns::Album]].toString() << "\n";
        infoText << trackInfo[MusicColumns::columnNames[MusicColumns::BPM]].toString();
        
        g.setFont (16);
        g.drawText (trackInfo[MusicColumns::columnNames[MusicColumns::Artist]].toString(),
                    m, m, w - (2 * m), 16,
                    Justification::centredLeft,
                    true);

        g.setFont (20);
        g.drawText (trackInfo[MusicColumns::columnNames[MusicColumns::Song]].toString(),
                          m, 16 + (2 * m), w - (2 * m), 20,
                          Justification::centredLeft,
                          true);
        
        g.setColour (Colours::lightgrey);
        g.setFont (12);
        g.drawText (trackInfo[MusicColumns::columnNames[MusicColumns::Album]].toString(),
                    m, 36 + (3 * m), w - (2 * m), 12,
                    Justification::centredLeft,
                    true);
        
    }
    else
    {
        String displayText;
        if (audioFilePlayer.getTotalLength() > 0)
            displayText = "No Track Info Available";
        else
            displayText = "Drop Tracks Here to Begin...";
        
        g.drawFittedText (displayText,
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

void TrackInfoComponent::audioFilePlayerSettingChanged (AudioFilePlayer* player, int settingCode)
{
    if (player == &audioFilePlayer
        && settingCode == AudioFilePlayerExt::SoundTouchSetting)
    {
        ValueTree trackInfo (audioFilePlayer.getLibraryEntry());
        double bpm = trackInfo[MusicColumns::columnNames[MusicColumns::BPM]].toString().getDoubleValue();
        
        if (audioFilePlayer.getSoundTouchAudioSource() != nullptr)
            bpm *= audioFilePlayer.getSoundTouchAudioSource()->getSoundTouchProcessor().getEffectivePlaybackRatio();
        
        bpmLabel.setText (String (bpm, 2), dontSendNotification);
    }
}

void TrackInfoComponent::timerCallback()
{
    double timeRemaining = audioFilePlayer.getLengthInSeconds() - audioFilePlayer.getCurrentPosition();
    String remain (timeRemaining < 0.0 ? String::empty : "-");
    remain << timeToTimecodeStringLowRes (timeRemaining);
    remainLabel.setText (remain, dontSendNotification);
}