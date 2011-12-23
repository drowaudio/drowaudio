/*
  ==============================================================================

    FDNReverb.cpp
    Created: 14 Dec 2011 10:02:07am
    Author:  David Rowland

  ==============================================================================
*/

#include "FDNReverb.h"

FDNReverb::FDNReverb()
    : random (Time::currentTimeMillis()),
      currentSampleRate (44100.0)
{
    setParameters (Parameters());
    setSampleRate (44100.0);
    
    static int tapsMs[] = {13, 31, 47, 98, 136, 189}; 
    for (int c = 0; c < numChannels; ++c)
    {
        for (int t = 0; t < numElementsInArray (tapsMs); ++t)
            earlyReflections[c].addTap (tapsMs[t], 1.0f, 0.0f);
    }            
}

//==============================================================================
void FDNReverb::setParameters (const Parameters& newParams)
{
    for (int i = 0; i < numChannels; i++)
    {
        earlyReflections[i].rescaleTapPositions (newParams.earlyReflectionsTime);
        earlyReflections[i].rescaleTapGains (newParams.earlyRelectionsGain);
    }

    parameters = newParams;
}

//==============================================================================
void FDNReverb::setSampleRate (const double sampleRate)
{
    jassert (sampleRate > 0);
    currentSampleRate = sampleRate;
    
//    static const short combTunings[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 }; // (at 44100Hz)
//    static const short allPassTunings[] = { 556, 441, 341, 225 };
//    const int stereoSpread = 23;
//    const int intSampleRate = (int) sampleRate;
    
    for (int i = 0; i < numChannels; i++)
    {
        earlyReflections[i].setSampleRate (sampleRate);
        late1[i].setSampleRate (sampleRate);
    }        
}

void FDNReverb::reset()
{
    for (int j = 0; j < numChannels; ++j)
    {
    }
}

//==============================================================================
void FDNReverb::processStereo (float* const left, float* const right, const int numSamples) noexcept
{
    jassert (left != nullptr && right != nullptr);
    
    earlyReflections[0].processSamples (left, numSamples);
    earlyReflections[1].processSamples (right, numSamples);

    late1[0].processSamples (left, numSamples);
    late1[1].processSamples (right, numSamples);
}

void FDNReverb::processMono (float* const samples, const int numSamples) noexcept
{
    earlyReflections[0].processSamples (samples, numSamples);
    late1[0].processSamples (samples, numSamples);
}
