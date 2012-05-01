/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/



CpuMeter::CpuMeter (AudioDeviceManager* deviceManagerToUse, int updateIntervalMs)
	: Label ("CpuMeter", "00.00%"),
	  deviceManager (deviceManagerToUse),
	  updateInterval (updateIntervalMs),
	  currentCpuUsage (0.0)
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

	setFont ((h < (w * 0.24f) ? h : w * 0.24f));
}

void CpuMeter::timerCallback()
{
	currentCpuUsage = (deviceManager->getCpuUsage() * 100.0);
	String displayString (currentCpuUsage, 2);
	displayString << "%";
    
	setText (displayString, false);
}

