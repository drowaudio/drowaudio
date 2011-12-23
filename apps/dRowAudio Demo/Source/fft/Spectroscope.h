/*
  ==============================================================================

    SpectroScope.h
    Created: 19 Oct 2010 11:03:01pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __SPECTROSCOPE_H_F2E587AC__
#define __SPECTROSCOPE_H_F2E587AC__

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class Spectroscope : public GraphicalComponent
{
public:
    //==============================================================================
	Spectroscope (int fftSizeLog2);
	
	~Spectroscope();
	
	void resized();
	
	void paint(Graphics &g);
	
	//============================================
//	void processSamples(float* samples, int numSamples);
//	
//	void bufferChanged(Buffer* buffer);
		
	//============================================	
	/* GraphicalComponent implimentations		*/
	 
	void copySamples (float* samples, int numSamples);

	void timerCallback();
	
	void process();
	
	//============================================	

	void flagForRepaint()
	{	
		needsRepaint = true;
		repaint();
	}

	//============================================	
	void setLogFrequencyDisplay(bool shouldDisplayLog)
	{
		logFrequency = shouldDisplayLog;
	}
	
	bool getLogFrequencyDisplay()	{	return logFrequency;	}

private:
    //==============================================================================
	FFTEngine fftEngine;
	int numBins;
	bool needsRepaint;
	HeapBlock<float> tempBlock;			
	FifoBuffer circularBuffer;
	
	bool logFrequency;
    Image scopeImage;
    
    void renderScopeImage();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spectroscope);
};


#endif  // __SPECTROSCOPE_H_F2E587AC__
