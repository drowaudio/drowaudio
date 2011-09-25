/*
 *  AudioFilePlayer.cpp
 *
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

AudioFilePlayer::AudioFilePlayer()
    :  bufferingTimeSliceThread ("Shared Buffering Thread")
{
    bufferingTimeSliceThread.startThread(3);
    
	formatManager.registerBasicFormats();
}

AudioFilePlayer::~AudioFilePlayer()
{
	setSource (nullptr);
}

void AudioFilePlayer::startFromZero()
{
	if(currentAudioFileSource == nullptr)
        return;
	
	setPosition (0.0);
	start();
}

void AudioFilePlayer::pause()
{
	if (isPlaying())
		stop();
	else
		start();
}

bool AudioFilePlayer::setFile(const String& path)
{
    filePath = path;
    
    return setSourceWithReader (formatManager.createReaderFor (File (path)));
}

bool AudioFilePlayer::setInputStream (InputStream* inputStream)
{
    filePath = String::empty;
    
	AudioFormatReader* reader = formatManager.createReaderFor (inputStream);
    
    return setSourceWithReader (reader);
}

bool AudioFilePlayer::setSourceWithReader (AudioFormatReader* reader)
{
    // should really delete/reset any exisiting data in case this method is callled more than once
	// (which it isn't in this example)
	stop();
	setSource (nullptr);
	deleteAndZero (currentAudioFileSource);
    
	if (reader != nullptr)
	{										
		// we SHOULD let the AudioFormatReaderSource delete the reader for us..
		currentAudioFileSource = new AudioFormatReaderSource (reader, true);
        
		// ..and plug it into our transport source
		setSource (currentAudioFileSource,
				   0,//32768,
                   nullptr, //&bufferingTimeSliceThread,
                   reader->sampleRate); // tells it to buffer this many samples ahead
        
		// let our listeners know that we have loaded a new file
		sendChangeMessage();
		
		return true;
	}
	
	return false;    
}

END_JUCE_NAMESPACE