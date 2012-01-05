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

#include "BufferTransformAudioSource.h"

BufferTransformAudioSource::BufferTransformAudioSource (AudioSource* source_,
                                                        bool deleteSourceWhenDeleted)
    : source (source_, deleteSourceWhenDeleted),
      buffer (512)
{
    jassert (source_ != nullptr);
    
    const float xScale = 1.0f / (buffer.getSize() - 1);
    
    for (int i = 0; i < buffer.getSize(); ++i)
    {
        buffer.getReference (i) = i * xScale;
    }
}

BufferTransformAudioSource::~BufferTransformAudioSource()
{
}

void BufferTransformAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    source->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void BufferTransformAudioSource::releaseResources()
{
    source->releaseResources();
}

void BufferTransformAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    source->getNextAudioBlock (info);
    
    const int bufferSize = buffer.getSize();
    float** channelData = info.buffer->getArrayOfChannels();
    float sample;

    for (int c = 0; c < info.buffer->getNumChannels(); ++c)
    {
        for (int s = 0; s < info.numSamples; ++s)
        {
            sample = channelData[c][s];
            
            if (sample < 0.0f && sample > -1.0f)
            {
                sample *= -1.0f;
                sample = linearInterpolate (buffer.getData(), bufferSize, sample * bufferSize);
                sample *= -1.0f;
            }
            else if (sample > 0.0f && sample < 1.0f)
            {
                sample = linearInterpolate (buffer.getData(), bufferSize, sample * bufferSize);
            }
            
            channelData[c][s] = sample;
        }
    }
}
