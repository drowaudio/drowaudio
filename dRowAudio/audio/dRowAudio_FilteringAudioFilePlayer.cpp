/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_FilteringAudioFilePlayer.h"

FilteringAudioFilePlayer::FilteringAudioFilePlayer(const String& path)
:	masterSource(nullptr),
    filePath(path),
	shouldBePlaying(false),
	deleteFormatManager(true)
{
    audioTransportSource = new AudioTransportSource();
    //smoothingAudioSource = new SmoothingAudioSource(audioTransportSource, false);
    reversibleAudioSource = new ReversibleAudioSource(audioTransportSource, false);
    resamplingAudioSource = new ResamplingAudioSource(reversibleAudioSource, false);
    filteringAudioSource = new FilteringAudioSource(resamplingAudioSource, false);
    //reversibleAudioSource->setPlayDirection(false);
    //filteringAudioSource->setLowEQGain(0.001);
    masterSource = filteringAudioSource;
    
	// set up the format manager
	formatManager = new AudioFormatManager();
	formatManager->registerBasicFormats();
	
	if (filePath != String::empty)
		setFile(path);
}

FilteringAudioFilePlayer::~FilteringAudioFilePlayer()
{
	audioTransportSource->setSource (0);
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
	if(currentAudioFormatReaderSource == nullptr)
		return;
	
	audioTransportSource->setPosition (0.0);
	audioTransportSource->start();
}

void FilteringAudioFilePlayer::pause()
{
	if (audioTransportSource->isPlaying()) {
		audioTransportSource->stop();
		shouldBePlaying = false;
	}
	else {
		audioTransportSource->start();
		shouldBePlaying = true;
	}
}

bool FilteringAudioFilePlayer::setFile(const String& path)
{
	// should really delete/reset any exisiting data in case this method is callled more than once
	// (which it isn't in this example)
	audioTransportSource->stop();
	audioTransportSource->setSource (0);
	
	filePath = path;
	
	// OK now let's add the new file
	AudioFormatReader *reader = audioFormatReaderFromFile(path);
	
	if (reader != 0)
	{										
        // we SHOULD let the AudioFormatReaderSource delete the reader for us..
		currentAudioFormatReaderSource = new AudioFormatReaderSource (reader, true);

		// ..and plug it into our transport source
        audioTransportSource->setSource (currentAudioFormatReaderSource,
                                         32768, // tells it to buffer this many samples ahead (this needs to be zero for it to play OOB)
                                         reader->sampleRate);
        
		// let our listeners know that we have loaded a new file
		audioTransportSource->sendChangeMessage();
		listeners.call (&Listener::fileChanged, this);

		if (shouldBePlaying)
			audioTransportSource->start();
		
		return true;
	}
	
	setLibraryEntry(ValueTree::invalid);
	
	audioTransportSource->sendChangeMessage();
	listeners.call (&Listener::fileChanged, this);

	return false;
}

void FilteringAudioFilePlayer::setLibraryEntry(ValueTree newEntry)
{
	libraryEntry = newEntry;
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
	if (currentAudioFormatReaderSource != nullptr)
		currentAudioFormatReaderSource->setLooping(shouldLoop);
}

void FilteringAudioFilePlayer::setResamplingRatio (const double samplesInPerOutputSample)
{
	resamplingAudioSource->setResamplingRatio(samplesInPerOutputSample);
	
	listeners.call (&Listener::resamplingRatioChanged, this);
}

//==============================================================================
void FilteringAudioFilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    if (masterSource != nullptr)
        masterSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
//    audioTransportSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void FilteringAudioFilePlayer::releaseResources()
{
    if (masterSource != nullptr)
        masterSource->releaseResources();
//    audioTransportSource->releaseResources();
}

void FilteringAudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
//    audioTransportSource->getNextAudioBlock(bufferToFill);
    if (masterSource != nullptr)
        masterSource->getNextAudioBlock(bufferToFill);
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
AudioFormatReader* FilteringAudioFilePlayer::audioFormatReaderFromFile(const String& path)
{
	File audioFile (path);
	fileName = audioFile.getFileName();
    
	return formatManager->createReaderFor (audioFile);
}

//==============================================================================

END_DROWAUDIO_NAMESPACE