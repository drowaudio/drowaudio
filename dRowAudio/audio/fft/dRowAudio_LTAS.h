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

#ifndef __DROWAUDIO_LTAS_H__
#define __DROWAUDIO_LTAS_H__

#if JUCE_MAC || JUCE_IOS || DROWAUDIO_USE_FFTREAL || defined (DOXYGEN)

class CumulativeMovingAverage;

//==============================================================================
/** Calculates the Long Term Average Spectrum of a set of samples.
    
    This is a simple LTAS calculator that uses finds the spectrum of a block of
    audio samples and then computes the average weights across the number of
    FFTs performed.
 */
class LTAS
{
public:
    //==============================================================================
    /** Creates an LTAS.
        
        This will use a given FFT size, remember this is the log2 of the FFT size
        so 11 will be a 2048 point FFT.
     
        @see FFTEngine
     */
    LTAS (int fftSizeLog2);
    
    /** Destructor. */
    ~LTAS();
    
    /** Calculates the LTAS based on a set of samples.
        
        For this to work the number of samples must be at least as many as the size
        of the FFT.
     */
    void updateLTAS (float* input, int numSamples);
    
    /** Returns the computed LTAS buffer.
     
        This can be used to find pitch, tone information etc.
        
        @see Buffer
     */
    Buffer& getLTASBuffer()                    {   return ltasBuffer;  }
        
private:
    //==============================================================================
    FFTEngine fftEngine;
    Buffer ltasBuffer;
    const int fftSize, numBins;
    HeapBlock<float> tempBuffer;
    Array<CumulativeMovingAverage> ltasAvg;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LTAS);
};

#endif
#endif  // __DROWAUDIO_LTAS_H__