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

LTAS::LTAS (int fftSizeLog2)
    : fftEngine     (fftSizeLog2),
      ltasBuffer    (fftEngine.getMagnitudesBuffer().getSize()),
      fftSize       (fftEngine.getFFTSize()),
      numBins       (ltasBuffer.getSize()),
      tempBuffer    (fftSize)
{
    ltasBuffer.reset();

    ltasAvg.insertMultiple (0, CumulativeMovingAverage(), numBins);
}

LTAS::~LTAS()
{
    
}

void LTAS::updateLTAS (float* input, int numSamples)
{
    if (input != nullptr)
    {
        Buffer& fftBuffer (fftEngine.getMagnitudesBuffer());
        for (int i = 0; i < numBins; ++i)
            ltasAvg.getReference (i).reset();

        while (numSamples >= fftSize)
        {
            memcpy (tempBuffer, input, fftSize * sizeof (float));
            fftEngine.performFFT (tempBuffer);
            fftEngine.findMagnitudes();
            
            for (int i = 0; i < numBins; ++i)
                ltasAvg.getReference (i).add (fftBuffer[i]);
            
            input += fftSize;
            numSamples -= fftSize;
        }
        
        for (int i = 0; i < numBins; ++i)
            ltasBuffer.getReference (i) = (float) ltasAvg.getReference (i).getAverage();
        
        ltasBuffer.updateListeners();
    }
}

#endif // JUCE_MAC || JUCE_IOS || DROWAUDIO_USE_FFTREAL