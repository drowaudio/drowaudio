/*
  ==============================================================================

    MultitapDelayLine.cpp
    Created: 14 Dec 2011 10:24:48am
    Author:  David Rowland

  ==============================================================================
*/

#include "MultitapDelayLine.h"

MultitapDelayLine::MultitapDelayLine()
    : sampleRate (44100.0),
      bufferSize ((int) sampleRate),
      bufferWritePos (0)
{
    buffer.calloc (bufferSize);
}

MultitapDelayLine::~MultitapDelayLine()
{
}

void MultitapDelayLine::addTap (double tapDelayMs, float outputGain, float feedbackGain)
{
    int sampleForTap = tapDelayMs * 0.001 * sampleRate;
    jassert (sampleForTap < bufferSize);

    Tap tap = {sampleForTap, sampleForTap,
               outputGain, outputGain,
               feedbackGain, feedbackGain};
    
    taps.add (tap);
}

void MultitapDelayLine::setBufferSize (int numSamples)
{
    buffer.realloc (numSamples);
}

void MultitapDelayLine::setSampleRate (double newSampleRate)
{
    jassert (newSampleRate > 0.0);
    
    rescaleTapPositions (sampleRate / newSampleRate);
    sampleRate = newSampleRate;
}

//==============================================================================
void MultitapDelayLine::rescaleTapPositions (double ratio)
{
    for (int i = 0; i < taps.size(); ++i)
    {
        Tap& tap = taps.getReference (i);
        tap.delaySamples = tap.originalDelaySamples * ratio;
    }
}

void MultitapDelayLine::rescaleTapFeedbacks (double ratio)
{
    for (int i = 0; i < taps.size(); ++i)
    {
        Tap& tap = taps.getReference (i);
        tap.feedback = tap.originalFeedback * ratio;
    }    
}

void MultitapDelayLine::rescaleTapGains (double ratio)
{
    for (int i = 0; i < taps.size(); ++i)
    {
        Tap& tap = taps.getReference (i);
        tap.gain = tap.originalGain * ratio;
    }    
}

//==============================================================================
float MultitapDelayLine::processSingleSample (float newSample) noexcept
{
    bufferWritePos = ++bufferWritePos % bufferSize;
    
    float out = newSample;
    float in = newSample;
    
    for (int i = 0; i < taps.size(); ++i)
    {
        const Tap& tap = taps.getReference (i);
        int readPos = bufferWritePos - tap.delaySamples;
        if (readPos < 0)
            readPos += bufferSize;
        in += buffer[readPos] * tap.feedback;
        out += buffer[readPos] * tap.gain;
    }

    buffer[bufferWritePos] = in;
    
    return out;
}

void MultitapDelayLine::processSamples (float* const samples, const int numSamples) noexcept
{
    for (int i = 0; i < numSamples; ++i)
    {
        samples[i] = processSingleSample (samples[i]);
    }
}

//==============================================================================
