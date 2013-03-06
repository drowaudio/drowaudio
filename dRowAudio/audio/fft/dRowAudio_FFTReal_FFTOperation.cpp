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

#if DROWAUDIO_USE_FFTREAL



FFTOperation::FFTOperation (int fftSizeLog2)
    : fftProperties (fftSizeLog2)
{
	fftConfig = new ffft::FFTReal<float> (fftProperties.fftSize);

	fftBuffer.malloc (fftProperties.fftSize);
	fftBufferSplit.realp = fftBuffer.getData();
	fftBufferSplit.imagp = fftBufferSplit.realp + getFFTProperties().fftSizeHalved;	
}

FFTOperation::~FFTOperation()
{
}

void FFTOperation::setFFTSizeLog2 (int newFFTSizeLog2)
{
	if (newFFTSizeLog2 != fftProperties.fftSizeLog2)
    {
        fftConfig = nullptr;
		
		fftProperties.setFFTSizeLog2 (newFFTSizeLog2);
		fftBuffer.malloc (fftProperties.fftSize);
		fftBufferSplit.realp = fftBuffer.getData();
		fftBufferSplit.imagp = fftBufferSplit.realp + getFFTProperties().fftSizeHalved;	
		
        fftConfig = new ffft::FFTReal<float> (fftProperties.fftSize);
	}
}

void FFTOperation::performFFT (float* samples)
{
    fftConfig->do_fft (fftBuffer.getData(), samples);
}



#endif //DROWAUDIO_USE_FFTREAL