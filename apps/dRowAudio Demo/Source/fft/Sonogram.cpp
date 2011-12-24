/*
  ==============================================================================

    Sonogram.cpp
    Created: 19 Oct 2010 11:03:01pm
    Author:  David Rowland

  ==============================================================================
*/

#include "Sonogram.h"

Sonogram::Sonogram (int fftSizeLog2)
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

//============================================	
void Sonogram::copySamples (const float* samples, int numSamples)
{
	circularBuffer.writeSamples (samples, numSamples);
	needToProcess = true;
}

void Sonogram::renderScopeImage()
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

void Sonogram::timerCallback()
{
	const int magnitudeBufferSize = fftEngine.getMagnitudesBuffer().getSize();
	float* magnitudeBuffer = fftEngine.getMagnitudesBuffer().getData();

    if (needsRepaint)
        repaint();
    
    //renderScopeImage();
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

void Sonogram::renderScopeLine()
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
    const float yScale = (float)h / (numBins + 1);
    const float* data = fftEngine.getMagnitudesBuffer().getData();
    
    float amp = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[0]) / 100.0f)));
    float y2, y1 = 0;
    
    {
        for (int i = 0; i < numBins; ++i)
        {
            amp = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[i]) / 100.0f)));
            y2 = (i + 1) * yScale;
            
//            g.setColour (Colour::fromHSV (amp * 0.7, 1.0f, 1.0f, 1.0f));
            g.setColour (Colour::greyLevel (amp));
            g.drawVerticalLine (x, h - y2, h - y1);
//            g.drawLine (0, h - (h * y1),
//                        1, h - (h * y2));

            y1 = y2;
        }	
    }
    
//    Graphics g2 (scopeImage);
//    g2.drawImageAt (tempImage, scopeImage.getWidth() - 1, 0);
}