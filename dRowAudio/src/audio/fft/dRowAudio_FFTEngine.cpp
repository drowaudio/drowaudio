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

void FFTEngine::findMagnitudes(Buffer *bufferToFill)
{
	// local copies for speed
	float* magBuf;
	if (bufferToFill == 0)
		magBuf = magnitutes.getData();
	else
		magBuf = bufferToFill->getData();

	const DSPSplitComplex &fftSplit = fftOperation.getFFTBuffer();
	const double oneOverFFTSize = getFFTProperties().oneOverFFTSize;
	const int fftSizeHalved = getFFTProperties().fftSizeHalved;
	const int oneOverWindowFactor = windowProperties.getOneOverWindowFactor();
	
	// find magnitudes
	magBuf[0] = magnitude(fftSplit.realp[0], 0.0, oneOverFFTSize, oneOverWindowFactor); // imag for DC is always zero 
	for(int i = 1; i < fftSizeHalved; i++)
	{		
		magBuf[i] = magnitude(fftSplit.realp[i], fftSplit.imagp[i], oneOverFFTSize, oneOverWindowFactor);
	}
	magBuf[fftSizeHalved] = magnitude(fftSplit.realp[0], 0.0, oneOverFFTSize, oneOverWindowFactor); // imag for Nyquist is always zero 
	
	magnitutes.updateListeners();
}

void FFTEngine::updateMagnitudesIfBigger()
{
	// local copies for speed
	const DSPSplitComplex &fftSplit = fftOperation.getFFTBuffer();
	float* magBuf = magnitutes.getData();
	const double oneOverFFTSize = getFFTProperties().oneOverFFTSize;
	const int fftSizeHalved = getFFTProperties().fftSizeHalved;
	const int oneOverWindowFactor = windowProperties.getOneOverWindowFactor();
	
	// find magnitudes
	float newMag = magnitude(fftSplit.realp[0], 0.0, oneOverFFTSize, oneOverWindowFactor); // imag for DC is always zero 
	if (newMag > magBuf[0]) {
		magBuf[0] = newMag;
	}
	for(int i = 1; i < fftSizeHalved; i++)
	{		
		newMag = magnitude(fftSplit.realp[i], fftSplit.imagp[i], oneOverFFTSize, oneOverWindowFactor);
		if(newMag > magBuf[i])
			magBuf[i] = newMag;
	}
	newMag = magnitude(fftSplit.realp[0], 0.0, oneOverFFTSize, oneOverWindowFactor); // imag for Nyquist is always zero 
	if(newMag > magBuf[fftSizeHalved])
		magBuf[fftSizeHalved] = newMag;
	
	magnitutes.updateListeners();
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