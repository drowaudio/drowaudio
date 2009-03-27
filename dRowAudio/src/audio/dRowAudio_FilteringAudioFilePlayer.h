/*
 *  FilteringAudioFilePlayer.h
 *  resampling_test
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _FILTERINGAUDIOFILEPLAYER_H_
#define _FILTERINGAUDIOFILEPLAYER_H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

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
	FilteringAudioFilePlayer();
	
	/** Creates an AudioFilePlayer from a given absolute path
	 */
	FilteringAudioFilePlayer(const String& path);
	
	/// Destructor
	~FilteringAudioFilePlayer();
	
	/// Returns the AudioFormatReaderSource currently being used
	AudioFormatReaderSource* getAudioFormatReaderSource() { return currentAudioFileSource; }
	
	/// Returns the AudioFormatManager being used
	AudioFormatManager* getaudioFormatManager() { return formatManager; }
	
	/// Play the audio file from the start
	void startFromZero();
	
	/// Pauses the audio file
	void pause();
	
	/// Open and get ready to play a given audio file from an absolute path
	bool setFile(const String& path);
	
	/// Returns the absolute path of the current audio file
	String getFile();
	
	/// Returns the name of the currently loaded file
	String getFileName();
	
private:	
	/// The AudioFormatManager
	AudioFormatManager* formatManager;

	/// Create the reader from a given path
	AudioFormatReader* audioFormatReaderFromFile(const String& path);
	
	/// Create the actual stream that's going to read from the audio file
	AudioFormatReaderSource* currentAudioFileSource;
	
	String filePath;
	String fileName;
};

#endif //_FILTERINGAUDIOFILEPLAYER_H_