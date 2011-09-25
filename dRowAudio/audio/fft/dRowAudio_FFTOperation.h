/*
 *  dRowAudio_FFTOperation.h
 *  dRowAudio
 *
 *  Created by David Rowland on 11/10/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_FFTOPERATION__
#define __DROWAUDIO_FFTOPERATION__

#include "../../core/dRowAudio_StandardHeader.h"

class FFTProperties
{
public:
	
	FFTProperties(int fftSizeLog2_)
	:	fftSizeLog2(fftSizeLog2_),
		fftSize(1L << fftSizeLog2),
		fftSizeMinus1(fftSize-1),
		fftSizeHalved(fftSize >> 1),
		oneOverFFTSizeMinus1(1.0 / fftSizeMinus1),
		oneOverFFTSize(1.0 / fftSize)
	{
	}
	
	long fftSizeLog2;;
	long fftSize;
	long fftSizeMinus1;
	long fftSizeHalved;
	
	double oneOverFFTSizeMinus1;
	double oneOverFFTSize;	
	
private:
	
	friend class FFTOperation;
	
	FFTProperties();
	
	void setFFTSizeLog2(int newFFTSizeLog2)
	{
		fftSizeLog2 = newFFTSizeLog2;
		fftSize = (1L << fftSizeLog2);
		fftSizeMinus1 = (fftSize-1);
		fftSizeHalved = (fftSize >> 1);
		oneOverFFTSizeMinus1 = (1.0 / fftSizeMinus1);
		oneOverFFTSize = (1.0 / fftSize);
	}
	
	JUCE_LEAK_DETECTOR (FFTProperties);
};


/** Performs various FFT operations.
	This class wraps all the platform specific FFT operations.
 */
class FFTOperation
{
public:
	FFTOperation(int fftSizeLog2);

	~FFTOperation();

	void setFFTSizeLog2(int newFFTSize);
	
	const FFTProperties& getFFTProperties()	{	return fftProperties;	}
	
	const DSPSplitComplex& getFFTBuffer()	{	return fftBufferSplit;		}
	
	void performFFT(float* samples);
	
private:
	FFTProperties fftProperties;
	HeapBlock<float> fftBuffer;
	
#ifdef JUCE_MAC
	FFTSetup fftConfig;
	DSPSplitComplex fftBufferSplit;
#endif //JUCE_MAC
	
	JUCE_LEAK_DETECTOR (FFTOperation);
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