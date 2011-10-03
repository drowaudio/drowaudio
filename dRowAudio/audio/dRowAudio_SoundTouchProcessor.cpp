/*
 *  dRowAuidio_SoundTouchProcessor.cpp
 *
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

SoundTouchProcessor::SoundTouchProcessor()
    : interleavedInputBufferSize (512),
      interleavedOutputBufferSize (512)
{
    settings.rate = 1.0f;
    settings.tempo = 1.0f;
    settings.pitch = 1.0f;
    
    setPlaybackSettings (settings);
    
    interleavedInputBuffer.malloc (interleavedInputBufferSize * 2);
    interleavedOutputBuffer.malloc (interleavedOutputBufferSize * 2);
}

SoundTouchProcessor::~SoundTouchProcessor()
{
}

void SoundTouchProcessor::initialise (int numChannels, double sampleRate)
{
    soundTouch.setChannels (numChannels);
    soundTouch.setSampleRate (sampleRate);
    soundTouch.clear();
}

void SoundTouchProcessor::writeSamples (float** sourceChannelData, int numChannels, int numSamples, int startSampleOffset)
{
    const int requiredBufferSize = numSamples * numChannels;
    if (interleavedInputBufferSize < requiredBufferSize) 
    {
        interleavedInputBuffer.malloc (requiredBufferSize);
        interleavedInputBufferSize = requiredBufferSize;
    }
    
    for (int i = 0; i < numChannels; i++)
        sourceChannelData[i] += startSampleOffset;
    
    AudioDataConverters::interleaveSamples ((const float**)sourceChannelData, interleavedInputBuffer,                                        
                                            numSamples, numChannels);
    
    for (int i = 0; i < numChannels; i++)
        sourceChannelData[i] -= startSampleOffset;
    
    soundTouch.putSamples ((SAMPLETYPE*)interleavedInputBuffer, numSamples);
}

void SoundTouchProcessor::readSamples (float** destinationChannelData, int numChannels, int numSamples, int startSampleOffset)
{
    const int requiredBufferSize = numSamples * numChannels;
    if (interleavedOutputBufferSize < requiredBufferSize) 
    {
        interleavedOutputBuffer.malloc (requiredBufferSize);
        interleavedOutputBufferSize = requiredBufferSize;
    }
    
    int numSamplesDone = 0;
    int numThisTime = 0;
    do
    {
        int maxNumSamples = numSamples - numSamplesDone;
        numThisTime = soundTouch.receiveSamples ((SAMPLETYPE*)&interleavedOutputBuffer[numChannels * numSamplesDone], maxNumSamples);
        
        numSamplesDone += numThisTime;
        
        if (numSamplesDone == numSamples)
            break;
    }
    while (numThisTime != 0);
    
    if (numSamplesDone < numSamples)
        zeromem (&interleavedOutputBuffer[numChannels * numSamplesDone], numChannels * sizeof (numSamples - numSamplesDone));
    
    for (int i = 0; i < numChannels; i++)
        destinationChannelData[i] += startSampleOffset;
    
    AudioDataConverters::deinterleaveSamples (interleavedOutputBuffer, destinationChannelData,                                        
                                              numSamples, numChannels);
    for (int i = 0; i < numChannels; i++)
        destinationChannelData[i] -= startSampleOffset;
}

void SoundTouchProcessor::setPlaybackSettings (PlaybackSettings newSettings)
{
    settings = newSettings;
    
    soundTouch.setRate (settings.rate);
    soundTouch.setTempo (settings.tempo);
    soundTouch.setPitch (settings.pitch);    
}

END_JUCE_NAMESPACE