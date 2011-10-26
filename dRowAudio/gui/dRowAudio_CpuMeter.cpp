/*
 *  dRowAudio_CpuMeter.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 11/09/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

CpuMeter::CpuMeter (AudioDeviceManager* deviceManagerToUse, int updateIntervalMs)
	: Label ("CpuMeter", "00.00%"),
	  deviceManager (deviceManagerToUse),
	  updateInterval (updateIntervalMs),
	  currentCpuUsage(0.0)
{
	if (deviceManagerToUse != nullptr)
		startTimer (updateInterval);
}

CpuMeter::~CpuMeter()
{
}

void CpuMeter::resized()
{
    const int w = getWidth();
    const int h = getHeight();

	setFont ((h < (w * 0.24) ? h : w * 0.24));
}

void CpuMeter::timerCallback()
{
	currentCpuUsage = (deviceManager->getCpuUsage() * 100.0);
	String displayString (currentCpuUsage, 2);
	displayString << "%";
    
	setText (displayString, false);
}

END_JUCE_NAMESPACE