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

static void drawBevel (Graphics& g, Rectangle<float> innerBevelBounds, float bevelThickness, Colour baseColour)
{
    Rectangle<float> outerBevelBounds (innerBevelBounds.expanded (bevelThickness, bevelThickness));
    Rectangle<float> centreBevelBounds (innerBevelBounds.expanded (bevelThickness * 0.5, bevelThickness * 0.5));
    
    Path pL, pR, pT, pB, pTL, pTR, pBL, pBR;
    pL.startNewSubPath (centreBevelBounds.getTopLeft());
    pL.lineTo (centreBevelBounds.getBottomLeft());
    
    pR.startNewSubPath (centreBevelBounds.getTopRight());
    pR.lineTo (centreBevelBounds.getBottomRight());
    
    pT.startNewSubPath (centreBevelBounds.getTopLeft());
    pT.lineTo (centreBevelBounds.getTopRight());
    
    pB.startNewSubPath (centreBevelBounds.getBottomLeft());
    pB.lineTo (centreBevelBounds.getBottomRight());
    
    pTL.addTriangle (outerBevelBounds.getX(), outerBevelBounds.getY(),
                     outerBevelBounds.getX(), innerBevelBounds.getY(),
                     innerBevelBounds.getX(), innerBevelBounds.getY());

    pTR.addTriangle (outerBevelBounds.getRight(), outerBevelBounds.getY(),
                     outerBevelBounds.getRight(), innerBevelBounds.getY(),
                     innerBevelBounds.getRight(), innerBevelBounds.getY());

    pBL.addTriangle (outerBevelBounds.getX(), outerBevelBounds.getBottom(),
                     outerBevelBounds.getX(), innerBevelBounds.getBottom(),
                     innerBevelBounds.getX(), innerBevelBounds.getBottom());

    pBR.addTriangle (outerBevelBounds.getRight(), innerBevelBounds.getBottom(),
                     outerBevelBounds.getRight(), outerBevelBounds.getBottom(),
                     innerBevelBounds.getRight(), innerBevelBounds.getBottom());
    
    g.saveState();

    g.setColour (baseColour);
    g.strokePath (pL, PathStrokeType (bevelThickness));
    g.strokePath (pR, PathStrokeType (bevelThickness));
    
    g.setColour (baseColour.darker (0.5f));
    g.strokePath (pT, PathStrokeType (bevelThickness, PathStrokeType::mitered, PathStrokeType::square));
    
    g.setColour (baseColour.brighter (0.5f));
    g.strokePath (pB, PathStrokeType (bevelThickness, PathStrokeType::mitered, PathStrokeType::square));
    
    g.setColour (baseColour);
    g.fillPath (pTL);
    g.fillPath (pTR);
    g.fillPath (pBL);
    g.fillPath (pBR);

    g.restoreState();
}

AudioPlaybackDemo::AudioPlaybackDemo (AudioFilePlayerExt& audioFilePlayer_, Buffer& distortionBuffer)
    : audioFilePlayer (audioFilePlayer_),
//      thumbnailCache (10),
//      audioThumbnail (128,
//                      *audioFilePlayer.getAudioFormatManager(),
//                      thumbnailCache),
      loopComponent (audioFilePlayer),
      backgroundThread ("Waveform Thread"),
      distortionComponent (distortionBuffer)
{
//    addAndMakeVisible (positionalDisplay = new ColouredPositionableWaveDisplay (&audioFilePlayer,
//                                                                                &thumbnailCache, 
//                                                                                &audioThumbnail));
//    addAndMakeVisible (draggableDisplay = new ColouredDraggableWaveDisplay (128,
//                                                                            &audioFilePlayer,
//                                                                            &thumbnailCache, 
//                                                                            &audioThumbnail));
    audioThumbnailImage = new AudioThumbnailImage (&audioFilePlayer, backgroundThread);
    
    positionableWaveDisplay = new PositionableWaveDisplay (*audioThumbnailImage);
    addAndMakeVisible (positionableWaveDisplay);
    
    draggableWaveDisplay = new DraggableWaveDisplay (*audioThumbnailImage);
    addAndMakeVisible (draggableWaveDisplay);
    
    addAndMakeVisible (&resolutionSlider);
    resolutionSlider.addListener (this);
    resolutionSlider.setRange (0, 20);
    resolutionSlider.setTextBoxStyle (Slider::NoTextBox, false, 50, 20);
    resolutionSlider.setValue (10);
    resolutionSlider.setSliderStyle (Slider::RotaryHorizontalDrag);
    
    addAndMakeVisible (&zoomSlider);
    zoomSlider.addListener (this);
    zoomSlider.setRange (0.0001, 2);
    zoomSlider.setTextBoxStyle (Slider::NoTextBox, false, 50, 20);
    zoomSlider.setValue (0.2);
    zoomSlider.setSliderStyle (Slider::RotaryVerticalDrag);
    
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
        playerControlLabels[i]->attachToComponent (playerControls[i], false);
        playerControls[i]->addListener (this);
        playerControls[i]->setValue (1.0);
        playerControls[i]->setSliderStyle (Slider::RotaryVerticalDrag);
        playerControls[i]->setTextBoxStyle (Slider::TextBoxBelow, false, 50, 16);
        
        Justification centreJustification (Justification::centred);
        playerControlLabels[i]->setJustificationType (centreJustification);
        playerControlLabels[i]->setColour (Label::textColourId, Colours::white);
    }
    
    playerControls[lowEQ]->setRange (0.00000001, 2, 0.001);
    playerControls[midEQ]->setRange (0.00000001, 2, 0.001);
    playerControls[highEQ]->setRange (0.00000001, 2, 0.001);

    playerControls[rate]->setRange (0.5, 1.5, 0.001);
    playerControls[tempo]->setRange (0.5, 1.5, 0.001);
    playerControls[pitch]->setRange (0.5, 1.5, 0.001);
    
    playerControlLabels[lowEQ]->setText ("Low EQ", false);
    playerControlLabels[midEQ]->setText ("Mid EQ", false);
    playerControlLabels[highEQ]->setText ("High EQ", false);
    playerControlLabels[rate]->setText ("Rate", false);
    playerControlLabels[tempo]->setText ("Tempo", false);
    playerControlLabels[pitch]->setText ("Pitch", false);

    playerControlLabels[lowEQ]->setFont (12);
    playerControlLabels[midEQ]->setFont (12);
    playerControlLabels[highEQ]->setFont (12);
    playerControlLabels[rate]->setFont (12);
    playerControlLabels[tempo]->setFont (12);
    playerControlLabels[pitch]->setFont (12);
    
    addAndMakeVisible (&distortionComponent);
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

    const int centre = w * 0.5;
    int offset = (centre - (80 * 3)) * 0.5;
    for (int i = 0; i < rate; i++)
    {
        playerControls[i]->setBounds (offset + i * 80 + 2, zoomSlider.getBottom() + 20 + 3 * m, 76, 76);
    }        

    offset += centre * 0.5;
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
    
    distortionComponent.setBounds (m, filterGroup.getBottom() + m, w * 0.5, (h - m) - (filterGroup.getBottom() + m));
}

void AudioPlaybackDemo::paint (Graphics& g)
{
    Rectangle<float> bevel1 (positionableWaveDisplay->getBounds().getX(), positionableWaveDisplay->getBounds().getY(),
                             positionableWaveDisplay->getBounds().getWidth(), positionableWaveDisplay->getBounds().getHeight());
    drawBevel (g, bevel1, 2.0f, Colours::darkgrey);
    
    Rectangle<float> bevel2 (draggableWaveDisplay->getBounds().getX(), draggableWaveDisplay->getBounds().getY(),
                             draggableWaveDisplay->getBounds().getWidth(), draggableWaveDisplay->getBounds().getHeight());
    drawBevel (g, bevel2, 2.0f, Colours::darkgrey);
}

void AudioPlaybackDemo::fileChanged (AudioFilePlayer* player)
{
}

void AudioPlaybackDemo::sliderValueChanged (Slider* slider)
{
    if (slider == &resolutionSlider)
    {
//        positionalDisplay->setResolution (resolutionSlider.getValue());
    }
    else if (slider == &zoomSlider)
    {
//        draggableDisplay->setHorizontalZoom (zoomSlider.getValue());
    }
    else if (slider == playerControls[lowEQ])
    {
        audioFilePlayer.getFilteringAudioSource()->setLowEQGain (playerControls[lowEQ]->getValue());
    }
    else if (slider == playerControls[midEQ])
    {
        audioFilePlayer.getFilteringAudioSource()->setMidEQGain (playerControls[midEQ]->getValue());
    }
    else if (slider == playerControls[highEQ])
    {
        audioFilePlayer.getFilteringAudioSource()->setHighEQGain (playerControls[highEQ]->getValue());
    }
    if (audioFilePlayer.getSoundTouchAudioSource() != nullptr)
    {
        if (slider == playerControls[rate])
        {
            SoundTouchProcessor::PlaybackSettings settings (audioFilePlayer.getSoundTouchAudioSource()->getPlaybackSettings());
            settings.rate = playerControls[rate]->getValue();
            audioFilePlayer.setPlaybackSettings (settings);
        }
        else if (slider == playerControls[tempo])
        {
            SoundTouchProcessor::PlaybackSettings settings (audioFilePlayer.getSoundTouchAudioSource()->getPlaybackSettings());
            settings.tempo = playerControls[tempo]->getValue();
            audioFilePlayer.setPlaybackSettings (settings);
        }
        else if (slider == playerControls[pitch])
        {
            SoundTouchProcessor::PlaybackSettings settings (audioFilePlayer.getSoundTouchAudioSource()->getPlaybackSettings());
            settings.pitch = playerControls[pitch]->getValue();
            audioFilePlayer.setPlaybackSettings (settings);
        }
    }
}

//==============================================================================
//void AudioPlaybackDemo::imageChanged (AudioThumbnailImage* audioThumbnailImage)
//{
//    //const ScopedLock sl (audioThumbnailImage->getLock());
//    thumbnailImage.setImage (audioThumbnailImage->getImage()/*AtTime (50, 60)*/, RectanglePlacement::stretchToFit);
//}
//
//void AudioPlaybackDemo::imageUpdated (AudioThumbnailImage* audioThumbnailImage)
//{
//    //const ScopedLock sl (audioThumbnailImage->getLock());
//    //const MessageManagerLock mm;
//    thumbnailImage.repaint();
//}
//
//void AudioPlaybackDemo::imageFinished (AudioThumbnailImage* audioThumbnailImage)
//{
//    //const ScopedLock sl (audioThumbnailImage->getLock());
//    //const MessageManagerLock mm;
//    thumbnailImage.repaint();
//}