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

#ifndef __DROWAUDIO_SPECTROSCOPE_H__
#define __DROWAUDIO_SPECTROSCOPE_H__

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
    /** Sets the scope to display in log or normal mode.
     */
	void setLogFrequencyDisplay (bool shouldDisplayLog);
	
    /** Returns true if the scope is being displayed in log mode.
     */
	inline bool getLogFrequencyDisplay() const      {   return logFrequency;	}

	//============================================	
	/** Copy a set of samples, ready to be processed.
        Your audio callback should continually call this method to pass it its
        audio data. When the scope has enough samples to perform an fft it will do
        so on a background thread and redraw itself.
     */
	void copySamples (const float* samples, int numSamples);

    /** @internal */
	void timerCallback();
	
    /** @internal */
	void process();
	
    /** @internal */
	void flagForRepaint();

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

#endif  // __DROWAUDIO_SPECTROSCOPE_H__