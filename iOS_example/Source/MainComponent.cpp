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
    positionSlider.setColour (positionSlider.trackColourId, 
                              Colours::darkgreen.withAlpha (0.5f));
    
    audioPicker.addListener (this);
    audioConverter.addListener (this);
    
    startTimer (500);
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
    auto textBounds = positionSlider.getBounds();
    
    if (title != String())
        g.drawText (artist + " - " + title, textBounds, Justification::centred);
}

//==============================================================================
void MainComponent::timerCallback()
{
    auto trackLength = audioManager.getAudioFilePlayer().getLengthInSeconds();
    auto position = audioManager.getAudioFilePlayer().getCurrentPosition();
    
    auto sliderPosition = trackLength > 0.0 ? position / trackLength : 0.0;
    positionSlider.setValue (sliderPosition, NotificationType::dontSendNotification);
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
void MainComponent::audioPickerCancelled()
{
}

void MainComponent::audioPickerFinished (const Array<void*>& mpMediaItems)
{
    artist = AudioPicker::mpMediaItemToArtist (mpMediaItems[0]);
    title = AudioPicker::mpMediaItemToTitle (mpMediaItems[0]);
    
    auto url = AudioPicker::mpMediaItemToAvassetUrl (mpMediaItems[0]);
    
    const bool convertToWavBeforePlaying = false;
    
    if (convertToWavBeforePlaying)
    {
        audioConverter.startConversion (url); 
    }
    else
    {
        auto stream = AVAssetAudioFormat::avAssetUrlStringToStream (url);
        
        if (audioManager.getAudioFilePlayer().setMemoryInputStream (stream))
            audioManager.getAudioFilePlayer().startFromZero();
    }
}

void MainComponent::conversionFinished (const File& convertedFile)
{   
    if (audioManager.getAudioFilePlayer().setFile (convertedFile))
        audioManager.getAudioFilePlayer().startFromZero();
}
