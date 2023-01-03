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
                                      BufferTransformAudioSource& bufferTransformAudioSource) :
    audioFilePlayer (audioFilePlayer_),
    loopComponent (audioFilePlayer),
    backgroundThread ("Waveform Thread"),
    audioThumbnailCache (10),
    audioThumbnail (512, *audioFilePlayer.getAudioFormatManager(), audioThumbnailCache),
    distortionDemo (bufferTransformAudioSource)
{
    audioThumbnailImage = std::make_unique<AudioThumbnailImage> (audioFilePlayer, backgroundThread, audioThumbnail, 512);

    positionableWaveDisplay = std::make_unique<PositionableWaveDisplay> (*audioThumbnailImage, backgroundThread);
    addAndMakeVisible (*positionableWaveDisplay);

    draggableWaveDisplay = std::make_unique<DraggableWaveDisplay> (*audioThumbnailImage);
    addAndMakeVisible (*draggableWaveDisplay);

    addAndMakeVisible (&resolutionSlider);
    resolutionSlider.addListener (this);
    resolutionSlider.setRange (0, 20);
    resolutionSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    resolutionSlider.setValue (3.0);
    resolutionSlider.setSkewFactorFromMidPoint (3.0);
    resolutionSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);

    addAndMakeVisible (&resolutionLabel);
    resolutionLabel.setText ("Detail", juce::dontSendNotification);
    resolutionLabel.setFont (12.0f);
    resolutionLabel.setJustificationType (juce::Justification::centred);
    resolutionLabel.attachToComponent (&resolutionSlider, false);
    resolutionLabel.setColour (juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible (&zoomSlider);
    zoomSlider.addListener (this);
    zoomSlider.setRange (0.01, 2);
    zoomSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 50, 20);
    zoomSlider.setSkewFactorFromMidPoint (1.0);
    zoomSlider.setValue (1.0);
    zoomSlider.setSliderStyle (juce::Slider::RotaryVerticalDrag);

    addAndMakeVisible (&zoomLabel);
    zoomLabel.setText ("Zoom", juce::dontSendNotification);
    zoomLabel.setFont (12.0f);
    zoomLabel.setJustificationType (juce::Justification::centred);
    zoomLabel.attachToComponent (&zoomSlider, false);
    zoomLabel.setColour (juce::Label::textColourId, juce::Colours::white);

    addAndMakeVisible (&loopComponent);

    addAndMakeVisible (&filterGroup);
    addAndMakeVisible (&rateGroup);
    filterGroup.setText ("Filter");
    rateGroup.setText ("Tempo and Pitch");

    filterGroup.setColour (juce::GroupComponent::outlineColourId, juce::Colours::white);
    filterGroup.setColour (juce::GroupComponent::textColourId, juce::Colours::white);
    rateGroup.setColour (juce::GroupComponent::outlineColourId, juce::Colours::white);
    rateGroup.setColour (juce::GroupComponent::textColourId, juce::Colours::white);

    for (int i = 0; i < numControls; ++i)
    {
        juce::Slider* slider = playerControls.add (new juce::Slider());
        slider->setValue (1.0);
        slider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
        slider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 16);
        slider->addListener (this);
        addAndMakeVisible (slider);

        juce::Label* label = playerControlLabels.add (new juce::Label());
        label->setFont (12.0f);
        label->attachToComponent (slider, false);
        label->setJustificationType (juce::Justification::centred);
        label->setColour (juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible (label);
    }

    playerControls[lowEQ]->setRange (0.05, 2, 0.001);
    playerControls[midEQ]->setRange (0.05, 2, 0.001);
    playerControls[highEQ]->setRange (0.05, 2, 0.001);

    playerControls[rate]->setRange (0.5, 1.5, 0.001);
    playerControls[tempo]->setRange (0.5, 1.5, 0.001);
    playerControls[pitch]->setRange (0.5, 1.5, 0.001);

    for (int i = 0; i < numControls; ++i)
        playerControls[i]->setSkewFactorFromMidPoint (1.0);

    playerControlLabels[lowEQ]->setText ("Low EQ", juce::dontSendNotification);
    playerControlLabels[midEQ]->setText ("Mid EQ", juce::dontSendNotification);
    playerControlLabels[highEQ]->setText ("High EQ", juce::dontSendNotification);
    playerControlLabels[rate]->setText ("Rate", juce::dontSendNotification);
    playerControlLabels[tempo]->setText ("Tempo", juce::dontSendNotification);
    playerControlLabels[pitch]->setText ("Pitch", juce::dontSendNotification);

    addAndMakeVisible (&distortionDemo);

    backgroundThread.startThread (juce::Thread::Priority::background);
}

void AudioPlaybackDemo::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    int m = 5;
    const int bevelSize = 2;

    juce::Rectangle<int> posBounds (0, 0, w, 50);
    resolutionSlider.setBounds (posBounds.removeFromLeft (50).removeFromBottom (35));
    positionableWaveDisplay->setBounds (posBounds.reduced (bevelSize));

    juce::Rectangle<int> dragBounds (0, 50 + m, w, 50);
    zoomSlider.setBounds (dragBounds.removeFromLeft (50).removeFromBottom (35));
    draggableWaveDisplay->setBounds (dragBounds.reduced (bevelSize));
    
    for (int i = 0; i < numControls; ++i)
        playerControls[i]->setBounds (i * 80 + 3 * m, zoomSlider.getBottom() + 20 + 3 * m, 50, 50);

    m *= 2;
    filterGroup.setBounds (playerControls[0]->getX() - m, 
                           playerControlLabels[0]->getY() - m,
                           playerControls[2]->getRight() - playerControls[0]->getX() + (2 * m), 
                           playerControls[0]->getBottom() - playerControlLabels[0]->getY() + (2 * m));

    rateGroup.setBounds (playerControls[3]->getX() - m, 
                         playerControlLabels[3]->getY() - m,
                         playerControls[5]->getRight() - playerControls[3]->getX() + (2 * m), 
                         playerControls[3]->getBottom() - playerControlLabels[3]->getY() + (2 * m));

    loopComponent.setBounds (positionableWaveDisplay->getBounds());

    m /= 2;
    distortionDemo.setBounds (0, filterGroup.getBottom() + (2 * m), w, h - filterGroup.getBottom() - (2 * m));
}

void AudioPlaybackDemo::paint (juce::Graphics& g)
{
    GuiHelpers::drawBevel (g, positionableWaveDisplay->getBounds().toFloat(), 2.0f, juce::Colours::darkgrey);
    GuiHelpers::drawBevel (g, draggableWaveDisplay->getBounds().toFloat(), 2.0f, juce::Colours::darkgrey);

    g.setColour (juce::Colours::grey.brighter());
    g.drawHorizontalLine (filterGroup.getBottom() + 5, 5.0f, getWidth() - 5.0f);
}

void AudioPlaybackDemo::sliderValueChanged (juce::Slider* slider)
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
