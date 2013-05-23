/*
  ==============================================================================

    PitchDetectorComponent.cpp
    Created: 28 Dec 2012 5:52:04pm
    Author:  David Rowland

  ==============================================================================
*/

#include "PitchDetectorComponent.h"

//==============================================================================
PitchDetectorComponent::PitchDetectorComponent()
    : displayLogFrequency (false),
      sampleRate (44100.0),
      pitch (0.0),
      sampleBuffer (1, 512)
{
    addAndMakeVisible (&pitchLabel);
    pitchLabel.setColour (Label::textColourId, Colour::greyLevel (0.9f));
    
    startTimer (25);
}

PitchDetectorComponent::~PitchDetectorComponent()
{
}

void PitchDetectorComponent::setLogFrequencyDisplay (bool shouldDisplayLogFrequency)
{
    displayLogFrequency = shouldDisplayLogFrequency;
    repaint();
}

void PitchDetectorComponent::paint (Graphics& g)
{
    g.setColour (Colours::green);
    g.drawVerticalLine (pitchXCoord.getCurrent(), 0.0f, (float) getHeight());
}

void PitchDetectorComponent::resized()
{
    pitchLabel.setBounds (getLocalBounds().removeFromTop (20).removeFromRight (200));
}

void PitchDetectorComponent::timerCallback()
{
    pitchString = String (pitch, 2);
    pitchString << " Hz" << " (" << Pitch::fromFrequency (pitch).getMidiNoteName() << ")";
    pitchLabel.setText (pitchString, dontSendNotification);

    const double proportion = pitch / (sampleRate / 2);
    const int w = getWidth();
    pitchXCoord = roundToInt ((displayLogFrequency ? logBase10Scale (proportion, 1.0, 40.0) : proportion) * w);

    if (! pitchXCoord.areEqual())
    {
        repaint (pitchXCoord.getPrevious(), 0, 1, getHeight());
        repaint (pitchXCoord.getCurrent(), 0, 1, getHeight());
    }
}

//==============================================================================
void PitchDetectorComponent::setSampleRate (double newSampleRate)
{
    sampleRate = newSampleRate;
    
    const ScopedLock sl (detectorLock);
    pitchDetector.setSampleRate (sampleRate);
}

void PitchDetectorComponent::processBlock (const float* inputChannelData, int numSamples)
{
    const ScopedLock sl (detectorLock);
    pitchDetector.processSamples (inputChannelData, numSamples);
    pitch = pitchDetector.getPitch();
}
