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

#ifndef __DROWAUDIO_SONOGRAM_H__
#define __DROWAUDIO_SONOGRAM_H__

#if JUCE_MAC || JUCE_IOS || DROWAUDIO_USE_FFTREAL

//==============================================================================
/** Creates a standard right-left scrolling greyscale Sonogram.
    This is very simple to use, it is a GraphicalComponent so just register one
    with a TimeSliceThread, make sure its running and then continually call the
    copySamples() method. The FFT itself will be performed on a background thread.
 */
class Sonogram : public GraphicalComponent
{
public:
    //==============================================================================
    /** Creates a Sonogram with a given FFT size.
        Note that the fft size given here is log2 of the FFT size so for example,
        for a 1024 size FFT use 10 as the argument.
     */
    Sonogram (int fftSizeLog2);
	
    /** Destructor. */
	~Sonogram();
	
    /** @internal */
	void resized();
	
    /** @internal */
	void paint (Graphics &g);
	
    //==============================================================================
    /** Sets the scope to display in log or normal mode.
     */
	void setLogFrequencyDisplay (bool shouldDisplayLog);
	
    /** Returns true if the scope is being displayed in log mode.
     */
	inline bool getLogFrequencyDisplay() const     {	return logFrequency;	}
    
    /** Sets the width for one block of fft data. This must be greater than 0.
        Higher values will effectively cause the scope to move faster.
     */
    void setBlockWidth (int newBlockWidth);

    /** Returns the current block width.
     */
    int getBlockWidth() const;
    
    //==============================================================================
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
	
    //==============================================================================
    /** @internal */
	void flagForRepaint();

private:
    //==============================================================================
	FFTEngine fftEngine;
	int numBins;
	bool needsRepaint;
	HeapBlock<float> tempBlock;			
	FifoBuffer<float> circularBuffer;
	bool logFrequency;
    float scopeLineW;
    Image scopeImage, tempImage;

    CriticalSection lock;

    void renderScopeLine();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sonogram);
};

#endif
#endif  // __DROWAUDIO_SONOGRAM_H__