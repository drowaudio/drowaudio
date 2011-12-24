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

#include "dRowAudio_Window.h"
#include "dRowAudio_FFTOperation.h"
#include "../../utility/dRowAudio_Constants.h"

//==============================================================================
class FFTEngine
{
public:
    //==============================================================================
	FFTEngine (int fftSizeLog2);
	
	~FFTEngine();

	/** Performs an FFT operation.
		The number of samples must be equal to the fftSize.
	 */
	void performFFT (float* samples);
	
	/**	This will fill the buffer with the magnitudes of the last performed FFT.
		You can then get this buffer using getMagnitudesBuffer(). Remember that
		the size of the buffer is the fftSizeHalved+1 to incorporate the Nyquist.
		Optionally you can pass in a Buffer to be filled. Remember this must be
		the correct size.
	 */
	void findMagnitudes (Buffer* bufferToFill = nullptr);

	/**	This will fill the buffer with the magnitudes of the last performed FFT if they are bigger.
		You can then get this buffer using getMagnitudesBuffer(). Remember that
		the size of the buffer is the fftSizeHalved + 1 to incorporate the Nyquist.
	 */
	void updateMagnitudesIfBigger();

	void setWindowType (Window::WindowType type);
	
	int getFFTSize()                        {	return getFFTProperties().fftSize;      }
	Buffer& getMagnitudesBuffer()           {	return magnitutes;                     }
	Window& getWindow()                     {	return windowProperties;               }
			
	const FFTProperties& getFFTProperties()	{	return fftOperation.getFFTProperties();	}

private:
    //==============================================================================
	FFTOperation fftOperation;
	Window windowProperties;
	Buffer magnitutes;
	
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FFTEngine);
};

#endif //__DROWAUDIO_FFTENGINE__