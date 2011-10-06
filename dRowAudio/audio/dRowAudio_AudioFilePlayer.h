/*
 *  AudioFilePlayer.h
 *  dRowAudio
 *
 *	Derived from code by Martin Robinson 2008
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_AUDIOFILEPLAYER__H_
#define _DROWAUDIO_AUDIOFILEPLAYER__H_

#include "dRowAudio_SoundTouchAudioSource.h"

/**
 This class can be used to load and play an audio file from disk.
 
 This combines the functionality of an AudioTransportSource, 
 AudioFormatReader and AudioFormatReaderSource.
 
 @see AudioTransportSource
 @see AudioFormatReader
 @see AudioFormatReaderSource
 */
class AudioFilePlayer	:	public AudioTransportSource
{
public:
	/// Creates an empty AudioFilePlayer.
	AudioFilePlayer();
    
	/// Destructor
	~AudioFilePlayer();
	
	/// Play the audio file from the start
	void startFromZero();
	
	/// Pauses the audio file
	void pause();
	
	/// Open and get ready to play a given audio file from an absolute path
	bool setFile (const String& path);
    
    /// Open and get ready to play a given audio file from a stream
	bool setInputStream (InputStream* inputStream);
    
	/// Returns the absolute path of the current audio file
	String getPath()                                        {	return filePath;	}
    
	/// Returns the AudioFormatReaderSource currently being used
	AudioFormatReaderSource* getAudioFormatReaderSource()   { return currentAudioFileSource; }
	
	/// Returns the AudioFormatManager being used
	AudioFormatManager* getAudioFormatManager()             { return &formatManager; }
        
    SoundTouchAudioSource* getSoundTouchAudioSource()       { return soundTouchAudioSource; }

    //==============================================================================
    /** A class for receiving callbacks from a FilteringAudioFilePlayer.
	 
	 To be told when a player's file changes, you can register a FilteringAudioFilePlayer::Listener
	 object using FilteringAudioFilePlayer::addListener().
	 
	 @see SoundTouchAudioFilePlayer::addListener, SoundTouchAudioFilePlayer::removeListener
	 */
    class  Listener
    {
    public:
        //==============================================================================
        /** Destructor. */
        virtual ~Listener() {}
		
        //==============================================================================
        /** Called when the player's file is changed.
         
         You can find out the new file path using FilteringAudioFilePlayer::getFilePath().
		 */
        virtual void fileChanged (AudioFilePlayer *player) = 0;
		
		/** Called when the player's resampling ratio is changed.
		 
         You can find out the new ratio using FilteringAudioFilePlayer::getResamplingRatio().
		 */
		virtual void resamplingRatioChanged (AudioFilePlayer *player) {};
        
        //==============================================================================
        /** Called when the the player is stopped or started.
         You can find out if it is currently stopped with FilteringAudioFilePlayer::isPlaying().
		 */
        //        virtual void playerStoppedOrStarted (FilteringAudioFilePlayer *player);
    };
	
    /** Adds a listener to be called when this slider's value changes. */
    void addListener (Listener* listener);
	
    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);
	
    //==============================================================================
    
private:	
	AudioFormatManager formatManager;
    ScopedPointer<SoundTouchAudioSource> soundTouchAudioSource;
    ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
	
	TimeSliceThread bufferingTimeSliceThread;
	
	String filePath;
    
    ListenerList <Listener> listeners;

	bool setSourceWithReader (AudioFormatReader* reader);
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFilePlayer);
};

#endif //_DROWAUDIO_AUDIOFILEPLAYER__H_