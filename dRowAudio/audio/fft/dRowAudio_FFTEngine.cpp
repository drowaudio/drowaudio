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

#if JUCE_MAC || JUCE_IOS || DROWAUDIO_USE_FFTREAL



//============================================================================
namespace 
{
    inline float magnitude (const float real, 
                            const float imag, 
                            const float oneOverFFTSize, 
                            const float oneOverWindowFactor)
    {
        const float rawMagnitude = hypotf (real, imag);
        const float magnitudeForFFTSize = rawMagnitude * oneOverFFTSize;
        const float magnitudeForWindowFactor = magnitudeForFFTSize * oneOverWindowFactor;
        
        return magnitudeForWindowFactor;
    }
}

//============================================================================
FFTEngine::FFTEngine (int fftSizeLog2_)
    : fftOperation (fftSizeLog2_),
      windowProperties (getFFTProperties().fftSize),
      magnitutes (getFFTProperties().fftSizeHalved + 1)
{
}

FFTEngine::~FFTEngine()
{
}

void FFTEngine::performFFT (float* samples)
{	
	// first apply the current window
	windowProperties.applyWindow (samples, getFFTProperties().fftSize);
	fftOperation.performFFT (samples);
}

void FFTEngine::findMagnitudes (Buffer* bufferToFill)
{
	// local copies for speed
	float* magBuf = magnitutes.getData();
	if (bufferToFill != nullptr)
		magBuf = bufferToFill->getData();

	const SplitComplex &fftSplit = fftOperation.getFFTBuffer();
	const float oneOverFFTSize = (float) getFFTProperties().oneOverFFTSize;
	const int fftSizeHalved = getFFTProperties().fftSizeHalved;
	const float oneOverWindowFactor = windowProperties.getOneOverWindowFactor();
	
	// find magnitudes
	magBuf[0] = magnitude (fftSplit.realp[0], 0.0f, oneOverFFTSize, oneOverWindowFactor); // imag for DC is always zero 
	for (int i = 1; i < fftSizeHalved; i++)
	{		
		magBuf[i] = magnitude (fftSplit.realp[i], fftSplit.imagp[i], oneOverFFTSize, oneOverWindowFactor);
	}
	magBuf[fftSizeHalved] = magnitude (fftSplit.realp[0], 0.0f, oneOverFFTSize, oneOverWindowFactor); // imag for Nyquist is always zero 
	
	magnitutes.updateListeners();
}

void FFTEngine::updateMagnitudesIfBigger()
{
	// local copies for speed
	const SplitComplex &fftSplit = fftOperation.getFFTBuffer();
	float* magBuf = magnitutes.getData();
	const float oneOverFFTSize = (float) getFFTProperties().oneOverFFTSize;
	const int fftSizeHalved = getFFTProperties().fftSizeHalved;
	const float oneOverWindowFactor = windowProperties.getOneOverWindowFactor();
	
	// find magnitudes
	float newMag = magnitude (fftSplit.realp[0], 0.0f, oneOverFFTSize, oneOverWindowFactor); // imag for DC is always zero 
	if (newMag > magBuf[0])
		magBuf[0] = newMag;
    
	for (int i = 1; i < fftSizeHalved; i++)
	{		
		newMag = magnitude (fftSplit.realp[i], fftSplit.imagp[i], oneOverFFTSize, oneOverWindowFactor);

		if(newMag > magBuf[i])
			magBuf[i] = newMag;
	}

	newMag = magnitude (fftSplit.realp[0], 0.0f, oneOverFFTSize, oneOverWindowFactor); // imag for Nyquist is always zero 
	if (newMag > magBuf[fftSizeHalved])
		magBuf[fftSizeHalved] = newMag;
	
	magnitutes.updateListeners();
}

void FFTEngine::setWindowType (Window::WindowType type)
{
	windowProperties.setWindowType (type);
}



#endif // JUCE_MAC || DROWAUDIO_USE_FFTREAL
