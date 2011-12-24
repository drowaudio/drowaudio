/*
  ==============================================================================

    TrackInfoComponent.cpp
    Created: 8 Oct 2011 4:26:24pm
    Author:  David Rowland

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

    bpmLabel.setText ("...", false);
    remainLabel.setText ("--:--", false);
    
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
        audioFilePlayerSettingChanged (player, AudioFilePlayerExt::SoundTouchSetting);
        
        repaint();
    }
}

void TrackInfoComponent::audioFilePlayerSettingChanged (AudioFilePlayer* player, int settingCode)
{
    if (settingCode == AudioFilePlayerExt::SoundTouchSetting)
    {
        ValueTree trackInfo (audioFilePlayer.getLibraryEntry());
        double bpm = trackInfo[MusicColumns::columnNames[MusicColumns::BPM]].toString().getDoubleValue();
        bpm *= audioFilePlayer.getSoundTouchAudioSource()->getSoundTouchProcessor().getEffectivePlaybackRatio();
        bpmLabel.setText (String (bpm, 2), false);
    }
}

void TrackInfoComponent::timerCallback()
{
    String remain ("-");
    remain << timeToTimecodeStringLowRes (audioFilePlayer.getLengthInSeconds() - audioFilePlayer.getCurrentPosition());
    remainLabel.setText (remain, false);
}