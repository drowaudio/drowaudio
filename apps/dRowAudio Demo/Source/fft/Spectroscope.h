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
    Image scopeImage;
    
    void renderScopeImage();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spectroscope);
};

#endif  // __SPECTROSCOPE_H_F2E587AC__
