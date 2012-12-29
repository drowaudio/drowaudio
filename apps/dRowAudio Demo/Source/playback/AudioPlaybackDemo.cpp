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
//    addAndMakeVisible (positionalDisplay = new ColouredPositionableWaveDisplay (&audioFilePlayer,
//                                                                                &thumbnailCache, 
//                                                                                &audioThumbnail));
//    addAndMakeVisible (draggableDisplay = new ColouredDraggableWaveDisplay (128,
//                                                                            &audioFilePlayer,
//                                                                            &thumbnailCache, 
//                                                                            &audioThumbnail));
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
    
    addAndMakeVisible (&zoomSlider);
    zoomSlider.addListener (this);
    zoomSlider.setRange (0.01, 2);
    zoomSlider.setTextBoxStyle (Slider::NoTextBox, false, 50, 20);
    zoomSlider.setSkewFactorFromMidPoint (1.0);
    zoomSlider.setValue (1.0);
    zoomSlider.setSliderStyle (Slider::RotaryVerticalDrag);
    
    addAndMakeVisible (&zoomLabel);
    zoomLabel.setText ("Zoom", false);
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

    playerControlLabels[lowEQ]->setText ("Low EQ", false);
    playerControlLabels[midEQ]->setText ("Mid EQ", false);
    playerControlLabels[highEQ]->setText ("High EQ", false);
    playerControlLabels[rate]->setText ("Rate", false);
    playerControlLabels[tempo]->setText ("Tempo", false);
    playerControlLabels[pitch]->setText ("Pitch", false);
    
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
    
    resolutionSlider.setBounds (0, 0, 50, 50);
    positionableWaveDisplay->setBounds (resolutionSlider.getRight() + bevelSize, bevelSize,
                                        w - (resolutionSlider.getWidth() + 2 * bevelSize), 50 - (2 * bevelSize));
    
    zoomSlider.setBounds (0, resolutionSlider.getBottom() + m, 50, 50);
    draggableWaveDisplay->setBounds (zoomSlider.getRight() + bevelSize, positionableWaveDisplay->getBottom() + m + bevelSize,
                                 w - (zoomSlider.getWidth() + 2 * bevelSize), 50 - (2 * bevelSize));

    const int centre = w / 2;
    int offset = (centre - (80 * 3)) / 2;
    for (int i = 0; i < rate; i++)
    {
        playerControls[i]->setBounds (offset + i * 80 + 2, zoomSlider.getBottom() + 20 + 3 * m, 76, 76);
    }        

    offset += centre / 2;
//    const int offset = (w - numControls * 80) * 0.5;
    for (int i = rate; i < numControls; i++)
    {
        playerControls[i]->setBounds (offset + i * 80 + 2, zoomSlider.getBottom() + 20 + 3 * m, 76, 76);
    }
    
    m *= 2;
    filterGroup.setBounds (playerControls[0]->getX() - m, playerControlLabels[0]->getY() - m,
                           playerControls[2]->getRight() - playerControls[0]->getX() + (2 * m), playerControls[0]->getBottom() - playerControlLabels[0]->getY() + (2 * m));
    
    rateGroup.setBounds (playerControls[3]->getX() - m, playerControlLabels[3]->getY() - m,
                         playerControls[5]->getRight() - playerControls[3]->getX() + (2 * m), playerControls[3]->getBottom() - playerControlLabels[3]->getY() + (2 * m));   
    
//    positionableWaveDisplay->setBounds (5, filterGroup.getBottom() + 5, w - (2 * 5), 100);
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
