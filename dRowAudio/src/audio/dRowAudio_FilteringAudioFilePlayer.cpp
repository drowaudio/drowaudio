/*
 *  FilteringAudioFilePlayer.cpp
 *  resampling_test
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_FilteringAudioFilePlayer.h"

FilteringAudioFilePlayer::FilteringAudioFilePlayer(const String& path)
:	currentAudioFileSource(0),
	filePath(path),
	shouldBePlaying(false),
	deleteFormatManager(true)
{
	// set up the format manager
	formatManager = new AudioFormatManager();
	formatManager->registerBasicFormats();

	if (filePath != String::empty)
		setFile(path);
}

FilteringAudioFilePlayer::~FilteringAudioFilePlayer()
{
	setSource (0);
	if (deleteFormatManager == false) {
		formatManager.release();
	}
}

void FilteringAudioFilePlayer::setAudioFormatManager(AudioFormatManager* newManager,  bool deleteWhenNotNeeded)
{
	if (deleteFormatManager == false)
		formatManager.release();
	deleteFormatManager = deleteWhenNotNeeded;
	
	formatManager = newManager;
}

void FilteringAudioFilePlayer::startFromZero()
{
	if(currentAudioFileSource == 0)
		return;
	
	setPosition (0.0);
	start();
}

void FilteringAudioFilePlayer::pause()
{
	if (isPlaying()) {
		stop();
		shouldBePlaying = false;
	}
	else {
		start();
		shouldBePlaying = true;
	}
}

bool FilteringAudioFilePlayer::setFile(const String& path)
{
	// should really delete/reset any exisiting data in case this method is callled more than once
	// (which it isn't in this example)
	stop();
	setSource (0);
	
	filePath = path;
	
	// OK now let's add the new file
	AudioFormatReader *reader = audioFormatReaderFromFile(path);
	
	if (reader != 0)
	{										
		// we SHOULD let the AudioFormatReaderSource delete the reader for us..
		currentAudioFileSource = new AudioFormatReaderSource (reader, true);
		
		// ..and plug it into our transport source
		setSource (currentAudioFileSource,
				   32768, // tells it to buffer this many samples ahead
				   reader->sampleRate);
		
		// let our listeners know that we have loaded a new file
		sendChangeMessage();
		listeners.call (&Listener::fileChanged, this);
		
		if (shouldBePlaying)
			start();
		
		return true;
	}
	
	return false;
}

void FilteringAudioFilePlayer::setLibraryEntry(ValueTree newEntry)
{
	libraryEntry = newEntry.createCopy();
}

String FilteringAudioFilePlayer::getFilePath()
{
	return filePath;
}

String FilteringAudioFilePlayer::getFileName()
{
	return fileName;
}

void FilteringAudioFilePlayer::setLooping(bool shouldLoop)
{
	if (currentAudioFileSource != 0)
		currentAudioFileSource->setLooping(shouldLoop);
}

AudioFormatReader* FilteringAudioFilePlayer::audioFormatReaderFromFile(const String& path)
{
	File audioFile (path);
	fileName = audioFile.getFileName();
		
	return formatManager->createReaderFor (audioFile);
}
//==============================================================================
void FilteringAudioFilePlayer::addListener (FilteringAudioFilePlayer::Listener* const listener)
{
    listeners.add (listener);
}

void FilteringAudioFilePlayer::removeListener (FilteringAudioFilePlayer::Listener* const listener)
{
    listeners.remove (listener);
}

//==============================================================================

END_DROWAUDIO_NAMESPACE