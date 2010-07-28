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
:	fftProperties(fftSizeLog2_),
	windowProperties(fftProperties.fftSize),
	magnitutes(fftProperties.fftSizeHalved+1)
{
	fftvDSP = create_fftsetup (fftProperties.fftSizeLog2, 0);

	fftBuffer.malloc(fftProperties.fftSize);
	fftBufferSplit.realp = fftBuffer.getData();
	fftBufferSplit.imagp = fftBufferSplit.realp + fftProperties.fftSizeHalved;
}

void FFTEngine::performFFT(float* samples)
{	
	// first apply the current window
	windowProperties.applyWindow(samples, fftProperties.fftSize);
	
	// perform fft operation
	ctoz ((COMPLEX *) samples, 2, &fftBufferSplit, 1, fftProperties.fftSizeHalved);
	fft_zrip (fftvDSP, &fftBufferSplit, 1, fftProperties.fftSizeLog2, FFT_FORWARD);
}

void FFTEngine::findMagnitudes()
{
	// find magnitudes
	magnitutes.getReference(0) = magnitude(fftBufferSplit.realp[0], 
										   0.0,					// imag for DC is always zero 
										   fftProperties.oneOverFFTSize, windowProperties.getOneOverWindowFactor()); 
	for(int i = 1; i < fftProperties.fftSizeHalved; i++)
	{		
		magnitutes.getReference(i) = magnitude(fftBufferSplit.realp[i], 
											   fftBufferSplit.imagp[i], 
											   fftProperties.oneOverFFTSize, windowProperties.getOneOverWindowFactor());
	}
	magnitutes.getReference(fftProperties.fftSizeHalved) = magnitude(fftBufferSplit.realp[0], 
																	 0.0,					// imag for Nyquist is always zero 
																	 fftProperties.oneOverFFTSize, windowProperties.getOneOverWindowFactor());	
}

void FFTEngine::setWindowType(Window::WindowType type)
{
	windowProperties.setWindowType(type);
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

END_DROWAUDIO_NAMESPACE