/*
 *  FilteringAudioFilePlayer.h
 *  resampling_test
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _FILTERINGAUDIOFILEPLAYER_H_
#define _FILTERINGAUDIOFILEPLAYER_H_

#include "../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_FilteringAudioTransportSource.h"
#include "dRowAudio_ReversableAudioFormatReaderSource.h"
#include "MADAudioFormat.h"

/**
 This class can be used to load and play an audio file from disk.
 
 This combines the functionality of a FilteringAudioTransportSource, 
 AudioFormatReader and AudioFormatReaderSource.
 
 @see FilteringAudioTransportSource
 @see AudioFormatReader
 @see AudioFormatReaderSource
 */
class FilteringAudioFilePlayer	:	public FilteringAudioTransportSource
{
public:
	/// Creates an empty AudioFilePlayer.
	FilteringAudioFilePlayer(const String& path =String::empty);
		
	/// Destructor
	~FilteringAudioFilePlayer();
	
	/// Returns the AudioFormatReaderSource currently being used
	AudioFormatReaderSource* getAudioFormatReaderSource() { return currentAudioFileSource; }
	
	/// Returns the AudioFormatManager being used
	AudioFormatManager* getAudioFormatManager() { return formatManager; }
	
	/// Play the audio file from the start
	void startFromZero();
	
	/// Pauses the audio file
	void pause();
	
	/// Open and get ready to play a given audio file from an absolute path
	bool setFile(const String& path);
	
	/// Returns the absolute path of the current audio file
	String getFilePath();
	
	/// Returns the name of the currently loaded file
	String getFileName();
	
	void setLooping(bool shouldLoop);
	
//	void setPlayDirection(bool shouldPlayForwards)
//	{	
//		if (currentAudioFileSource != 0) {
//			currentAudioFileSource->setPlayDirection(shouldPlayForwards);
//		}
//	}
//	bool getPlayDirection()
//	{
//		if (currentAudioFileSource != 0) {
//			return currentAudioFileSource->getPlayDirection();
//		}	
//	}
	//==============================================================================
    /** A class for receiving callbacks from a FilteringAudioFilePlayer.
	 
	 To be told when a player's file changes, you can register a FilteringAudioFilePlayer::Listener
	 object using FilteringAudioFilePlayer::addListener().
	 
	 @see FilteringAudioFilePlayer::addListener, FilteringAudioFilePlayer::removeListener
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
        virtual void fileChanged (FilteringAudioFilePlayer *player) = 0;
		
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
	/// The AudioFormatManager
	ScopedPointer<AudioFormatManager> formatManager;

	/// Create the reader from a given path
	AudioFormatReader* audioFormatReaderFromFile(const String& path);
	
	/// Create the actual stream that's going to read from the audio file
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
	
	String filePath;
	String fileName;
	bool shouldBePlaying;
	
	ListenerList <Listener> listeners;
	
	JUCE_LEAK_DETECTOR (FilteringAudioFilePlayer);
};

#endif //_FILTERINGAUDIOFILEPLAYER_H_