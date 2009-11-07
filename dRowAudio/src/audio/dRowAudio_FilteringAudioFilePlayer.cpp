/*
 *  FilteringAudioFilePlayer.cpp
 *  resampling_test
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
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

//FilteringAudioFilePlayer::FilteringAudioFilePlayer()
//	:	currentSampleRate(44100.0),
//		filterSource(true)
//{	
//	// instantiate the filters
//	lowEQFilterLeft = new IIRFilter();
//	lowEQFilterRight = new IIRFilter();
//	midEQFilterLeft = new IIRFilter();
//	midEQFilterRight = new IIRFilter();
//	highEQFilterLeft = new IIRFilter();
//	highEQFilterRight = new IIRFilter();
//
//	lowEQFilterLeft->makeLowShelf(currentSampleRate, 70, 1.5, lowEQGain);
//	lowEQFilterRight->makeLowShelf(currentSampleRate, 70, 1.5, lowEQGain);
//	midEQFilterLeft->makeBandPass(currentSampleRate, 1000, 1.5, midEQGain);
//	midEQFilterRight->makeBandPass(currentSampleRate, 1000, 1.5, midEQGain);
//	highEQFilterLeft->makeHighShelf(currentSampleRate, 1300, 1.5, highEQGain);
//	highEQFilterRight->makeHighShelf(currentSampleRate, 1300, 1.5, highEQGain);	
//}
//
//FilteringAudioFilePlayer::~FilteringAudioFilePlayer()
//{
//	deleteAndZero(lowEQFilterLeft);
//	deleteAndZero(midEQFilterLeft);
//	deleteAndZero(highEQFilterLeft);
//	deleteAndZero(lowEQFilterRight);
//	deleteAndZero(midEQFilterRight);
//	deleteAndZero(highEQFilterRight);
//}
//
//void FilteringAudioFilePlayer::setLowEQGain(float newLowEQGain)
//{
//	lowEQGain = newLowEQGain;
//	
//	lowEQFilterLeft->makeLowShelf(currentSampleRate, 500, 1, lowEQGain);
//	lowEQFilterRight->makeLowShelf(currentSampleRate, 500, 1, lowEQGain);
//}
//
//void FilteringAudioFilePlayer::setMidEQGain(float newMidEQGain)
//{
//	midEQGain = newMidEQGain;
//	
//	midEQFilterLeft->makeBandPass(currentSampleRate, 2000, 1, midEQGain);
//	midEQFilterRight->makeBandPass(currentSampleRate, 2000, 1, midEQGain);
//}
//
//void FilteringAudioFilePlayer::setHighEQGain(float newHighEQGain)
//{
//	highEQGain = newHighEQGain;
//	
//	highEQFilterLeft->makeHighShelf(currentSampleRate, 3500, 1, highEQGain);
//	highEQFilterRight->makeHighShelf(currentSampleRate, 3500, 1, highEQGain);	
//}
//
//void FilteringAudioFilePlayer::setFilterSource(bool shouldFilter)
//{
//	filterSource = shouldFilter;
//}
//
//void FilteringAudioFilePlayer::setResamplingRatio (double resamplingRatio)
//{
//	currentResamplingRatio = resamplingRatio;
//	setSource(getAudioFormatReaderSource(), 32768, currentSampleRate*resamplingRatio);
//}
//
///** Implementation of the AudioSource method. */
//void FilteringAudioFilePlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
//{
//	currentSampleRate = sampleRate;
//	setLowEQGain(lowEQGain);
//	setMidEQGain(midEQGain);
//	setHighEQGain(highEQGain);
//	
//	AudioTransportSource::prepareToPlay (samplesPerBlockExpected, sampleRate);
//}
//
//void FilteringAudioFilePlayer::releaseResources()
//{
//	AudioTransportSource::releaseResources();
//}
//
///** Implementation of the AudioSource method. */
//void FilteringAudioFilePlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
//{
//	AudioTransportSource::getNextAudioBlock (bufferToFill);
//	
//	if (filterSource)
//	{
//		// filter samples
//		const int bufferNumSamples = bufferToFill.numSamples;
//		float *leftSampleData = bufferToFill.buffer->getSampleData(0, bufferToFill.startSample);
//		float *rightSampleData = bufferToFill.buffer->getSampleData(1, bufferToFill.startSample);
//		
//		lowEQFilterLeft->processSamples(leftSampleData, bufferNumSamples);
//		lowEQFilterRight->processSamples(rightSampleData, bufferNumSamples);
//		midEQFilterLeft->processSamples(leftSampleData, bufferNumSamples);
//		midEQFilterRight->processSamples(rightSampleData, bufferNumSamples);
//		highEQFilterLeft->processSamples(leftSampleData, bufferNumSamples);
//		highEQFilterRight->processSamples(rightSampleData, bufferNumSamples);
//	}	
//}
