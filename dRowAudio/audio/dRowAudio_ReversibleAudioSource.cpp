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



//==============================================================================
ReversibleAudioSource::ReversibleAudioSource (PositionableAudioSource* const inputSource,
											  const bool deleteInputWhenDeleted)
    : input (inputSource, deleteInputWhenDeleted),
      previousReadPosition (0),
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
	if (isForwards) 
    {
        previousReadPosition = input->getNextReadPosition();
        input->getNextAudioBlock (info);
    }
    else
	{
        //int64 nextReadPosition = input->getNextReadPosition() - (2 * info.numSamples);
        int64 nextReadPosition = previousReadPosition - info.numSamples;
        
		if (nextReadPosition < 0 && input->isLooping())
			nextReadPosition += input->getTotalLength();
		
		input->setNextReadPosition (nextReadPosition);
        input->getNextAudioBlock (info);

		if (info.buffer->getNumChannels() == 1)
		{
			reverseArray (info.buffer->getSampleData (0) + info.startSample, info.numSamples);
		}
		else if (info.buffer->getNumChannels() == 2) 
		{
			reverseTwoArrays (info.buffer->getSampleData (0) + info.startSample,
                              info.buffer->getSampleData (1) + info.startSample,
                              info.numSamples);
		}
		else
		{
			for (int c = 0; c < info.buffer->getNumChannels(); c++)
				reverseArray (info.buffer->getSampleData(c) + info.startSample, info.numSamples);
		}
        
        previousReadPosition = nextReadPosition;
	}
}

