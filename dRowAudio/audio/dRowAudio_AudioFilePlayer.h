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
	String getFile()                                        {	return filePath;	}
    
	/// Returns the AudioFormatReaderSource currently being used
	AudioFormatReaderSource* getAudioFormatReaderSource()   { return currentAudioFileSource; }
	
	/// Returns the AudioFormatManager being used
	AudioFormatManager* getAudioFormatManager()             { return &formatManager; }
        
private:	
	AudioFormatManager formatManager;
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;
	
	TimeSliceThread bufferingTimeSliceThread;
	
	String filePath;
    
	bool setSourceWithReader (AudioFormatReader* reader);
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFilePlayer);
};

#endif //_DROWAUDIO_AUDIOFILEPLAYER__H_