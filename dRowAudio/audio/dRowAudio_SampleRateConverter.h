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

#ifndef __DROWAUDIO_SAMPLERATECONVERTER_H__
#define __DROWAUDIO_SAMPLERATECONVERTER_H__

//==============================================================================
/**
    Simple sample rate converter class.
 
    This converts a block of samples from one sample rate to another. It is
    based on a linear interpolation algorithm.
    To use it simply create one with the desired number of channels and then
    repeatedly call its process() method. The sample ratio is based on the
    difference in input and output buffer sizes so for example to convert a
    44.1KHz signal to a 22.05KHz one you could pass in buffers with sizes 512
    and 256 respectively.
 */
class SampleRateConverter
{
public:
    //==============================================================================
    /** Creates a SampleRateConverter with a given number of channels.
     */
    SampleRateConverter (const int numChannels = 1);

    /** Destructor.
     */
    ~SampleRateConverter();
    
    /** Performs the conversion.
        The minimum number of channels will be processed here so it is a good idea
        to make sure that the number of input channels is equal to the number of
        output channels. The input channel data is filtered during this process so
        if you don't want to lose it then make a copy before calling this method.
     */
    void process (float** inputChannelData, const int numInputChannels, const int numInputSamples,
                  float** outputChannelData, const int numOutputChannels, const int numOutputSamples);

private:
    //==============================================================================
    double ratio;
    double coefficients[6];
    const int numChannels;
    
    void setFilterCoefficients (double c1, double c2, double c3, double c4, double c5, double c6);
    void createLowPass (double proportionalRate);
    
    struct FilterState
    {
        double x1, x2, y1, y2;
    };
    
    HeapBlock<FilterState> filterStates;
    void resetFilters();
    
    void applyFilter (float* samples, int num, FilterState& fs);
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SampleRateConverter);
};


#endif  // __SAMPLERATECONVERTER_H_65500721__
