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

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_StereoMeter.h"

StereoMeter::StereoMeter(float* leftBarValueToUse_, float* leftLineValueToUse_,
						 float* rightBarValueToUse_, float* rightLineValueToUse_)
	:	leftBarValueToUse(leftBarValueToUse_),
		leftLineValueToUse(leftLineValueToUse_),
		rightBarValueToUse(rightBarValueToUse_),
		rightLineValueToUse(rightLineValueToUse_)
{
	addAndMakeVisible( leftMeter = new MeterComponent (leftBarValueToUse, leftLineValueToUse) );
	addAndMakeVisible( rightMeter = new MeterComponent (rightBarValueToUse, rightLineValueToUse) );
	
	leftMeter->stopTimer();
	rightMeter->stopTimer();
	
	startTimer(50);
}

StereoMeter::~StereoMeter()
{
	deleteAllChildren();
}

void StereoMeter::resized()
{
	leftMeter->setBounds(0, 0, getWidth()*0.5f-1, getHeight());
	rightMeter->setBounds(getWidth()*0.5f+1, 0, getWidth()*0.5f-1, getHeight());
}

void StereoMeter::setUpdateInterval(int newUpdateInterval)
{
	startTimer(newUpdateInterval);
}

void StereoMeter::timerCallback()
{
	leftMeter->setMeterLevel(*leftBarValueToUse, *leftBarValueToUse);
	rightMeter->setMeterLevel(*rightBarValueToUse, *rightBarValueToUse);
}

END_DROWAUDIO_NAMESPACE