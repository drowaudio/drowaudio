/*
 *  dRowAudio_CpuMeter.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 11/09/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_CpuMeter.h"

CpuMeter::CpuMeter(AudioDeviceManager* deviceManagerToUse, int updateInterval)
	:	Label(T("CpuMeter"), T("00.00%")),
		deviceManager(deviceManagerToUse),
		updateInterval_(updateInterval),
		currentCpuUsage(0.0)
{
	if(deviceManagerToUse)
		startTimer(updateInterval_);
}

CpuMeter::~CpuMeter()
{
}


void CpuMeter::resized()
{
	setFont( ((getHeight() < getWidth()*0.24) ? getHeight() : getWidth()*0.24) );
}

void CpuMeter::timerCallback()
{
	currentCpuUsage = (deviceManager->getCpuUsage() * 100.0);
	setText(String(currentCpuUsage, 2) <<T("%"), false);
}