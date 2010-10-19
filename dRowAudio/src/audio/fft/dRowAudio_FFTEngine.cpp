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
:	fftOperation(fftSizeLog2_),
	windowProperties(getFFTProperties().fftSize),
	magnitutes(getFFTProperties().fftSizeHalved+1)
{
}

void FFTEngine::performFFT(float* samples)
{	
	// first apply the current window
	windowProperties.applyWindow(samples, getFFTProperties().fftSize);
	fftOperation.performFFT(samples);
}

void FFTEngine::findMagnitudes()
{
	const DSPSplitComplex &fftSplit = fftOperation.getFFTBuffer();
	
	// find magnitudes
	magnitutes.getReference(0) = magnitude(fftSplit.realp[0], 
										   0.0,					// imag for DC is always zero 
										   getFFTProperties().oneOverFFTSize, windowProperties.getOneOverWindowFactor()); 
	for(int i = 1; i < getFFTProperties().fftSizeHalved; i++)
	{		
		magnitutes.getReference(i) = magnitude(fftSplit.realp[i], 
											   fftSplit.imagp[i], 
											   getFFTProperties().oneOverFFTSize, windowProperties.getOneOverWindowFactor());
	}
	magnitutes.getReference(getFFTProperties().fftSizeHalved) = magnitude(fftSplit.realp[0], 
																		  0.0,					// imag for Nyquist is always zero 
																		  getFFTProperties().oneOverFFTSize,
																		  windowProperties.getOneOverWindowFactor());	
}

void FFTEngine::setWindowType(Window::WindowType type)
{
	windowProperties.setWindowType(type);
}

FFTEngine::~FFTEngine()
{
}

//============================================================================

END_DROWAUDIO_NAMESPACE