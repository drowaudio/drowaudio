/*
  ==============================================================================

  This file is part of the dRowAudio JUCE module
  Copyright 2004-13 by dRowAudio.

  ------------------------------------------------------------------------------

  dRowAudio is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ==============================================================================
*/

#if DROWAUDIO_USE_FFTREAL || DROWAUDIO_USE_VDSP

LTAS::LTAS (int fftSizeLog2)
    : fftEngine     (fftSizeLog2),
      ltasBuffer    (fftEngine.getMagnitudesBuffer().getSize()),
      fftSize       (fftEngine.getFFTSize()),
      numBins       (int (ltasBuffer.getSize())),
      tempBuffer    (fftSize)
{
    ltasBuffer.reset();

    ltasAvg.insertMultiple (0, CumulativeMovingAverage(), numBins);
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
            memcpy (tempBuffer, input, size_t (fftSize) * sizeof (float));
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

#endif //DROWAUDIO_USE_FFTREAL
