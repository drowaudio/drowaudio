/*
 *  FilteringAudioFilePlayer.cpp
 *  resampling_test
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_FilteringAudioFilePlayer.h"

FilteringAudioFilePlayer::FilteringAudioFilePlayer()
	: currentAudioFileSource(0)
{
	// set up the format manager
	formatManager = new AudioFormatManager();
	formatManager->registerBasicFormats();
	formatManager->registerFormat(new MADAudioFormat(), false);	
}

FilteringAudioFilePlayer::FilteringAudioFilePlayer(const String& path)
	:	currentAudioFileSource(0)
{
	setFile(path);
}


FilteringAudioFilePlayer::~FilteringAudioFilePlayer()
{
	setSource (0);
	deleteAndZero (currentAudioFileSource);
}

void FilteringAudioFilePlayer::startFromZero()
{
	if(currentAudioFileSource == 0) return;
	
	setPosition (0.0);
	start();
}

void FilteringAudioFilePlayer::pause()
{
	if (this->isPlaying())
		this->stop();
	else
		this->start();
}

bool FilteringAudioFilePlayer::setFile(const String& path)
{
	// should really delete/reset any exisiting data in case this method is callled more than once
	// (which it isn't in this example)
	stop();
	setSource (0);
	if (currentAudioFileSource != 0)
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

String FilteringAudioFilePlayer::getFile()
{
	return filePath;
}

String FilteringAudioFilePlayer::getFileName()
{
	return fileName;
}

AudioFormatReader* FilteringAudioFilePlayer::audioFormatReaderFromFile(const String& path)
{
	File audioFile (path);
	
	fileName = audioFile.getFileName();
		
	return formatManager->createReaderFor (audioFile);
}
