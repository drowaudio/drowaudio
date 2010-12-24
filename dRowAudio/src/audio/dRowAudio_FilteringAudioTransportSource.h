/*
 *  FilteringAudioTransportSource.h
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _FILTERINGAUDIOTRANSPORTSOURCE_H_
#define _FILTERINGAUDIOTRANSPORTSOURCE_H_

#include "../core/dRowAudio_StandardHeader.h"

/**	An AudioSource that contains three settable filters to EQ the audio stream.
	This also creates an internal resampler source that is adjustable.
	
 */
class FilteringAudioTransportSource : public PositionableAudioSource,
									  public ChangeBroadcaster
{

public:
	//==============================================================================
    /** Creates an FilteringAudioTransportSource.
	 
	 After creating one of these, use the setSource() method to select an input source.
	 */
    FilteringAudioTransportSource();
	
    /** Destructor. */
    ~FilteringAudioTransportSource();
	
    //==============================================================================
    /** Sets the reader that is being used as the input source.
	 
	 This will stop playback, reset the position to 0 and change to the new reader.
	 
	 The source passed in will not be deleted by this object, so must be managed by
	 the caller.
	 
	 @param newSource                        the new input source to use. This may be zero
	 @param readAheadBufferSize              a size of buffer to use for reading ahead. If this
	 is zero, no reading ahead will be done; if it's
	 greater than zero, a BufferingAudioSource will be used
	 to do the reading-ahead
	 @param sourceSampleRateToCorrectFor     if this is non-zero, it specifies the sample
	 rate of the source, and playback will be sample-rate
	 adjusted to maintain playback at the correct pitch. If
	 this is 0, no sample-rate adjustment will be performed
	 */
    void setSource (PositionableAudioSource* const newSource,
                    int readAheadBufferSize = 0,
                    double sourceSampleRateToCorrectFor = 0.0);
	
    //==============================================================================
    /** Changes the current playback position in the source stream.
	 
	 The next time the getNextAudioBlock() method is called, this
	 is the time from which it'll read data.
	 
	 @see getPosition
	 */
    void setPosition (double newPosition);
	
    /** Returns the position that the next data block will be read from
	 
	 This is a time in seconds.
	 */
    double getCurrentPosition() const;
	
    /** Returns true if the player has stopped because its input stream ran out of data.
	 */
    bool hasStreamFinished() const throw()              { return inputStreamEOF; }
	
    //==============================================================================
    /** Starts playing (if a source has been selected).
	 
	 If it starts playing, this will send a message to any ChangeListeners
	 that are registered with this object.
	 */
    void start();
	
    /** Stops playing.
	 
	 If it's actually playing, this will send a message to any ChangeListeners
	 that are registered with this object.
	 */
    void stop();
	
    /** Returns true if it's currently playing. */
    bool isPlaying() const throw()      { return playing; }
	
    //==============================================================================
    /** Changes the gain to apply to the output.
	 
	 @param newGain  a factor by which to multiply the outgoing samples,
	 so 1.0 = 0dB, 0.5 = -6dB, 2.0 = 6dB, etc.
	 */
    void setGain (const float newGain) throw();
	
    /** Returns the current gain setting.
	 
	 @see setGain
	 */
    float getGain() const throw()       { return gain; }
	
	/** Changes the gain of the lowShelfFilter to apply to the output.
	 */
	void setLowEQGain(float newLowEQGain);
	
	/** Changes the gain of the bandPassFilter to apply to the output.
	 */
	void setMidEQGain(float newMidEQGain);
	
	/** Changes the gain of the highShelfFilter to apply to the output.
	 */
	void setHighEQGain(float newHighEQGain);
	
	/** Toggles the filtering of the transport source.
	 */
	void setFilterSource(bool shouldFilter);

	/** Returns whether the source is being filtered or not.
	 */
	bool getFilterSource()				{ return filterSource; }
	
	/** Sets the resampling ratio to use.
	 */
	void setResamplingRatio (const double samplesInPerOutputSample);
	
	/** Return the current resampling ratio being used
	 */
	double getResamplingRatio()			{ return resamplingRatio; }
	
    //==============================================================================
    /** Implementation of the AudioSource method. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
	
    /** Implementation of the AudioSource method. */
    void releaseResources();
	
    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);
	
    //==============================================================================
    /** Implements the PositionableAudioSource method. */
    void setNextReadPosition (int newPosition);
	
    /** Implements the PositionableAudioSource method. */
    int getNextReadPosition() const;
	
    /** Implements the PositionableAudioSource method. */
    int getTotalLength() const;
	
    /** Implements the PositionableAudioSource method. */
    bool isLooping() const;
	
    //==============================================================================
    juce_UseDebuggingNewOperator
	
private:
    PositionableAudioSource* source;
    ResamplingAudioSource* resamplerSource;
    BufferingAudioSource* bufferingSource;
    PositionableAudioSource* positionableSource;
    AudioSource* masterSource;
	
    CriticalSection callbackLock;
    float volatile gain, lastGain;
    bool volatile playing, stopped;
    double sampleRate, sourceSampleRate, resamplingRatio;
    int blockSize, readAheadBufferSize;
    bool isPrepared, inputStreamEOF;
	
    FilteringAudioTransportSource (const FilteringAudioTransportSource&);
    const FilteringAudioTransportSource& operator= (const FilteringAudioTransportSource&);
	
	// filter members
	IIRFilter *lowEQFilterLeft, *midEQFilterLeft, *highEQFilterLeft;
	IIRFilter *lowEQFilterRight, *midEQFilterRight, *highEQFilterRight;
	
	float lowEQGain, midEQGain, highEQGain;
	bool filterSource;
	
	JUCE_LEAK_DETECTOR (FilteringAudioTransportSource);
};

#endif //_FILTERINGAUDIOTRANSPORTSOURCE_H_