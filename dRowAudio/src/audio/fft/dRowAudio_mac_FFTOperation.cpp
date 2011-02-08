/*
 *  dRowAudio_FFTOperation.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 11/10/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */


#include "../../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_FFTOperation.h"

#ifdef JUCE_MAC

FFTOperation::FFTOperation(int fftSizeLog2)
:	fftProperties(fftSizeLog2)
{
	fftConfig = create_fftsetup (fftProperties.fftSizeLog2, 0);
	
	fftBuffer.malloc(fftProperties.fftSize);
	fftBufferSplit.realp = fftBuffer.getData();
	fftBufferSplit.imagp = fftBufferSplit.realp + getFFTProperties().fftSizeHalved;	
}

FFTOperation::~FFTOperation()
{
	destroy_fftsetup(fftConfig);
}

void FFTOperation::setFFTSizeLog2(int newFFTSizeLog2)
{
	if (newFFTSizeLog2 != fftProperties.fftSizeLog2) {
		destroy_fftsetup(fftConfig);
		
		fftProperties.setFFTSizeLog2(newFFTSizeLog2);
		fftBuffer.malloc(fftProperties.fftSize);
		fftBufferSplit.realp = fftBuffer.getData();
		fftBufferSplit.imagp = fftBufferSplit.realp + getFFTProperties().fftSizeHalved;	
		
		fftConfig = create_fftsetup (fftProperties.fftSizeLog2, 0);
	}
}

void FFTOperation::performFFT(float* samples)
{
	ctoz ((COMPLEX *) samples, 2, &fftBufferSplit, 1, fftProperties.fftSizeHalved);
	fft_zrip (fftConfig, &fftBufferSplit, 1, fftProperties.fftSizeLog2, FFT_FORWARD);
}


#endif //JUCE_MAC

//============================================================================

END_DROWAUDIO_NAMESPACE