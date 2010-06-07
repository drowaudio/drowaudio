/*
 *  dRowAudio_SegmentedMeter.h
 *  Decks
 *
 *  Created by David Rowland on 15/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_SEGMENTEDMETER__H_
#define _DROWAUDIO_SEGMENTEDMETER__H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

class SegmentedMeter :	public Component,
						public Timer
{
public:
	SegmentedMeter();
	
	~SegmentedMeter();
	
	void paint (Graphics &g);
	
	void resized(){}
	void timerCallback();
	

	void processBlock (float* channelData, int numSamples);

	/**	Processes a number of samples to show on the meter.
		If the number of channels is just 1 a quick method of sampling will be used.
		If there are more than 1 channels the highest sample in any of the channels will be used.
	 */
    void processBlock (float** channelData, int numSamples, int numChannels);
	
	//================================================================
	int getTotalNoSegments()	{	return totalSegs;	}
	int getNoOverSegments()		{	return noRedSeg;	}
	int	getNoWarningSegments()	{	return noYellowSeg;	}
	int	getNoUnderSegments()	{	return noGreenSeg;	}
	float getNoDbPerSegment()	{	return 3;			}
	//================================================================

private:
	int noRedSeg, noYellowSeg, noGreenSeg, totalSegs;
	int currentNoSegs;
	int sampleCount, samplesToCount;
    float sampleMax, currentLevel, prevLevel;
};

#endif //_DROWAUDIO_SEGMENTEDMETER__H_