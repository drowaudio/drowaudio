/*
  ==============================================================================

    Sonogram.h
    Created: 19 Oct 2010 11:03:01pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __SONOGRAM_H_F2E587AC__
#define __SONOGRAM_H_F2E587AC__

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class Sonogram : public GraphicalComponent
{
public:
    //==============================================================================
	Sonogram (int fftSizeLog2);
	
	~Sonogram();
	
	void resized();
	
	void paint(Graphics &g);
	
	//============================================	
	/* GraphicalComponent implimentations		*/
	void copySamples (const float* samples, int numSamples);

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
    Image scopeImage, tempImage;

    CriticalSection lock;

    void renderScopeImage();
    void renderScopeLine();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sonogram);
};


#endif  // __SONOGRAM_H_F2E587AC__