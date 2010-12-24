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

#include "../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_GraphicalComponent.h"
#include "../utility/dRowAudio_StateVariable.h"

/**	A segmented graphical VU meter.
	This class is a very efficient way of creating meters as it will only repaint itself when
	necessarry and does all its processing on a shared background thread.
 
	It is very customisable letting you set the number of each segments, how many decibels
	each segment represents and the colours of the segments.
 
	To use one, register it with a GraphicalComponentManager and then in your audio callback
	push some values to it with copyValues(...).
 
	Eg. in your Component
	@code
		graphicalManager = new GraphicalComponentManager();
		addAndMakeVisible(meter = new SegmentedMeter());
		graphicalManager->addGraphicalComponent(meter);
	@endcode
 
	and in your audioCallback
	@code
		if (currentMeter != 0) {
			currentMeter->copyValues(outputChannelData[0], numSamples);
		}
	@endcode
 
 */
class SegmentedMeter :	public GraphicalComponent
{
public:
	/**	Creates a SegmentedMeter.
		Initially this will do nothing as you need to register it with a GraphicalComponentManager
		then push some values to it with copyValues().
	 */
	SegmentedMeter();
	
	/**	Destructor.
	 */
	~SegmentedMeter();
	
	/**	Calculates the number of segments currently required and triggers a repaint if necessary.
	 */
	void calculateSegments();
	
	/**	Draws the meter.
	 */
	void paint (Graphics &g);
	
	/**	@internal
	 */
	void timerCallback();
	
	/**	@internal
	 */
	void resized()
	{
		needsRepaint = true;
	}

	/**	@internal
	 */
	void moved()
	{
		needsRepaint = true;
	}
		
	/**	Processes the channel data for the value to display.
	 */
	virtual void process();
	
//	void processBlock (float* channelData, int numSamples);
//
//	/**	Processes a number of samples to show on the meter.
//		If the number of channels is just 1 a quick method of sampling will be used.
//		If there are more than 1 channels the highest sample in any of the channels will be used.
//	 */
//    void processBlock (float** channelData, int numSamples, int numChannels);
	
	//================================================================
	/**	Returns the total number of segments the meter has.
	 */
	int getTotalNoSegments()	{	return totalSegs;	}
	
	/**	Returns the number of segments that represent the over level.
	 */
	int getNoOverSegments()		{	return noRedSeg;	}

	/**	Returns the total number of segments that represent the warning level.
	 */
	int	getNoWarningSegments()	{	return noYellowSeg;	}

	/**	Returns the total number of segments that represent the safe level.
	 */
	int	getNoSafeSegments()	{	return noGreenSeg;	}
	
	/**	Returns the number of decibels each segment represents.
	 */
	float getNoDbPerSegment()	{	return decibelsPerSeg;			}

	/**	Sets the number of decibels each segment represents.
	 */
	void setNoDecibelsPerSeg(float numDecibelsPerSegment)
	{
		decibelsPerSeg = numDecibelsPerSegment;
	}
	
	/**	Forces the meter to repaint itself.
		You may need to use this if a container component moves without moving
		or resizing its parent directly, eg. if you are housing your component
		in a tabbed component.
	 */
	void flagForRepaint()
	{	
		needsRepaint = true;
		repaint();
	}
	//================================================================

private:
	int noRedSeg, noYellowSeg, noGreenSeg, totalSegs;
	float decibelsPerSeg;
	StateVariable<int> noSegs;
	int sampleCount, samplesToCount;
    float sampleMax;
	StateVariable<float> level;
	bool needsRepaint;
	
	JUCE_LEAK_DETECTOR (SegmentedMeter);
};

#endif //_DROWAUDIO_SEGMENTEDMETER__H_