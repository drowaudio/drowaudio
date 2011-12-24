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

#ifndef __DROWAUDIO_FFTOPERATION__
#define __DROWAUDIO_FFTOPERATION__

#if JUCE_MAC
    typedef FFTSetup FFTConfig;
    typedef DSPSplitComplex SplitComplex;
#endif

//==============================================================================
class FFTProperties
{
public:
    //==============================================================================
	FFTProperties (int fftSizeLog2_)
        : fftSizeLog2 (fftSizeLog2_),
		  fftSize (1L << fftSizeLog2),
		  fftSizeMinus1 (fftSize - 1),
		  fftSizeHalved (fftSize >> 1),
		  oneOverFFTSizeMinus1 (1.0 / fftSizeMinus1),
		  oneOverFFTSize (1.0 / fftSize)
	{
	}
	
    //==============================================================================
	long fftSizeLog2;
	long fftSize;
	long fftSizeMinus1;
	long fftSizeHalved;
	
	double oneOverFFTSizeMinus1;
	double oneOverFFTSize;	
	
private:
    //==============================================================================
	friend class FFTOperation;
	
	FFTProperties();
	
	void setFFTSizeLog2 (int newFFTSizeLog2)
	{
		fftSizeLog2 = newFFTSizeLog2;
		fftSize = (1L << fftSizeLog2);
		fftSizeMinus1 = (fftSize - 1);
		fftSizeHalved = (fftSize >> 1);
		oneOverFFTSizeMinus1 = (1.0 / fftSizeMinus1);
		oneOverFFTSize = (1.0 / fftSize);
	}
	
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTProperties);
};

//==============================================================================
/** Performs various FFT operations.
	This class wraps all the platform specific FFT operations.
 */
class FFTOperation
{
public:
    //==============================================================================
	FFTOperation (int fftSizeLog2);

	~FFTOperation();

	void setFFTSizeLog2 (int newFFTSize);
	
	const FFTProperties& getFFTProperties() {	return fftProperties;       }
	
	const SplitComplex& getFFTBuffer()      {	return fftBufferSplit;		}
	
	void performFFT (float* samples);
	
private:
    //==============================================================================
	FFTProperties fftProperties;
	HeapBlock<float> fftBuffer;
	
#if JUCE_MAC
	FFTConfig fftConfig;
	SplitComplex fftBufferSplit;
#endif //JUCE_MAC
	
	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTOperation);
};

/*
FFTSetup fftvDSP;
DSPSplitComplex fftBufferSplit;

// set-up the FFT pre-op. Pass FFTProperties, returns FFTSetup
fftvDSP = create_fftsetup (fftProperties.fftSizeLog2, 0);


// perform fft operation. Pass the audio samples split complex & properties
ctoz ((COMPLEX *) samples, 2, &fftBufferSplit, 1, fftProperties.fftSizeHalved);
fft_zrip (fftvDSP, &fftBufferSplit, 1, fftProperties.fftSizeLog2, FFT_FORWARD);

// need to call in desructor
destroy_fftsetup(fftvDSP);
*/
#endif //__DROWAUDIO_FFTOPERATION__