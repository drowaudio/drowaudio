/*
 *  AudioFilePlayer.h
 *  dRowAudio
 *
 *	Derived from code by Martin Robinson 2008
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef AUDIOFILEPLAYER_H
#define AUDIOFILEPLAYER_H

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

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
	
	/** Creates an AudioFilePlayer from a given absolute path
	 */
	AudioFilePlayer(const String& path);
	
	/// Destructor
	~AudioFilePlayer();
	
	/// Play the audio file from the start
	void startFromZero();
	
	/// Pauses the audio file
	void pause();
	
	/// Open and get ready to play a given audio file from an absolute path
	bool setFile(const String& path);

	/// Returns the absolute path of the current audio file
	String getFile() 	{	return filePath;	}
	
	/// Returns the name of the currently loaded file
	String getFileName()	{	return fileName;	}
	
	/// Togles Loop Mode
	void setLooping(bool shouldLoop);
		
	/// Returns the AudioFormatReaderSource currently being used
	AudioFormatReaderSource* getAudioFormatReaderSource() { return currentAudioFileSource; }
	
	/// Returns the AudioFormatManager being used
	AudioFormatManager* getAudioFormatManager() { return formatManager; }

private:	
	AudioFormatManager* formatManager;
	AudioFormatReaderSource* currentAudioFileSource;
	
	/// Create the reader from a given path
	AudioFormatReader* audioFormatReaderFromFile(const String& path);
	
	
	String filePath;
	String fileName;
};

#endif //AUDIOFILEPLAYER_H