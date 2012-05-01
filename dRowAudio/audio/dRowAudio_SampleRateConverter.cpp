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
SampleRateConverter::SampleRateConverter (const int numChannels_)
    : ratio (1.0),
      numChannels (numChannels_)
{
    filterStates.calloc (numChannels);

    createLowPass (ratio);
    resetFilters();    
}

SampleRateConverter::~SampleRateConverter()
{
}

//==============================================================================
void SampleRateConverter::process (float** inputChannelData, const int numInputChannels, const int numInputSamples,
                                   float** outputChannelData, const int numOutputChannels, const int numOutputSamples)
{
    const int channelsToProcess = jmin (numInputChannels, numOutputChannels);
    const double localRatio = numInputSamples / (double) numOutputSamples;
        
    if (localRatio != ratio)
    {
        createLowPass (localRatio);
        ratio = localRatio;
    }
    
    if (localRatio > 1.0001)
    {
        // for down-sampling, pre-apply the filter..
        for (int i = channelsToProcess; --i >= 0;)
            applyFilter (inputChannelData[i], numInputSamples, filterStates[i]);
    }
    
    float nextSample = 0.0f;
    for (int s = 0; s < numOutputSamples; ++s)
    {
        for (int channel = 0; channel < channelsToProcess; ++channel)
            *outputChannelData[channel]++ = linearInterpolate (inputChannelData[channel], numInputSamples, nextSample);
        
        nextSample += (float) localRatio;
    }
    
    if (localRatio < 0.9999)
    {
        // for up-sampling, apply the filter after transposing..
        for (int i = channelsToProcess; --i >= 0;)
            applyFilter (inputChannelData[i], numInputSamples, filterStates[i]);
    }
    else if (localRatio <= 1.0001 && numInputSamples > 0)
    {
        // if the filter's not currently being applied, keep it stoked with the last couple of samples to avoid discontinuities
        for (int i = channelsToProcess; --i >= 0;)
        {
            const float* const endOfBuffer = &inputChannelData[i][numInputSamples - 1];
            FilterState& fs = filterStates[i];
            
            if (numInputSamples > 1)
            {
                fs.y2 = fs.x2 = *(endOfBuffer - 1);
            }
            else
            {
                fs.y2 = fs.y1;
                fs.x2 = fs.x1;
            }
            
            fs.y1 = fs.x1 = *endOfBuffer;
        }
    }
}
//==============================================================================
void SampleRateConverter::setFilterCoefficients (double c1, double c2, double c3, double c4, double c5, double c6)
{
    const double a = 1.0 / c4;
    
    c1 *= a;
    c2 *= a;
    c3 *= a;
    c5 *= a;
    c6 *= a;
    
    coefficients[0] = c1;
    coefficients[1] = c2;
    coefficients[2] = c3;
    coefficients[3] = c4;
    coefficients[4] = c5;
    coefficients[5] = c6;
}

void SampleRateConverter::createLowPass (const double frequencyRatio)
{
    const double proportionalRate = (frequencyRatio > 1.0) ? 0.5 / frequencyRatio
    : 0.5 * frequencyRatio;
    
    const double n = 1.0 / std::tan (double_Pi * jmax (0.001, proportionalRate));
    const double nSquared = n * n;
    const double c1 = 1.0 / (1.0 + std::sqrt (2.0) * n + nSquared);
    
    setFilterCoefficients (c1,
                           c1 * 2.0f,
                           c1,
                           1.0,
                           c1 * 2.0 * (1.0 - nSquared),
                           c1 * (1.0 - std::sqrt (2.0) * n + nSquared));
}

void SampleRateConverter::resetFilters()
{
    filterStates.clear (numChannels);
}

void SampleRateConverter::applyFilter (float* samples, int num, FilterState& fs)
{
    while (--num >= 0)
    {
        const double in = *samples;
        
        double out = coefficients[0] * in
        + coefficients[1] * fs.x1
        + coefficients[2] * fs.x2
        - coefficients[4] * fs.y1
        - coefficients[5] * fs.y2;
        
#if JUCE_INTEL
        if (! (out < -1.0e-8 || out > 1.0e-8))
            out = 0;
#endif
        
        fs.x2 = fs.x1;
        fs.x1 = in;
        fs.y2 = fs.y1;
        fs.y1 = out;
        
        *samples++ = (float) out;
    }
}

