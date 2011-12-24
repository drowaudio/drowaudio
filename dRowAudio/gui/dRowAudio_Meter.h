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

#ifndef _METERCOMPONENT_H_
#define _METERCOMPONENT_H_

#include "../core/dRowAudio_StandardHeader.h"

/**	This is a class used to display a meter component.
	
	The slow decay and logarithmic scaling is implimented internally 
	so all you need to do is pass it some pointers to the Bar and Line levels to 
	display. A critical section is also required to avoid corruptind data from different threads.
 */
class MeterComponent	:	public Component,
							public Timer
{	
public:
	
	/** Create the meter component.
		The default refresh rate is 50ms but this can be changed with setUpdateInterval()
		
		@param barValueToUse	A pointer to the value for the bar to display
		@param lineValueToUse	A pointer to the value for the line to display
		@param lockToUse		An optional pointer to a Critical Section to use.
	 */
	MeterComponent(float* barValueToUse, float* lineValueToUse, const CriticalSection* lockToUse =0);
	/// Destructor
	~MeterComponent();
	
	//========================================
	void paint(Graphics &g);
	
	void resized();
	
	void timerCallback();
	//========================================
	/**	Changes the refresh rate of the meter.
		This can be used to change how frequently the meters update.
		This could be useful for turning them off or slowing them down to release some resources.
	 */
	void setUpdateInterval(const int intervalInMilliseconds);
	
	/** Sets the new level of the meter.
		This can be called to explicitly set the meter levels although this is not recomended.
	 */
	void setMeterLevel(const float newBarLevel, const float newLineLevel =0.0f);
	//========================================

private:
	
	float* pCurrentBarLevel;
	float* pCurrentLineLevel;
	const CriticalSection* lock;
	CriticalSection* dummyLock;

	float currentBarLevel, prevBarLevel;
	float currentLineLevel, prevLineLevel;
	
	
	unsigned short lineCounter;
	bool dropLineLevel;
	bool clipped;
	bool deleteDummyLock;
	
	JUCE_LEAK_DETECTOR (MeterComponent);
};

#endif //_METERCOMPONENT_H_