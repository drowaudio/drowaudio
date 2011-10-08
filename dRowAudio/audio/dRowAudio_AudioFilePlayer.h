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

//#include "dRowAudio_SoundTouchAudioSource.h"
class SoundTouchAudioSource;
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
class AudioFilePlayer	:	public AudioSource
{
public:
    //==============================================================================
	/** Creates an empty AudioFilePlayer.
     */
	AudioFilePlayer();
    
	/** Destructor.
     */
	~AudioFilePlayer();
	
    /** Sets the current library entry.
     */
    void setLibraryEntry(ValueTree newEntry)                {   libraryEntry = newEntry;    }

    /** Returns the currents library entry.
     */
    ValueTree getLibraryEntry()                             {   return  libraryEntry;       }
    
	/** Open and get ready to play a given audio file from an absolute path.
     */
	bool setFile (const String& path);
    
    /** Open and get ready to play a given audio file from a stream.
     */
	bool setInputStream (InputStream* inputStream);
    
	/** Returns the absolute path of the current audio file.
     */
	String getPath()                                        {	return filePath;                }
    
    //==============================================================================
    /** Play the audio file from the start.
     */
	void startFromZero();
	
	/** Pauses the audio file.
     */
	void pause();
	    
    //==============================================================================
    /** Sets the resampling ratio to use also notifying any listeners.
     */
    void setResamplingRatio (const double samplesInPerOutputSample);
    
    //==============================================================================
	/// Returns the AudioFormatReaderSource currently being used
	inline AudioFormatReaderSource* getAudioFormatReaderSource()   {   return audioFormatReaderSource; }
	
    /** Sets the AudioFormatManager to use.
     */
    void setAudioFormatManager (AudioFormatManager* newManager,  bool deleteWhenNotNeeded = true);
    
	/** Returns the AudioFormatManager being used.
     */
	inline AudioFormatManager* getAudioFormatManager()             {   return formatManager;           }
        
    /** Returns the AudioTransportSource being used.
     */
    inline AudioTransportSource* getAudioTransportSource()         {   return audioTransportSource;    }

    /** Returns the SoundTouchAudioSource being used.
     */
    inline SoundTouchAudioSource* getSoundTouchAudioSource()       {   return soundTouchAudioSource;   }

	/** Returns the FilteringAudioSource being used.
     */
    inline FilteringAudioSource* getFilteringAudioSource()         {   return filteringAudioSource;    }

    //==============================================================================
    /** A class for receiving callbacks from a FilteringAudioFilePlayer.
	 
	 To be told when a player's file changes, you can register a FilteringAudioFilePlayer::Listener
	 object using FilteringAudioFilePlayer::addListener().
	 
	 @see AudioFilePlayer::addListener, AudioFilePlayer::removeListener
	 */
    class  Listener
    {
    public:
        //==============================================================================
        /** Destructor. */
        virtual ~Listener() {}
		
        //==============================================================================
        /** Called when the player's file is changed.
         
            You can find out the new file path using AudioFilePlayer::getFilePath().
		 */
        virtual void fileChanged (AudioFilePlayer *player) = 0;
		
		/** Called when the player's resampling ratio is changed.
		 
            You can find out the new ratio using AudioFilePlayer::getSoundTouchAudioSource().
		 */
		virtual void resamplingRatioChanged (AudioFilePlayer *player) {};

        /** Called when the player's tempo ratio is changed.
		 
            You can find out the new ratio using AudioFilePlayer::getSoundTouchAudioSource().
		 */
		virtual void tempoRatioChanged (AudioFilePlayer *player) {};

        /** Called when the player's pitch ratio is changed.
		 
            You can find out the new ratio using AudioFilePlayer::getSoundTouchAudioSource().
		 */
		virtual void pitchRatioChanged (AudioFilePlayer *player) {};
        
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
    /** Implementation of the AudioSource method. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    
    /** Implementation of the AudioSource method. */
    void releaseResources();
    
    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
    
    //==============================================================================
    
private:	
    //==============================================================================
	TimeSliceThread bufferingTimeSliceThread;

	OptionalScopedPointer<AudioFormatManager> formatManager;

    AudioSource* masterSource;
	ScopedPointer<AudioTransportSource> audioTransportSource;
    ScopedPointer<AudioFormatReaderSource> audioFormatReaderSource;
    ScopedPointer<SoundTouchAudioSource> soundTouchAudioSource;
    ScopedPointer<FilteringAudioSource> filteringAudioSource;

	String filePath;
    ValueTree libraryEntry;
    
    ListenerList <Listener> listeners;

	bool setSourceWithReader (AudioFormatReader* reader);
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFilePlayer);
};

#endif //_DROWAUDIO_AUDIOFILEPLAYER__H_