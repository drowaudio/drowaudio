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

#if JUCE_MAC && ! DROWAUDIO_USE_FFTREAL



FFTOperation::FFTOperation (int fftSizeLog2)
    : fftProperties (fftSizeLog2)
{
	fftConfig = vDSP_create_fftsetup (fftProperties.fftSizeLog2, 0);

	fftBuffer.malloc (fftProperties.fftSize);
	fftBufferSplit.realp = fftBuffer.getData();
	fftBufferSplit.imagp = fftBufferSplit.realp + getFFTProperties().fftSizeHalved;	
}

FFTOperation::~FFTOperation()
{
	vDSP_destroy_fftsetup (fftConfig);
}

void FFTOperation::setFFTSizeLog2 (int newFFTSizeLog2)
{
	if (newFFTSizeLog2 != fftProperties.fftSizeLog2)
    {
		vDSP_destroy_fftsetup (fftConfig);
		
		fftProperties.setFFTSizeLog2 (newFFTSizeLog2);
		fftBuffer.malloc (fftProperties.fftSize);
		fftBufferSplit.realp = fftBuffer.getData();
		fftBufferSplit.imagp = fftBufferSplit.realp + getFFTProperties().fftSizeHalved;	
		
		fftConfig = vDSP_create_fftsetup (fftProperties.fftSizeLog2, 0);
	}
}

void FFTOperation::performFFT (float* samples)
{
	vDSP_ctoz ((COMPLEX *) samples, 2, &fftBufferSplit, 1, fftProperties.fftSizeHalved);
	vDSP_fft_zrip (fftConfig, &fftBufferSplit, 1, fftProperties.fftSizeLog2, FFT_FORWARD);
}

//============================================================================



#endif //JUCE_MAC