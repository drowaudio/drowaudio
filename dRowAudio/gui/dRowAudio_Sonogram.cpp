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

Sonogram::Sonogram (int fftSizeLog2)
:	fftEngine       (fftSizeLog2),
	needsRepaint    (true),
	tempBlock       (fftEngine.getFFTSize()),
	circularBuffer  (fftEngine.getMagnitudesBuffer().getSize() * 4),
	logFrequency    (false),
    scopeLineW      (1.0f)
{
	setOpaque (true);

	fftEngine.setWindowType (Window::Hann);
	numBins = fftEngine.getFFTProperties().fftSizeHalved;
    
    circularBuffer.reset();
    
    scopeImage = Image (Image::RGB,
                        100, 100,
                        false);
    scopeImage.clear (scopeImage.getBounds(), Colours::black);
}

Sonogram::~Sonogram()
{
}

void Sonogram::resized()
{
    const ScopedLock sl (lock);
    scopeImage = scopeImage.rescaled (jmax (1, getWidth()), jmax (1, getHeight()));
}

void Sonogram::paint(Graphics &g)
{
    const ScopedLock sl (lock);
    g.drawImageAt (scopeImage, 0, 0, false);
}

//==============================================================================
void Sonogram::setLogFrequencyDisplay (bool shouldDisplayLog)
{
    logFrequency = shouldDisplayLog;
}

void Sonogram::setBlockWidth (int newBlockWidth)
{
    jassert (newBlockWidth > 0);
    scopeLineW = (float) newBlockWidth;
}

int Sonogram::getBlockWidth() const
{
    return (int) scopeLineW;
}

//==============================================================================
void Sonogram::copySamples (const float* samples, int numSamples)
{
	circularBuffer.writeSamples (samples, numSamples);
	needToProcess = true;
}

void Sonogram::timerCallback()
{
    if (needsRepaint)
        repaint();
}

void Sonogram::process()
{
    jassert (circularBuffer.getNumFree() != 0); // buffer is too small!
    
    while (circularBuffer.getNumAvailable() > fftEngine.getFFTSize())
	{
		circularBuffer.readSamples (tempBlock.getData(), fftEngine.getFFTSize());
		fftEngine.performFFT (tempBlock);
		fftEngine.findMagnitudes();

        renderScopeLine();
        
		needsRepaint = true;
	}
}

void Sonogram::flagForRepaint()
{	
    needsRepaint = true;
    repaint();
}

void Sonogram::renderScopeLine()
{
    const ScopedLock sl (lock);

    scopeImage.moveImageSection (0, 0, (int) scopeLineW, 0,
                                 scopeImage.getWidth(), scopeImage.getHeight());

    const int h = scopeImage.getHeight();
    
    Graphics g (scopeImage);
    const int x = scopeImage.getWidth() - (int) scopeLineW;
        
    const int numBins = fftEngine.getMagnitudesBuffer().getSize() - 1;
    const float yScale = (float) h / (numBins + 1);
    const float* data = fftEngine.getMagnitudesBuffer().getData();
    
    float amp = jlimit (0.0f, 1.0f, (float) (1 + (toDecibels (data[0]) / 100.0f)));
    float y2, y1 = 0;
    
    if (logFrequency)
    {
        for (int i = 0; i < numBins; ++i)
        {
            amp = jlimit (0.0f, 1.0f, (float) (1 + (toDecibels (data[i]) / 100.0f)));
            y2 = log10 (1 + 39 * ((i + 1.0f) / numBins)) / log10 (40.0f) * h;

            g.setColour (Colour::greyLevel (amp));
            g.fillRect ((float)x, h - y2, scopeLineW, y1 - y2);
            
            y1 = y2;
        }	
    }
    else
    {
        for (int i = 0; i < numBins; ++i)
        {
            amp = jlimit (0.0f, 1.0f, (float) (1 + (toDecibels (data[i]) / 100.0f)));
            y2 = (i + 1) * yScale;
            
            g.setColour (Colour::greyLevel (amp));
            g.fillRect ((float) x, h - y2, scopeLineW, y1 - y2);

            y1 = y2;
        }	
    }
}

#endif // JUCE_MAC || JUCE_IOS || DROWAUDIO_USE_FFTREAL
