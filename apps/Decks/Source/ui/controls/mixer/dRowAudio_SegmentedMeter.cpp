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
	currentNoSegs(0),
	sampleCount(0),
	samplesToCount(2048),
	sampleMax(0.0f),
	currentLevel(0.0f),
	prevLevel(0.0f)
{
	setOpaque (true);
	startTimer (1000 / 50);
}

SegmentedMeter::~SegmentedMeter(){}

void SegmentedMeter::paint (Graphics &g)
{
	const int m = 2;
	const int w = getWidth();
	const int h = getHeight();
	const int segWidth = w-(2*m);
	
	
	const int noSeg = (noRedSeg + noYellowSeg + noGreenSeg);
	const float segHeight = (h-m) / noSeg;
	

	const int noReq = currentNoSegs;
	
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
		g.fillRect((float)m, h-(2*m)-(i*(segHeight)), (float)segWidth, segHeight);

		g.setColour(Colours::black);
		g.drawLine((float)m, h-(2*m)-(i*segHeight), w-m, h-(2*m)-(i*segHeight), m);
	}
	
	g.setColour(Colours::black);
	g.drawRect(0, 0, w, h, m);
}

void SegmentedMeter::timerCallback()
{
	currentNoSegs =  pow(currentLevel, 0.25) * (totalSegs-noRedSeg);
	currentLevel *= 0.9f;
	
	repaint();
}

void SegmentedMeter::processBlock (float* channelData, int numSamples)
{
	if (channelData != 0)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			float sample = fabsf(channelData[i]);
			
			if (sample > sampleMax)
				sampleMax = sample;
			
			if (++sampleCount == samplesToCount) {
				if (sampleMax > currentLevel)
					currentLevel = sampleMax;

				sampleMax = 0.0f;
				sampleCount = 0;
			}
		}
	}
}

/**	Processes a number of samples displaying them on the meter.
 */
void SegmentedMeter::processBlock (float** channelData,
								   int numSamples, int numChannels)
{
	if (channelData != 0)
	{
		if (numChannels == 1)
		{
			for (int i = 0; i < numSamples; ++i)
			{
				float sample = fabsf(channelData[0][i]);
				
				if (sample > sampleMax)
					sampleMax = sample;
				
				if (++sampleCount == samplesToCount) {
					if (sampleMax > currentLevel)
						currentLevel = sampleMax;
					sampleMax = 0.0f;
					sampleCount = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < numSamples; ++i)
			{
				float sample = 0.0f;
				for (int c=0; c<numChannels; c++) {
					if (sample < fabsf(channelData[c][i])) {
						sample = fabsf(channelData[c][i]);
					}
				}
				
				if (sample > sampleMax)
					sampleMax = sample;
				
				if (++sampleCount == samplesToCount) {
					if (sampleMax > currentLevel)
						currentLevel = sampleMax;
					sampleMax = 0.0f;
					sampleCount = 0;
				}
			}
		}

	}
}