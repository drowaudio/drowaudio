/*
 *  dRowAudio_FFTEngine.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 20/07/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "../../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_FFTEngine.h"

inline float magnitude(const float real, 
					   const float imag, 
					   const float oneOverFFTSize, 
					   const float oneOverWindowFactor)
{
	const float rawMagnitude = hypotf(real, imag);
	const float magnitudeForFFTSize = rawMagnitude * oneOverFFTSize;
	const float magnitudeForWindowFactor = magnitudeForFFTSize * oneOverWindowFactor;
	
	return magnitudeForWindowFactor;
}

//============================================================================
FFTEngine::FFTEngine(int fftSizeLog2_)
:	fftProperties(fftSizeLog2_)
{
	fftvDSP = create_fftsetup (fftProperties.fftSizeLog2, 0);

	fftBuffer.malloc(fftProperties.fftSize);
}

void FFTEngine::performFFT(float* samples)
{
	fftBufferSplit.realp = fftBuffer;
	fftBufferSplit.imagp = fftBufferSplit.realp + fftProperties.fftSizeHalved;
	
	windowProperties.applyWindow(samples, fftProperties);
	
	// perform fft operation
	ctoz ((COMPLEX *) samples, 2, &fftBufferSplit, 1, fftProperties.fftSizeHalved);
	fft_zrip (fftvDSP, &fftBufferSplit, 1, fftProperties.fftSizeLog2, FFT_FORWARD);
	
	// find magnitudes
	printf("Magnitudes FFT:\n");
	float magDC = magnitude(fftBufferSplit.realp[0], 
							0.0,					// imag for DC is always zero 
							fftProperties.oneOverFFTSize, windowProperties.oneOverWindowFactor); 
	printf("bin[ DC] = %10f\n", magDC);
	for(int i = 1; i < fftProperties.fftSizeHalved; i++)
	{		
		float magBin = magnitude(fftBufferSplit.realp[i], 
								 fftBufferSplit.imagp[i], 
								 fftProperties.oneOverFFTSize, windowProperties.oneOverWindowFactor);
		printf("bin[%3d] = %10f\n", i, magBin);
	}
	float magNyquist = magnitude(fftBufferSplit.imagp[0], 
								 0.0,			// imag for Nyquist is always zero 
								 fftProperties.oneOverFFTSize, windowProperties.oneOverWindowFactor); 
	printf("bin[NYQ] = %10f\n", magNyquist);	
	printf("\n");
	
}

FFTEngine::~FFTEngine()
{
	destroy_fftsetup(fftvDSP);
}

//============================================================================
FFTEngine::FFTProperties::FFTProperties(int fftSizeLog2_)
:	fftSizeLog2(fftSizeLog2_),
	fftSize(1L << fftSizeLog2),
	fftSizeMinus1(fftSize-1),
	fftSizeHalved(fftSize >> 1),
	oneOverFFTSizeMinus1(1.0 / fftSizeMinus1),
	oneOverFFTSize(1.0 / fftSize)
{
}

//============================================================================
FFTEngine::WindowProperties::WindowProperties()
{
}

void FFTEngine::WindowProperties::applyWindow(float *samples, const FFTProperties &properties)
{
	printf("Apply Hann window to audio buffer:\n");
	windowFactor = 0.0;
	for(int i = 0; i < properties.fftSize; i++)
	{
		// Hann window equation
		float window = 0.5 * (1.0 - cos((double)i * properties.oneOverFFTSizeMinus1 * twoTimesPi));
		samples[i] *= window;
		windowFactor += window;
	}
	windowFactor *= properties.oneOverFFTSize;
	oneOverWindowFactor = 1.0 / windowFactor;
}

END_DROWAUDIO_NAMESPACE