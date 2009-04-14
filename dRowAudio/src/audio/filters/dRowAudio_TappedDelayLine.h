/*
 *  dRowAudio_TappedDelayLine.h
 *
 *  Created by David Rowland on 13/04/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_TAPPEDDELAYLINE_H_
#define _DROWAUDIO_TAPPEDDELAYLINE_H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

struct Tap
{
	int delaySamples;
	int originalDelaySamples;
	int sampleRateWhenCreated;
	float tapGain;
	float tapFeedback;
	float originalTapFeedback;
/*	float tapPan;
	float tapHPCutoff;
	float tapLPCutoff;
 */
};

class TappedDelayLine
{
public:
	TappedDelayLine(int initialBufferSize =96000);
	TappedDelayLine(float bufferLengthMs, double sampleRate);
	
	/// Destructor
	~TappedDelayLine();
	
	/** Adds a tap to the delay line at a given number of samples.
		This will not make a note of the current sample rate being used
		unless you explecity specify it. Use
		addTap(int newTapPosMs, double sampleRate) if you need the delay
		to be dependant on time.
	 */
	void addTap(int noDelaySamples, int sampleRate =0);
	
	/** Adds a tap to the delay line at a given time.
		If the sample rate changes make sure you call updateDelayTimes()
		to recalculate the number of samples for the taps to delay by.
	 */
	void addTapAtTime(int newTapPosMs, double sampleRate);
	
	/**	Scales the spacing between the taps.
		This value must be greater than 0. Values < 1 will squash the taps, 
		creating a denser delay, values greater than 1 will expand the taps
		spacing creating a more sparse delay.
		The value is a proportion of the original spacing.
		This is simple than namually setting all of the taps positions.
	 */
	void setTapSpacing(float newSpacingCoefficient);

	/**	Scales all of the taps feedback coeficients in one go.
		This should be between 0 and 1 to avoid blowing up the line.
		The value is a proportion of the original feedback coefficient
		for each tap so setting this to 1 will return them all to their default.
	 */
	void scaleFeedbacks(float newFeedbackCoefficient);

	/**	Returns an array of sample positions where there are taps.
		This can then be used to remove a specific tap.
	 */
	Array<int> getTapSamplePositions();
	
	/** Removes a tap at a specific index.
		Returns true if a tap was removed.
	 */
	bool removeTapAtIndex(int tapIndex);
	
	/** Removes a tap with a specific delay samples.
		Returns true if a tap was revoved, false otherwise.
	 */
	bool removeTapAtSample(int sampleForRemovedTap);
	
	/** Attempts to remove a tap at a specific time.
		Returns true if a tap was revoved, false otherwise.
	 */ 
	bool removeTapAtMs(int timeMsForRemovedTap, int sampleRate);
	
	/**	Removes all the taps.
	 */
	void removeAllTaps();
	
	/**	Updates the delay samples of all the taps based on their time.
		Call this if you change sample rates to make sure the taps are
		still positioned at the right time. 
	 */
	void updateDelayTimes(double newSampleRate);
	
	float processSingleSample(float newSample) throw();
	
private:
	
	float *pfDelayBuffer;
	int bufferSize, bufferWritePos;
	
	float inputGain, feedbackGain;
	int noTaps;
	Array<Tap> readTaps;
	
	float spacingCoefficient, feedbackCoefficient;
	
	void initialiseBuffer(int bufferSize);
};

#endif //_DROWAUDIO_TAPPEDDELAYLINE_H_