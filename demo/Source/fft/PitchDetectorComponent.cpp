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

#include "PitchDetectorComponent.h"

PitchDetectorComponent::PitchDetectorComponent() :
    displayLogFrequency (false),
    sampleRate (44100.0),
    pitch (0.0),
    sampleBuffer (1, 512)
{
    addAndMakeVisible (&pitchLabel);
    pitchLabel.setColour (Label::textColourId, Colour::greyLevel (0.9f));

    startTimer (25);
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
    pitchString = juce::String (pitch, 2);
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
