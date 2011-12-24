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

#include "PitchTracker.h"

PitchTracker::PitchTracker (int fftSizeLog2)
:	fftEngine       (fftSizeLog2),
	needsRepaint    (true),
	tempBlock       (fftEngine.getFFTSize()),
	circularBuffer  (fftEngine.getMagnitudesBuffer().getSize() * 4),
	logFrequency    (false)
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

PitchTracker::~PitchTracker()
{
}

void PitchTracker::resized()
{
    const ScopedLock sl (lock);
    scopeImage = scopeImage.rescaled (jmax (1, getWidth()), jmax (1, getHeight()));
}

void PitchTracker::paint(Graphics &g)
{
    const ScopedLock sl (lock);
    g.drawImageAt (scopeImage, 0, 0, false);
}

//============================================	
void PitchTracker::copySamples (const float* samples, int numSamples)
{
	circularBuffer.writeSamples (samples, numSamples);
	needToProcess = true;
}

void PitchTracker::renderScopeImage()
{
    if (needsRepaint)
	{
        Graphics g (scopeImage);

		const int w = getWidth();
		const int h = getHeight();
        
		g.setColour (Colours::white);
		g.fillAll();
        
		g.setColour (Colours::black);
		
        const int numBins = fftEngine.getMagnitudesBuffer().getSize() - 1;
        const float xScale = (float)w / (numBins + 1);
        const float* data = fftEngine.getMagnitudesBuffer().getData();
        

        float y2, y1 = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[0]) / 100.0f)));
        float x2, x1 = 0;
        
        if (logFrequency)
		{
			for (int i = 1; i < numBins; ++i)
			{
				y2 = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[i]) / 100.0f)));
				x2 = log10 (i + 1) * xScale;
                
				g.drawLine (x1, h-h*y1,
						    x2, h-h*y2);
				
				y1 = y2;
				x1 = x2;
			}	
		}
		else
		{
			for (int i = 0; i < numBins; ++i)
			{
				y2 = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[i]) / 100.0f)));
				x2 = (i + 1) * xScale;
				
				g.drawLine (x1, h - h * y1,
						    x2, h - h * y2);
				
				y1 = y2;
				x1 = x2;
			}	
		}
		
		needsRepaint = false;

        repaint();
	}
}

void PitchTracker::timerCallback()
{
    if (needsRepaint)
        repaint();
}

void PitchTracker::process()
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

void PitchTracker::renderScopeLine()
{
    const ScopedLock sl (lock);

    scopeImage.moveImageSection (0, 0, 1, 0, scopeImage.getWidth(), scopeImage.getHeight());

    //    const int w = scopeImage.getWidth();
    const int h = scopeImage.getHeight();

//    tempImage = Image (Image::RGB, 1, h, false);
//    tempImage.clear (tempImage.getBounds(), Colours::white);
    
    Graphics g (scopeImage);
    const int x = scopeImage.getWidth() - 1;
        
    const int numBins = fftEngine.getMagnitudesBuffer().getSize() - 1;
    const float yScale = (float)h / log10 (numBins + 1);
    const float* data = fftEngine.getMagnitudesBuffer().getData();
    
    float amp = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[0]) / 100.0f)));
    
    float max = amp;
    int maxBin = 0;
    for (int i = 0; i < numBins; ++i)
    {
        amp = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[i]) / 100.0f)));
        
        if (amp > max)
        {
            max = amp;
            maxBin = i;
        }
    }
    
    float y1 = log10 (maxBin) * yScale;
    //y2 = (maxBin + 1) * yScale;
    
    g.setColour (Colours::black);
    g.drawVerticalLine (x, 0.0f, h);
    
    g.setColour (Colour::greyLevel (1.0f));
    g.drawVerticalLine (x, h - y1 + 1, h - y1);
    
    
    
//    Graphics g2 (scopeImage);
//    g2.drawImageAt (tempImage, scopeImage.getWidth() - 1, 0);
}