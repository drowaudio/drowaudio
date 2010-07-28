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

#include "dRowAudio_Window.h"
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
	
	/**	This will fill the buffer with the magnitudes of the last performed FFT.
		You can then get this buffer using getMagnitudesBuffer(). Remember that
		the size of the buffer is the fftSizeHalved+1 to incorporate the Nyquist.
	 */
	void findMagnitudes();

	void setWindowType(Window::WindowType type);
	
	int getFFTSize()	{	return fftProperties.fftSize;	}
	Buffer* getMagnitudesBuffer()	{	return &magnitutes;	}
	Window* getWindow()				{	return &windowProperties;	}
	
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
		
	const FFTProperties& getFFTProperties()	{	return fftProperties;	}

private:
	
	FFTProperties fftProperties;
	Window windowProperties;
	
	FFTSetup fftvDSP;
	DSPSplitComplex fftBufferSplit;
	Buffer magnitutes;

	HeapBlock<float> fftBuffer;
};

#endif //__DROWAUDIO_FFTENGINE__