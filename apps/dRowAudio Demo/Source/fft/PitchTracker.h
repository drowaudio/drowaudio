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

#ifndef __PITCHTRACKER_H__
#define __PITCHTRACKER_H__

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class PitchTracker : public GraphicalComponent
{
public:
    //==============================================================================
	PitchTracker (int fftSizeLog2);
	
	~PitchTracker();
	
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchTracker);
};


#endif  // __PITCHTRACKER_H__