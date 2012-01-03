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

#ifndef __DROWAUDIO_FILTERINGAUDIOSOURCE_H__
#define __DROWAUDIO_FILTERINGAUDIOSOURCE_H__

//==============================================================================
/**	An AudioSource that contains three settable filters to EQ the audio stream.
 */
class FilteringAudioSource : public AudioSource
{

public:
	//==============================================================================
    /** Creates an FilteringAudioTransportSource.
	 
	 After creating one of these, use the setSource() method to select an input source.
	 */
    FilteringAudioSource (AudioSource* inputSource,
                          bool deleteInputWhenDeleted);
	
    /** Destructor. */
    ~FilteringAudioSource();
    
    //==============================================================================
	/** Changes the gain of the lowShelfFilter to apply to the output.
	 */
	void setLowEQGain (float newLowEQGain);
	
	/** Changes the gain of the bandPassFilter to apply to the output.
	 */
	void setMidEQGain (float newMidEQGain);
	
	/** Changes the gain of the highShelfFilter to apply to the output.
	 */
	void setHighEQGain (float newHighEQGain);
	
	/** Toggles the filtering of the transport source.
	 */
	void setFilterSource (bool shouldFilter);

	/** Returns whether the source is being filtered or not.
	 */
	bool getFilterSource()				{ return filterSource; }

    //==============================================================================
    /** Implementation of the AudioSource method. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	
    /** Implementation of the AudioSource method. */
    void releaseResources();
	
    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
		
private:
    //==============================================================================
    enum FilterType
    {
        low = 0,
        mid,
        high
    };

    OptionalScopedPointer<AudioSource> input;
	IIRFilter lowEQFilterL, midEQFilterL, highEQFilterL;
	IIRFilter lowEQFilterR, midEQFilterR, highEQFilterR;
	
    SpinLock callbackLock;
    double sampleRate;
	float lowEQGain, midEQGain, highEQGain;
	bool filterSource;

    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilteringAudioSource);
};

#endif //__DROWAUDIO_FILTERINGAUDIOSOURCE_H__