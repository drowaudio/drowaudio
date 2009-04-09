/*
 *  AudioFilePlayer.cpp
 *
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_AudioFilePlayer.h"

AudioFilePlayer::AudioFilePlayer()
{
	currentAudioFileSource = 0;
}

AudioFilePlayer::AudioFilePlayer(const String& path)
{
	currentAudioFileSource = 0;
	setFile(path);
}


AudioFilePlayer::~AudioFilePlayer()
{
	setSource (0);
	deleteAndZero (currentAudioFileSource);
}

void AudioFilePlayer::startFromZero()
{
	if(currentAudioFileSource == 0) return;
	
	setPosition (0.0);
	start();
}

void AudioFilePlayer::pause()
{
	if (this->isPlaying())
		this->stop();
	else
		this->start();
}

bool AudioFilePlayer::setFile(const String& path)
{
	// should really delete/reset any exisiting data in case this method is callled more than once
	// (which it isn't in this example)
	stop();
	setSource (0);
	deleteAndZero (currentAudioFileSource);
	
	filePath = path;
	
	// OK now let's add the new file
	AudioFormatReader* reader = audioFormatReaderFromFile(path);
	
	if (reader != 0)
	{										
		// we SHOULD let the AudioFormatReaderSource delete the reader for us..
		currentAudioFileSource = new AudioFormatReaderSource (reader, true);
		
		// ..and plug it into our transport source
		setSource (currentAudioFileSource,
				   32768, // tells it to buffer this many samples ahead
				   reader->sampleRate);
		
		// let our listeners know that we have loaded a new file
		sendChangeMessage(this);
		
		return true;
	}
	
	return false;
}

String AudioFilePlayer::getFile()
{
	return filePath;
}

String AudioFilePlayer::getFileName()
{
	return fileName;
}

AudioFormatReader* AudioFilePlayer::audioFormatReaderFromFile(const String& path)
{
	File audioFile (path);
	
	fileName = audioFile.getFileName();
	
	// get a format manager and set it up with the basic types (wav and aiff).
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	
	return formatManager.createReaderFor (audioFile);
}
