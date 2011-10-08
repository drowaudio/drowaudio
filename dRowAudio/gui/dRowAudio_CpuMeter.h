/*
 *  dRowAudio_CpuMeter.h
 *  dRowAudio
 *
 *  Created by David Rowland on 11/09/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_CPUMETER_H__
#define __DROWAUDIO_CPUMETER_H__

//==============================================================================
/**
	Handy class that will display the cpu usage of a given AudioDeviceManager
    as a percentage.
 */
class CpuMeter : public Label,
				 public Timer
{
public:
    //==============================================================================
	/**	Creates a CpuMeter.
		You need to provide the device manager to monitor and optionally the refresh
        rate of the display.
	 */
	CpuMeter (AudioDeviceManager* deviceManagerToUse, int updateIntervalMs = 50);
	
	/**	Descructor.
     */
	~CpuMeter();
	
	/** Returns the current cpu usage as a percentage.
     */
	double getCurrentCpuUsage()                         {	return currentCpuUsage;                         }
	
	/** Changes the colour of the text.
     */
	void setTextColour (const Colour &newTextColour)    {	setColour (Label::textColourId, newTextColour);	}

	//==============================================================================
    /** @internal */
	void resized();

	/** @internal */
	void timerCallback();
	
private:
    //==============================================================================
	AudioDeviceManager* deviceManager;
	int updateInterval;
	double currentCpuUsage;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CpuMeter);
};

#endif	//__DROWAUDIO_CPUMETER_H__