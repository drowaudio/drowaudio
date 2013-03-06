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

#ifndef __DROWAUDIO_CPUMETER_H__
#define __DROWAUDIO_CPUMETER_H__

//==============================================================================
/** Handy class that will display the cpu usage of a given AudioDeviceManager
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
	double getCurrentCpuUsage() const
    {
        return currentCpuUsage;
    }
	
	/** Changes the colour of the text.
     */
	void setTextColour (const Colour& newTextColour)
    {
        setColour (Label::textColourId, newTextColour);
    }

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
	
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CpuMeter);
};

#endif	//__DROWAUDIO_CPUMETER_H__