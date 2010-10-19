/*
 *  dRowAudio_Window.h
 *  dRowAudio
 *
 *  Created by David Rowland on 24/07/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_WINDOW_H__
#define __DROWAUDIO_WINDOW_H__

#include "../../core/dRowAudio_StandardHeader.h"

#include "../dRowAudio_Buffer.h"
#include "../../utility/dRowAudio_Constants.h"
#include "../../maths/dRowAudio_MathsUtilities.h"

class Window
{
public:
	
	enum WindowType {
		Rectangular,
		Hann,
		Hamming,
		Cosine,
		Lanczos,
		ZeroEndTriangle,
		NonZeroEndTriangle,
		Gaussian,
		BartlettHann,
		Blackman,
		Nuttall,
		BlackmanHarris,
		BlackmanNuttall,
		FlatTop
	};
	
	Window(int windowSize);

	~Window()
	{}

	void setWindowType(WindowType newType);
	WindowType getWindowType()		{	return windowType;	}
	
	float getWindowFactor()			{	return windowFactor;	}
	float getOneOverWindowFactor()	{	return oneOverWindowFactor;	}
	
	void applyWindow(float *samples,  const int numSamples);
	
	
private:
	
	void setUpWindowBuffer();
	
	void applyRectangularWindow(float *samples,  const int numSamples);
	void applyHannWindow(float *samples,  const int numSamples);
	void applyHammingWindow(float *samples,  const int numSamples);
	void applyCosineWindow(float *samples,  const int numSamples);
	void applyLanczosWindow(float *samples,  const int numSamples);
	void applyZeroEndTriangleWindow(float *samples,  const int numSamples);
	void applyNonZeroEndTriangleWindow(float *samples,  const int numSamples);
	void applyGaussianWindow(float *samples,  const int numSamples);
	void applyBartlettHannWindow(float *samples,  const int numSamples);
	void applyBlackmanWindow(float *samples,  const int numSamples);
	void applyNuttallWindow(float *samples,  const int numSamples);
	void applyBlackmanHarrisWindow(float *samples,  const int numSamples);
	void applyBlackmanNuttallWindow(float *samples,  const int numSamples);
	void applyFlatTopWindow(float *samples,  const int numSamples);
	
	int windowSize;
	WindowType windowType;
	float windowFactor;
	float oneOverWindowFactor;
	
	Buffer windowBuffer;
};

#endif //__DROWAUDIO_WINDOW_H__