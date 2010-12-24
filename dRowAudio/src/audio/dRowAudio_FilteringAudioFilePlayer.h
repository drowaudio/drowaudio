/*
 *  FilteringAudioFilePlayer.h
 *  resampling_test
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _FILTERINGAUDIOFILEPLAYER_H_
#define _FILTERINGAUDIOFILEPLAYER_H_

#include "../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_FilteringAudioTransportSource.h"
#include "dRowAudio_ReversableAudioFormatReaderSource.h"
#include "MADAudioFormat.h"

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
	AudioFormatManager* getAudioFormatManager() { return formatManager; }
	
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
	
	void setLooping(bool shouldLoop);
	
//	void setPlayDirection(bool shouldPlayForwards)
//	{	
//		if (currentAudioFileSource != 0) {
//			currentAudioFileSource->setPlayDirection(shouldPlayForwards);
//		}
//	}
//	bool getPlayDirection()
//	{
//		if (currentAudioFileSource != 0) {
//			return currentAudioFileSource->getPlayDirection();
//		}	
//	}
	
private:	
	/// The AudioFormatManager
	ScopedPointer<AudioFormatManager> formatManager;

	/// Create the reader from a given path
	AudioFormatReader* audioFormatReaderFromFile(const String& path);
	
	/// Create the actual stream that's going to read from the audio file
	AudioFormatReaderSource* currentAudioFileSource;
	
	String filePath;
	String fileName;
	bool shouldBePlaying;
	
	JUCE_LEAK_DETECTOR (FilteringAudioFilePlayer);
};

//#include <juce/juce.h>
//#include "dRowAudio_AudioFilePlayer.h"
//
//class FilteringAudioFilePlayer	:	public AudioFilePlayer
//{
//public:
//	FilteringAudioFilePlayer();
//	
//	~FilteringAudioFilePlayer();
//	
//	/** Changes the gain of the lowShelfFilter to apply to the output.
//	 */
//	void setLowEQGain(float newLowEQGain);
//	
//	/** Changes the gain of the bandPassFilter to apply to the output.
//	 */
//	void setMidEQGain(float newMidEQGain);
//	
//	/** Changes the gain of the highShelfFilter to apply to the output.
//	 */
//	void setHighEQGain(float newHighEQGain);
//	
//	/** Toggles the filtering of the transport source.
//	 */
//	void setFilterSource(bool shouldFilter);
//	
//	/** Returns whether the source is being filtered or not.
//	 */
//	bool getFilterSource()				{ return filterSource; }
//	
//	void setResamplingRatio (double resamplingRatio);
//	
////========================================================================
//	/** Implementation of the AudioSource method. */
//    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
//		
//    /** Implementation of the AudioSource method. */
//    void releaseResources();
//	
//    /** Implementation of the AudioSource method. */
//    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
//
////========================================================================
//	
//private:
//	
//	// resampler methods
//	double currentResamplingRatio;
//	
//	// filter members
//	IIRFilter *lowEQFilterLeft, *midEQFilterLeft, *highEQFilterLeft;
//	IIRFilter *lowEQFilterRight, *midEQFilterRight, *highEQFilterRight;
//	
//	double currentSampleRate;
//	float lowEQGain, midEQGain, highEQGain;
//	bool filterSource;
//	
//};

#endif //_FILTERINGAUDIOFILEPLAYER_H_