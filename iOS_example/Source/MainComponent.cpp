/*
  ==============================================================================

    MainComponent.cpp
    Created: 8 Jun 2012 7:39:57am
    Author:  David Rowland

  ==============================================================================
*/

#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible (&pickButton);
    pickButton.setButtonText ("Pick");
    pickButton.addListener (this);
    
    addAndMakeVisible (&playButton);
    playButton.setButtonText ("Play");
    playButton.addListener (this);
    
    addAndMakeVisible (&positionSlider);
    positionSlider.setRange (0.0, 1.0);
    positionSlider.setSliderStyle (Slider::LinearBar);
    positionSlider.setTextBoxStyle (Slider::NoTextBox, false, 100, 20);
    positionSlider.addListener (this);
    
    audioPicker.addListener (this);
    
    startTimer (1000 / 10);
}

MainComponent::~MainComponent()
{
    audioPicker.removeListener (this);

    pickButton.removeListener (this);
    pickButton.removeListener (this);
    positionSlider.removeListener (this);
}

void MainComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    const int m = 5;
    
    if ((Desktop::getInstance().getCurrentOrientation()
        & (Desktop::upright | Desktop::upsideDown)) > 0)
    {
        pickButton.setBounds (m, m, (w / 2) - m, (h / 2) - m);
        playButton.setBounds ((w / 2) + m, m, (w / 2) - m, (h / 2) - m);
        
        positionSlider.setBounds (m, (h / 2) + m, w - (2 * m), (h / 2) - (2 * m));
    }
    else
    {
        pickButton.setBounds (m, m, (w / 2) - m, (h / 2) - m);
        playButton.setBounds ((w / 2) + m, m, (w / 2) - m, (h / 2) - m);
        
        positionSlider.setBounds (m, (h / 2) + m, w - (2 * m), (h / 2) - (2 * m));
    }
}

void MainComponent::paint (Graphics& g)
{
}

//==============================================================================
void MainComponent::timerCallback()
{
    const double trackLength = audioManager.getAudioFilePlayer().getLengthInSeconds();
    
    if (trackLength > 0.0)
    {
        const double position = audioManager.getAudioFilePlayer().getCurrentPosition()
                                  / trackLength;
        
        positionSlider.setValue (position, false);
    }
    else
    {
        positionSlider.setValue (0.0, false);
    }
}

void MainComponent::buttonClicked (Button* button)
{
    if (button == &pickButton)
    {
        audioPicker.show();
    }
    else if (button == &playButton)
    {
        audioManager.getAudioFilePlayer().pause();
    }
}

void MainComponent::sliderValueChanged (Slider* slider)
{
    if (slider == &positionSlider)
    {
        const double newPosition = positionSlider.getValue()
                                    * audioManager.getAudioFilePlayer().getLengthInSeconds();
        
        audioManager.getAudioFilePlayer().setPosition (newPosition);
    }
}

//==============================================================================
void MainComponent::audioPickerFinished (const Array<void*> mpMediaItems)
{
    const bool shouldBePlaying = audioManager.getAudioFilePlayer().isPlaying();
    
    const String avAssetUrlString (AudioPicker::mpMediaItemToAvassetUrl (mpMediaItems[0]));
    audioManager.getAudioFilePlayer().setMemoryInputStream (AVAssetAudioFormat::avAssetUrlStringToStream (avAssetUrlString));
    
    if (shouldBePlaying)
        audioManager.getAudioFilePlayer().start();
}

void MainComponent::audioPickerCancelled()
{
}
