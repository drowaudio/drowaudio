/*
 *  dRowAudio_FFTEngine.h
 *  dRowAudio
 *
 *  Created by David Rowland on 20/07/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_FFTENGINE__
#define __DROWAUDIO_FFTENGINE__

#include "../../core/dRowAudio_StandardHeader.h"

#include "../../utility/dRowAudio_Constants.h"

class FFTEngine
{
public:
	FFTEngine(int fftSizeLog2);
	
	~FFTEngine();

	/** Performs an FFT operation.
		The number of samples must be equal to the fftSize.
	 */
	void performFFT(float* samples);
	
	int getFFTSize()	{	return fftProperties.fftSize;	}
	
	class FFTProperties
	{
	public:
		
		FFTProperties(int fftSizeLog2);
		
		const long fftSizeLog2;;
		const long fftSize;
		const long fftSizeMinus1;
		const long fftSizeHalved;
		
		const double oneOverFFTSizeMinus1;
		const double oneOverFFTSize;	
		
	private:
		
		FFTProperties();
	};
	
	class WindowProperties
	{
	public:
		WindowProperties();
		
		void applyWindow(float *samples, const FFTProperties &properties);
		
		float windowFactor;
		float oneOverWindowFactor;
	};
	
	const FFTProperties& getFFTProperties()	{	return fftProperties;	}

private:
	
	FFTProperties fftProperties;
	WindowProperties windowProperties;
	
	FFTSetup fftvDSP;
	DSPSplitComplex fftBufferSplit;

	HeapBlock<float> fftBuffer;
};

#endif //__DROWAUDIO_FFTENGINE__