/*
 *  dRowAudio_CpuMeter.h
 *  dRowAudio
 *
 *  Created by David Rowland on 11/09/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_CPUMETER_H_
#define _DROWAUDIO_CPUMETER_H_

#include <juce/juce.h>

/**
	Handy class that will display the cpu usage of a given AudioDeviceManager as a percentage.
 */
class CpuMeter : public Label,
				 public Timer
{
public:
	
	/**	Constructor.
		You need to provide the device manager to monitor and optionally the refresh rate of the display.
	 */
	CpuMeter(AudioDeviceManager* deviceManagerToUse, int updateInterval =50);
	
	///	Descructor.
	~CpuMeter();
	
	/// @internal
	void resized();

	/// @internal
	void timerCallback();

	/// Returns the current cpu usage as a percentage.
	double getCurrentCpuUsage()	{	return currentCpuUsage;	}
	
	/// Changes the colour of the text.
	void setTextColour(const Colour &newTextColour)	{	setColour(Label::textColourId, newTextColour);	}
	
private:
	
	AudioDeviceManager* deviceManager;
	int updateInterval_;
	double currentCpuUsage;
};

#endif	//_DROWAUDIO_CPUMETER_H_