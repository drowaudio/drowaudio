/*
 *  AudioFilePlayerExt.h
 *  dRowAudio
 *
 *	Derived from code by Martin Robinson 2008
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_AUDIOFILEPLAYEREXT__H_
#define _DROWAUDIO_AUDIOFILEPLAYEREXT__H_

#include "dRowAudio_SoundTouchAudioSource.h"
#include "dRowAudio_ReversibleAudioSource.h"
#include "dRowAudio_LoopingAudioSource.h"
class FilteringAudioSource;

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
        SoundTouchSetting = 0x0001,
        LoopBeetweenTimesSetting = 0x0002,
        PlayDirectionSetting = 0x0003
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
	//void setPlayDirection (bool shouldPlayForwards)	{	soundTouchAudioSource->setPlayDirection (shouldPlayForwards);	}
    
    /** Returns true if the source is playing forwards.
     */
	//bool getPlayDirection ()						{	return soundTouchAudioSource->getPlayDirection();	}
    
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
    void setPositionIgnoringLoop (double newPosition);

    //==============================================================================
    /** Returns the SoundTouchAudioSource being used.
     */
    inline SoundTouchAudioSource* getSoundTouchAudioSource()       {   return soundTouchAudioSource;   }

	/** Returns the FilteringAudioSource being used.
     */
    inline FilteringAudioSource* getFilteringAudioSource()         {   return filteringAudioSource;    }
    
private:	
    //==============================================================================
    ScopedPointer<LoopingAudioSource> loopingAudioSource;
    ScopedPointer<SoundTouchAudioSource> soundTouchAudioSource;
    ScopedPointer<FilteringAudioSource> filteringAudioSource;

    ValueTree libraryEntry;

    //==============================================================================
	bool setSourceWithReader (AudioFormatReader* reader);
    
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFilePlayerExt);
};

#endif //_DROWAUDIO_AUDIOFILEPLAYEREXT__H_