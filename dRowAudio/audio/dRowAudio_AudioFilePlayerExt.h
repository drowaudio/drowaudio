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

#ifndef __DROWAUDIO_AUDIOFILEPLAYEREXT_H__
#define __DROWAUDIO_AUDIOFILEPLAYEREXT_H__

#if DROWAUDIO_USE_SOUNDTOUCH

#include "dRowAudio_SoundTouchAudioSource.h"
#include "dRowAudio_ReversibleAudioSource.h"
#include "dRowAudio_LoopingAudioSource.h"
#include "dRowAudio_FilteringAudioSource.h"

//==============================================================================
/**
 This class can be used to load and play an audio file from disk.
 
 This combines the functionality of an AudioTransportSource, 
 AudioFormatReader and AudioFormatReaderSource.
 
 @see AudioTransportSource
 @see AudioFormatReader
 @see AudioFormatReaderSource
 */
class AudioFilePlayerExt :  public AudioFilePlayer
{
public:
    //==============================================================================
    enum PlayerSettingCode
    {
        SoundTouchSetting           = 0x0001,
        LoopBeetweenTimesSetting    = 0x0002,
        PlayDirectionSetting        = 0x0003,
        FilterGainSetting           = 0x0004
    };
    
    //==============================================================================
	/** Creates an empty AudioFilePlayerExt.
     */
	AudioFilePlayerExt();
    
	/** Destructor.
     */
	~AudioFilePlayerExt();
	
    //==============================================================================
    /** Sets the current library entry.
     */
    void setLibraryEntry (ValueTree newEntry)               {   libraryEntry = newEntry;    }

    /** Returns the currents library entry.
     */
    ValueTree getLibraryEntry()                             {   return  libraryEntry;       }
    
    //==============================================================================
    /** Sets SoundTouchProcessor settings.
     */
    void setPlaybackSettings (SoundTouchProcessor::PlaybackSettings newSettings);
    
    /** Returns the current SoundTouchProcessor settings.
     */
    SoundTouchProcessor::PlaybackSettings getPlaybackSettings() {   return soundTouchAudioSource->getPlaybackSettings();    }    
    
    /** Sets whether the source should play forwards or backwards.
     */
	void setPlayDirection (bool shouldPlayForwards);
    
    /** Returns true if the source is playing forwards.
     */
	bool getPlayDirection()                                     {   return reversibleAudioSource->getPlayDirection();       }

    /** Sets the gain of one of the FilteringAudioSource filters.
     */
    void setFilterGain (FilteringAudioSource::FilterType type, float newGain);
    
    //==============================================================================
    /** Sets the start and end times of the loop.
        This doesn't actually activate the loop, use setLoopBetweenTimes() to toggle this.
     */
	void setLoopTimes (double startTime, double endTime);
	
    /** Enables the loop point set.
     */
    void setLoopBetweenTimes (bool shouldLoop);
    
    /** Returns true if the loop is activated.
     */
    bool getLoopBetweenTimes()                      {   return loopingAudioSource->getLoopBetweenTimes();       }
    
    /** Sets the next play position in seconds disregarding the loop boundries.
     */
    void setPosition (double newPosition, bool ignoreAnyLoopBounds = false);
    
    //==============================================================================
    /** Returns the SoundTouchAudioSource being used.
     */
    inline SoundTouchAudioSource* getSoundTouchAudioSource()       {   return soundTouchAudioSource;   }

	/** Returns the FilteringAudioSource being used.
     */
    inline FilteringAudioSource* getFilteringAudioSource()         {   return filteringAudioSource;    }
    
private:	
    //==============================================================================
    ScopedPointer<BufferingAudioSource> bufferingAudioSource;
    ScopedPointer<LoopingAudioSource> loopingAudioSource;
    ScopedPointer<SoundTouchAudioSource> soundTouchAudioSource;
    ScopedPointer<ReversibleAudioSource> reversibleAudioSource;
    ScopedPointer<FilteringAudioSource> filteringAudioSource;

    ValueTree libraryEntry;

    //==============================================================================
	bool setSourceWithReader (AudioFormatReader* reader);
    
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFilePlayerExt);
};

#endif
#endif // __DROWAUDIO_AUDIOFILEPLAYEREXT_H__