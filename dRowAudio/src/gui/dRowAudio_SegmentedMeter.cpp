/*
 *  dRowAudio_SegmentedMeter.cpp
 *  Decks
 *
 *  Created by David Rowland on 15/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_SegmentedMeter.h"

SegmentedMeter::SegmentedMeter()
:	noRedSeg(2),
	noYellowSeg(4),
	noGreenSeg(9),
	totalSegs(noRedSeg + noYellowSeg + noGreenSeg),
	decibelsPerSeg(3.0f),
	noSegs(1),
	sampleCount(0),
	samplesToCount(2048),
	sampleMax(0.0f),
	level(0.0f),
	needsRepaint(true)
{
	setOpaque (true);
}

SegmentedMeter::~SegmentedMeter(){}

void SegmentedMeter::paint (Graphics &g)
{
//	noSegs = pow(level.getCurrent(), 0.25) * (totalSegs-noRedSeg);

	float noDecibels = 20*log10(level.getCurrent());
	// map decibels to noSegs
	noSegs = roundToInt((noDecibels / decibelsPerSeg) + (((totalSegs-noRedSeg)*decibelsPerSeg) / decibelsPerSeg));
		
	// decay meter levels
//	level *= 0.9f;
	
	

	// only actually need to repaint if the noSegs has changed
	if (((noSegs.getCurrent() >= 0) && !noSegs.areEqual()) || needsRepaint)
	{
		needsRepaint = false;
				
		const int m = 2;
		const int w = getWidth();
		const int h = getHeight();
		const int segWidth = w-(2*m);
		
		
		const int noSeg = (noRedSeg + noYellowSeg + noGreenSeg);
		const float segHeight = (h-m) / (float)noSeg;
		

		const int noReq = noSegs.getCurrent();
		
		for (int i=1; i <= noSeg; i++)
		{
			if (i <= noGreenSeg)
			{
				i <= noReq ? g.setColour(Colours::green.brighter(0.8))
							: g.setColour(Colours::green.darker());
			}
			else if (i <= (noYellowSeg+noGreenSeg))
			{
				i <= noReq ? g.setColour(Colours::orange.brighter())
							: g.setColour(Colours::orange.darker());
			}
			else
			{
				i <= noReq ? g.setColour(Colours::red.brighter())
							: g.setColour(Colours::red.darker());
			}
			g.fillRect((float)m, h-m-(i*(segHeight)), (float)segWidth, segHeight);

			g.setColour(Colours::black);
			g.drawLine((float)m, h-m-(i*segHeight), w-m, h-m-(i*segHeight), m);
		}
		
		g.setColour(Colours::black);
		g.drawRect(0, 0, w, h, m);
	}
	
	// impliment slow decay
//	level.set((0.5f * level.getCurrent()) + (0.1f * level.getPrevious()));
	level *= 0.7;
}

void SegmentedMeter::process()
{
	// calculate current meter level
	if (samples.getData() != 0)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			float sample = fabsf(samples[i]);
			
			if (sample > sampleMax)
				sampleMax = sample;
			
			if (++sampleCount == samplesToCount) {
				if (sampleMax > level.getCurrent())
					level = sampleMax;
				
				sampleMax = 0.0f;
				sampleCount = 0;
			}
		}
	}
}
//void SegmentedMeter::timerCallback()
//{
//	noSegs =  pow(currentLevel, 0.25) * (totalSegs-noRedSeg);
//	currentLevel *= 0.9f;
//	
//	repaint();
//}

//void SegmentedMeter::processBlock (float* channelData, int numSamples)
//{
//	if (channelData != 0)
//	{
//		for (int i = 0; i < numSamples; ++i)
//		{
//			float sample = fabsf(channelData[i]);
//			
//			if (sample > sampleMax)
//				sampleMax = sample;
//			
//			if (++sampleCount == samplesToCount) {
//				if (sampleMax > currentLevel)
//					currentLevel = sampleMax;
//
//				sampleMax = 0.0f;
//				sampleCount = 0;
//			}
//		}
//	}
//}
//
///**	Processes a number of samples displaying them on the meter.
// */
//void SegmentedMeter::processBlock (float** channelData,
//								   int numSamples, int numChannels)
//{
//	if (channelData != 0)
//	{
//		if (numChannels == 1)
//		{
//			for (int i = 0; i < numSamples; ++i)
//			{
//				float sample = fabsf(channelData[0][i]);
//				
//				if (sample > sampleMax)
//					sampleMax = sample;
//				
//				if (++sampleCount == samplesToCount) {
//					if (sampleMax > currentLevel)
//						currentLevel = sampleMax;
//					sampleMax = 0.0f;
//					sampleCount = 0;
//				}
//			}
//		}
//		else
//		{
//			for (int i = 0; i < numSamples; ++i)
//			{
//				float sample = 0.0f;
//				for (int c=0; c<numChannels; c++) {
//					if (sample < fabsf(channelData[c][i])) {
//						sample = fabsf(channelData[c][i]);
//					}
//				}
//				
//				if (sample > sampleMax)
//					sampleMax = sample;
//				
//				if (++sampleCount == samplesToCount) {
//					if (sampleMax > currentLevel)
//						currentLevel = sampleMax;
//					sampleMax = 0.0f;
//					sampleCount = 0;
//				}
//			}
//		}
//
//	}
//}