/*
  ==============================================================================

    FeedbackMatrixBuffer.cpp
    Created: 14 Dec 2011 10:25:48am
    Author:  David Rowland

  ==============================================================================
*/

#include "FeedbackMatrixBuffer.h"

//==============================================================================
namespace {
    const float matrix[7][7] = {
        {0, -1, -1, 1, -1, 1, 1},
        {1, 0, -1, -1, 1, -1, 1},
        {1, 1, 0, -1, -1, 1, -1},
        {-1, 1, 1, 0, -1, -1, 1},
        {1, -1, 1, 1, 0, -1, -1},
        {-1, 1, -1, 1, 1, 0, -1},
        {-1, -1, 1, -1, 1, 1, 0}
    };
    
    const float delayTimes[8] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
    
    const float defaultGains[8] = {0.837429, 0.2364782348, 0.2378462836, 0.6584727492, 0.1837847293, 0.782467823468, 0.1284648723, 0.742834568};
}
//==============================================================================
FeedbackMatrixBuffer::FeedbackMatrixBuffer()
{
    setSampleRate (44100);
    rescaleGains (1.0f);
}

FeedbackMatrixBuffer::~FeedbackMatrixBuffer()
{
}

//==============================================================================
void FeedbackMatrixBuffer::setSampleRate (double newSampleRate)
{
    for (int i = 0; i < numPaths; ++i)
    {
        bufferSize = (int) newSampleRate;
        buffers[i].malloc (bufferSize);
        bufferIndex = 0;
    }
    
    rescaleDelayTimes (newSampleRate / sampleRate);
    
    sampleRate = newSampleRate;
}

void FeedbackMatrixBuffer::rescaleDelayTimes (double ratio)
{
    ratio *= (sampleRate / 44100);

    for (int i = 0; i < numPaths; ++i)
    {
        delays[i] = delayTimes[i] * ratio;
    }
}

void FeedbackMatrixBuffer::rescaleGains (double ratio)
{
    for (int i = 0; i < numPaths; ++i)
    {
        gains[i] = defaultGains[i] * ratio * 0.9;
    }    
}

float FeedbackMatrixBuffer::processSingleSample (float newSample) noexcept
{
    float mIns[numPaths], mOuts[numPaths];
    float out = 0.0f;
    
    // read outputs
    for (int i = 0; i < numPaths; ++i)
    {
        const int readPos = (bufferIndex - delays[i]) % bufferSize;
        mIns[i] = buffers[i][readPos] * gains[i]; //also filter here
        out += mIns[i];
    }

    //apply matrix
    for (int x = 0; x < numPaths; ++x)
    {
        mOuts[x] = 0.0;
        
        for (int y = 0; y < numPaths; ++y)
            mOuts[x] += matrix[x][y] * mIns[y];
    }
    
    // write into buffers
    for (int i = 0; i < numPaths; ++i)
    {
        buffers[i][bufferIndex] = newSample + mOuts[i];
    }
    
    bufferIndex = ++bufferIndex % bufferSize;
    
    return out;
}

void FeedbackMatrixBuffer::processSamples(float* const samples,
                                          const int numSamples) noexcept
{
    for (int i = 0; i < numSamples; ++i)
    {
        samples[i] = processSingleSample (samples[i]);
    }
}

//==============================================================================
/*FeedbackMatrixBuffer::DelayBlock::DelayBlock()
    : gain (0.0f),
      bufferSize (0),
      bufferIndex (0)
{
}

void FeedbackMatrixBuffer::DelayBlock::setSize (int size)
{
    if (size != bufferSize)
    {
        bufferIndex = 0;
        buffer.malloc ((size_t) size);
        bufferSize = size;
        buffer.clear ((size_t) bufferSize);
    }
}

void FeedbackMatrixBuffer::setGain (float newGain)
{
    gain = newGain;
}

float FeedbackMatrixBuffer::DelayBlock::process (const float input)
{
    float out = buffer[bufferIndex];
    
    bufferIndex = ++bufferIndex % bufferSize;
    buffer[bufferIndex] = input;
    
    return gain * out;
}
*/
//==============================================================================
