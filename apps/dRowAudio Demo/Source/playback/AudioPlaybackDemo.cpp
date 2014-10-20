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

#include "AudioPlaybackDemo.h"


AudioPlaybackDemo::AudioPlaybackDemo (AudioFilePlayerExt& audioFilePlayer_,
                                      BufferTransformAudioSource& bufferTransformAudioSource)
    : audioFilePlayer (audioFilePlayer_),
      loopComponent (audioFilePlayer),
      backgroundThread ("Waveform Thread"),
      audioThumbnailCache (10),
      audioThumbnail (512, *audioFilePlayer.getAudioFormatManager(), audioThumbnailCache),
      distortionDemo (bufferTransformAudioSource)
{
    audioThumbnailImage = new AudioThumbnailImage (audioFilePlayer, backgroundThread, audioThumbnail, 512);
    
    positionableWaveDisplay = new PositionableWaveDisplay (*audioThumbnailImage, backgroundThread);
    addAndMakeVisible (positionableWaveDisplay);
    
    draggableWaveDisplay = new DraggableWaveDisplay (*audioThumbnailImage);
    addAndMakeVisible (draggableWaveDisplay);
    
    addAndMakeVisible (&resolutionSlider);
    resolutionSlider.addListener (this);
    resolutionSlider.setRange (0, 20);
    resolutionSlider.setTextBoxStyle (Slider::NoTextBox, false, 50, 20);
    resolutionSlider.setValue (3.0);
    resolutionSlider.setSkewFactorFromMidPoint (3.0);
    resolutionSlider.setSliderStyle (Slider::RotaryHorizontalDrag);
    
    addAndMakeVisible (&resolutionLabel);
    resolutionLabel.setText ("Detail", dontSendNotification);
    resolutionLabel.setFont (12.0f);
    resolutionLabel.setJustificationType (Justification::centred);
    resolutionLabel.attachToComponent (&resolutionSlider, false);
    resolutionLabel.setColour (Label::textColourId, Colours::white);

    addAndMakeVisible (&zoomSlider);
    zoomSlider.addListener (this);
    zoomSlider.setRange (0.01, 2);
    zoomSlider.setTextBoxStyle (Slider::NoTextBox, false, 50, 20);
    zoomSlider.setSkewFactorFromMidPoint (1.0);
    zoomSlider.setValue (1.0);
    zoomSlider.setSliderStyle (Slider::RotaryVerticalDrag);
    
    addAndMakeVisible (&zoomLabel);
    zoomLabel.setText ("Zoom", dontSendNotification);
    zoomLabel.setFont (12.0f);
    zoomLabel.setJustificationType (Justification::centred);
    zoomLabel.attachToComponent (&zoomSlider, false);
    zoomLabel.setColour (Label::textColourId, Colours::white);
    
    addAndMakeVisible (&loopComponent);
    
    addAndMakeVisible (&filterGroup);
    addAndMakeVisible (&rateGroup);
    filterGroup.setText ("Filter");
    rateGroup.setText ("Tempo and Pitch");
    
    filterGroup.setColour (GroupComponent::outlineColourId, Colours::white);
    filterGroup.setColour (GroupComponent::textColourId, Colours::white);
    rateGroup.setColour (GroupComponent::outlineColourId, Colours::white);
    rateGroup.setColour (GroupComponent::textColourId, Colours::white);
    
    for (int i = 0; i < numControls; i++)
    {
        playerControls.add (new Slider());
        playerControlLabels.add (new Label ());
        addAndMakeVisible (playerControls[i]);
        addAndMakeVisible (playerControlLabels[i]);
        playerControlLabels[i]->setFont (12.0f);
        playerControlLabels[i]->attachToComponent (playerControls[i], false);
        playerControls[i]->addListener (this);
        playerControls[i]->setValue (1.0);
        playerControls[i]->setSliderStyle (Slider::RotaryVerticalDrag);
        playerControls[i]->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 16);
        
        Justification centreJustification (Justification::centred);
        playerControlLabels[i]->setJustificationType (centreJustification);
        playerControlLabels[i]->setColour (Label::textColourId, Colours::white);
    }
    
    playerControls[lowEQ]->setRange (0.05, 2, 0.001);
    playerControls[midEQ]->setRange (0.05, 2, 0.001);
    playerControls[highEQ]->setRange (0.05, 2, 0.001);

    playerControls[rate]->setRange (0.5, 1.5, 0.001);
    playerControls[tempo]->setRange (0.5, 1.5, 0.001);
    playerControls[pitch]->setRange (0.5, 1.5, 0.001);
    
    for (int i = 0; i < numControls; i++)
        playerControls[i]->setSkewFactorFromMidPoint (1.0);

    playerControlLabels[lowEQ]->setText ("Low EQ", dontSendNotification);
    playerControlLabels[midEQ]->setText ("Mid EQ", dontSendNotification);
    playerControlLabels[highEQ]->setText ("High EQ", dontSendNotification);
    playerControlLabels[rate]->setText ("Rate", dontSendNotification);
    playerControlLabels[tempo]->setText ("Tempo", dontSendNotification);
    playerControlLabels[pitch]->setText ("Pitch", dontSendNotification);
    
    addAndMakeVisible (&distortionDemo);
    
    backgroundThread.startThread (1);
}

AudioPlaybackDemo::~AudioPlaybackDemo()
{
    resolutionSlider.removeListener (this);
    zoomSlider.removeListener (this);

    for (int i = 0; i < numControls; i++)
    {
        playerControls[i]->removeListener (this);
    }
}

void AudioPlaybackDemo::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    int m = 5;
    const int bevelSize = 2;
    
    Rectangle<int> posBounds (0, 0, w, 50);
    resolutionSlider.setBounds (posBounds.removeFromLeft (50).removeFromBottom (35));
    positionableWaveDisplay->setBounds (posBounds.reduced (bevelSize));

    Rectangle<int> dragBounds (0, 50 + m, w, 50);
    zoomSlider.setBounds (dragBounds.removeFromLeft (50).removeFromBottom (35));
    draggableWaveDisplay->setBounds (dragBounds.reduced (bevelSize));

    const int centre = w / 2;
    int offset = (centre - (80 * 3)) / 2;
    for (int i = 0; i < rate; i++)
    {
        playerControls[i]->setBounds (offset + i * 80 + 2, zoomSlider.getBottom() + 20 + 3 * m, 76, 76);
    }        

    offset += centre / 2;

    for (int i = rate; i < numControls; i++)
        playerControls[i]->setBounds (offset + i * 80 + 2, zoomSlider.getBottom() + 20 + 3 * m, 76, 76);

    m *= 2;
    filterGroup.setBounds (playerControls[0]->getX() - m, playerControlLabels[0]->getY() - m,
                           playerControls[2]->getRight() - playerControls[0]->getX() + (2 * m), playerControls[0]->getBottom() - playerControlLabels[0]->getY() + (2 * m));
    
    rateGroup.setBounds (playerControls[3]->getX() - m, playerControlLabels[3]->getY() - m,
                         playerControls[5]->getRight() - playerControls[3]->getX() + (2 * m), playerControls[3]->getBottom() - playerControlLabels[3]->getY() + (2 * m));   
    
    loopComponent.setBounds (positionableWaveDisplay->getBounds());
    
    m /= 2;
    distortionDemo.setBounds (0, filterGroup.getBottom() + (2 * m), w, h - filterGroup.getBottom() - (2 * m));
}

void AudioPlaybackDemo::paint (Graphics& g)
{
    GuiHelpers::drawBevel (g, positionableWaveDisplay->getBounds().toFloat(), 2.0f, Colours::darkgrey);
    GuiHelpers::drawBevel (g, draggableWaveDisplay->getBounds().toFloat(), 2.0f, Colours::darkgrey);
    
    g.setColour (Colours::grey.brighter());
    g.drawHorizontalLine (filterGroup.getBottom() + 5, 5.0f, getWidth() - 5.0f);
}

void AudioPlaybackDemo::sliderValueChanged (Slider* slider)
{
    if (slider == &resolutionSlider)
    {
        audioThumbnailImage->setResolution (resolutionSlider.getValue());
    }
    else if (slider == &zoomSlider)
    {
        draggableWaveDisplay->setHorizontalZoom ((float) zoomSlider.getValue());
    }
    else if (slider == playerControls[lowEQ])
    {
        audioFilePlayer.setFilterGain (FilteringAudioSource::Low, (float) playerControls[lowEQ]->getValue());
    }
    else if (slider == playerControls[midEQ])
    {
        audioFilePlayer.setFilterGain (FilteringAudioSource::Mid, (float) playerControls[midEQ]->getValue());
    }
    else if (slider == playerControls[highEQ])
    {
        audioFilePlayer.setFilterGain (FilteringAudioSource::High, (float) playerControls[highEQ]->getValue());
    }
    if (audioFilePlayer.getSoundTouchAudioSource() != nullptr)
    {
        if (slider == playerControls[rate])
        {
            SoundTouchProcessor::PlaybackSettings settings (audioFilePlayer.getPlaybackSettings());
            settings.rate = (float) playerControls[rate]->getValue();
            audioFilePlayer.setPlaybackSettings (settings);
        }
        else if (slider == playerControls[tempo])
        {
            SoundTouchProcessor::PlaybackSettings settings (audioFilePlayer.getPlaybackSettings());
            settings.tempo = (float) playerControls[tempo]->getValue();
            audioFilePlayer.setPlaybackSettings (settings);
        }
        else if (slider == playerControls[pitch])
        {
            SoundTouchProcessor::PlaybackSettings settings (audioFilePlayer.getPlaybackSettings());
            settings.pitch = (float) playerControls[pitch]->getValue();
            audioFilePlayer.setPlaybackSettings (settings);
        }
    }
}
