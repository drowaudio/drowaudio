/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#ifndef __DROWAUDIO_WINDOW_H__
#define __DROWAUDIO_WINDOW_H__

#include "../dRowAudio_Buffer.h"
#include "../../utility/dRowAudio_Constants.h"
#include "../../maths/dRowAudio_MathsUtilities.h"

//==============================================================================
class Window
{
public:
    //==============================================================================
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
	
    //==============================================================================
	Window (int windowSize);

	~Window();

	void setWindowType (WindowType newType);
    
	WindowType getWindowType()              {	return windowType;          }
	
	float getWindowFactor()                 {	return windowFactor;        }
	
    float getOneOverWindowFactor()          {	return oneOverWindowFactor;	}
	
	void applyWindow (float *samples,  const int numSamples);
	
private:
    //==============================================================================
	void setUpWindowBuffer();
	
	void applyRectangularWindow (float *samples,  const int numSamples);
	void applyHannWindow (float *samples,  const int numSamples);
	void applyHammingWindow (float *samples,  const int numSamples);
	void applyCosineWindow (float *samples,  const int numSamples);
	void applyLanczosWindow (float *samples,  const int numSamples);
	void applyZeroEndTriangleWindow (float *samples,  const int numSamples);
	void applyNonZeroEndTriangleWindow (float *samples,  const int numSamples);
	void applyGaussianWindow (float *samples,  const int numSamples);
	void applyBartlettHannWindow (float *samples,  const int numSamples);
	void applyBlackmanWindow (float *samples,  const int numSamples);
	void applyNuttallWindow (float *samples,  const int numSamples);
	void applyBlackmanHarrisWindow (float *samples,  const int numSamples);
	void applyBlackmanNuttallWindow (float *samples,  const int numSamples);
	void applyFlatTopWindow (float *samples,  const int numSamples);
	
    //==============================================================================
	int windowSize;
	WindowType windowType;
	float windowFactor;
	float oneOverWindowFactor;
	
	Buffer windowBuffer;
    
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Window);
};

#endif //__DROWAUDIO_WINDOW_H__