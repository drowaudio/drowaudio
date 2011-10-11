/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-10 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

//==============================================================================
ReversibleAudioSource::ReversibleAudioSource (PositionableAudioSource* const inputSource,
											  const bool deleteInputWhenDeleted)
    : input (inputSource, deleteInputWhenDeleted),
	  isForwards(true)
{
    jassert (inputSource != 0);
}

ReversibleAudioSource::~ReversibleAudioSource()
{
}

void ReversibleAudioSource::prepareToPlay (int samplesPerBlockExpected,
										   double sampleRate)
{
	input->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void ReversibleAudioSource::releaseResources()
{
	input->releaseResources();
}

void ReversibleAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
	input->getNextAudioBlock(info);
	
	if (! isForwards) 
	{
		if (info.buffer->getNumChannels() == 1)
		{
			reverseArray (info.buffer->getSampleData(0), info.numSamples);
		}
		if (info.buffer->getNumChannels() == 2) 
		{
			reverseTwoArrays (info.buffer->getSampleData(0), info.buffer->getSampleData(1), info.numSamples);
		}
		else
		{
			for (int c = 0; c < info.buffer->getNumChannels(); c++)
				reverseArray (info.buffer->getSampleData(c), info.numSamples);
		}
		
		int64 nextReadPosition = input->getNextReadPosition() - (2 * info.numSamples);

		if (nextReadPosition < 0 && input->isLooping())
			nextReadPosition += input->getTotalLength();
		
		input->setNextReadPosition (nextReadPosition);
	}
}

END_JUCE_NAMESPACE