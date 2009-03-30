/*
 *  dRowAudio_StereoMeter.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 27/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

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