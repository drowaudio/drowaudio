/*
  ==============================================================================

    Spectroscope.cpp
    Created: 19 Oct 2010 11:03:01pm
    Author:  David Rowland

  ==============================================================================
*/

#include "Spectroscope.h"

Spectroscope::Spectroscope (int fftSizeLog2)
:	fftEngine       (fftSizeLog2),
	needsRepaint    (true),
	tempBlock       (fftEngine.getFFTSize()),
	circularBuffer  (fftEngine.getMagnitudesBuffer()->getSize() * 4),
	logFrequency    (false)
{
	fftEngine.setWindowType (Window::Hann);
	numBins = fftEngine.getFFTProperties().fftSizeHalved;
	DBG ("Num bins: " << numBins);
    circularBuffer.reset();
	setOpaque (true);
//	fftEngine.getMagnitudesBuffer()->addListener(this);
//	responsePoints.malloc(fftEngine.getFFTProperties().fftSizeHalved);
    
    resized();
}

Spectroscope::~Spectroscope()
{
//	fftEngine.getMagnitudesBuffer()->removeListener(this);
}

void Spectroscope::resized()
{
    scopeImage = Image (Image::RGB,
                        jmax (1, getWidth()),
                        jmax (1, getHeight()),
                        true);
    scopeImage.clear (scopeImage.getBounds(), Colours::black);
}

void Spectroscope::paint(Graphics &g)
{
    g.drawImageAt (scopeImage, 0, 0, false);
}

//============================================	
/* GraphicalComponent implimentations		*/

void Spectroscope::copySamples (float* samples, int numSamples)
{
	circularBuffer.writeSamples (samples, numSamples);
	needToProcess = true;

	
/*	if (circularBuffer.getNumAvailable() > fftEngine.getFFTSize())
	{
		circularBuffer.readSamples(tempBlock.getData(), fftEngine.getFFTSize());
		
		fftEngine.performFFT(tempBlock);
		fftEngine.findMagnitudes();
		
		needsRepaint = true;
	}	*/
}

void Spectroscope::renderScopeImage()
{
    if (needsRepaint)
	{
        Graphics g (scopeImage);

		const int w = getWidth();
		const int h = getHeight();
        
		g.setColour(Colours::black);
		g.fillAll();
		
        
		g.setColour(Colours::white);
		
		if (logFrequency)
		{
			float scale = (float)w / log10(fftEngine.getFFTProperties().fftSizeHalved+1);
			float* data = fftEngine.getMagnitudesBuffer()->getData();
			
			float y2, y1 = jlimit(0.0f, 1.0f, float(1 + (toDecibels(data[0]) / 100.0f)));
			float x2, x1 = (log10(1)*scale);
            
			for (int i = 1; i < fftEngine.getFFTProperties().fftSizeHalved; i++)
			{
				y2 = jlimit(0.0f, 1.0f, float(1 + (toDecibels(data[i]) / 100.0f)));
				x2 = (log10(i+1)*scale);
                
                //				g.drawVerticalLine(x, h, h-h*y);
				g.drawLine(x1, h-h*y1,
						   x2, h-h*y2);
				
				y1 = y2;
				x1 = x2;
			}	
		}
		else
		{
			float scale = (float)w / (fftEngine.getFFTProperties().fftSizeHalved + 1);
			float* data = fftEngine.getMagnitudesBuffer()->getData();
			
			float y2, y1 = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[0]) / 100.0f)));
			float x2, x1 = (1 * scale);
            
			for (int i = 0; i < fftEngine.getFFTProperties().fftSizeHalved; i++)
			{
				y2 = jlimit (0.0f, 1.0f, float (1 + (toDecibels (data[i]) / 100.0f)));
				x2 = ((i+1) * scale);
				
				g.drawLine(x1, h - h * y1,
						   x2, h - h * y2);
				
				y1 = y2;
				x1 = x2;
			}	
			
		}
		
		needsRepaint = false;
	}
    else {
        DBG("Didn't need repaint");
    }

    
    repaint();
}

void Spectroscope::timerCallback()
{
	const int magnitudeBufferSize = fftEngine.getMagnitudesBuffer()->getSize();
	float* magnitudeBuffer = fftEngine.getMagnitudesBuffer()->getData();

    renderScopeImage();

	// fall levels here
	for (int i = 0; i < magnitudeBufferSize; i++)
		magnitudeBuffer[i] *= 0.707f;
	
}

void Spectroscope::process()
{
    jassert (circularBuffer.getNumFree() != 0); // buffer is too small!
    
    while (circularBuffer.getNumAvailable() > fftEngine.getFFTSize())
	{
		circularBuffer.readSamples (tempBlock.getData(), fftEngine.getFFTSize());
		fftEngine.performFFT (tempBlock);
		fftEngine.updateMagnitudesIfBigger();
		
		needsRepaint = true;
	}
}


