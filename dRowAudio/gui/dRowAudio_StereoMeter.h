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

#ifndef _DROWAUDIOSTEREOMETER_H_
#define _DROWAUDIOSTEREOMETER_H_

#include "../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_Meter.h"

class StereoMeter	:	public Component,
						public Timer
{
public:
	
	StereoMeter(float* leftBarValueToUse_, float* leftLineValueToUse_,
				float* rightBarValueToUse_, float* rightLineValueToUse_);
	
	~StereoMeter();
	
	void resized();
	
	void setUpdateInterval(int newUpdateInterval);
	
	void timerCallback();
	
private:
	MeterComponent *leftMeter, *rightMeter;
	float *leftBarValueToUse, *leftLineValueToUse;
	float *rightBarValueToUse, *rightLineValueToUse;
	
	JUCE_LEAK_DETECTOR (StereoMeter);
};

#endif //_DROWAUDIOSTEREOMETER_H_