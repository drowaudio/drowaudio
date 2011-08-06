/*
 *  dRowAudio_SegmentedMeter.cpp
 *  Decks
 *
 *  Created by David Rowland on 15/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_SegmentedMeter.h"

SegmentedMeter::SegmentedMeter()
:	noRedSeg(2),
	noYellowSeg(4),
	noGreenSeg(9),
	totalSegs(noRedSeg + noYellowSeg + noGreenSeg),
	decibelsPerSeg(3.0f),
	noSegs(0),
	sampleCount(0),
	samplesToCount(2048),
	sampleMax(0.0f),
	level(0.0f),
	needsRepaint(true)
{
	setOpaque (true);
}

SegmentedMeter::~SegmentedMeter(){}

void SegmentedMeter::calculateSegments()
{
	float numDecibels = toDecibels (level.getCurrent());
	// map decibels to noSegs
	noSegs = jmax(0, roundToInt((numDecibels / decibelsPerSeg) + (totalSegs - noRedSeg)));
	
	// impliment slow decay
	//	level.set((0.5f * level.getCurrent()) + (0.1f * level.getPrevious()));
	level *= 0.8;
	
	// only actually need to repaint if the noSegs has changed
	if (! noSegs.areEqual() || needsRepaint)
		repaint();
}

void SegmentedMeter::timerCallback()
{
	calculateSegments();
}

void SegmentedMeter::resized()
{
	const int m = 2;
	const int w = getWidth();
	const int h = getHeight();

    onImage = Image (Image::RGB,
                     w, h,
                     false);
    offImage = Image (Image::RGB,
                      w, h,
                      false);
    
    Graphics gOn (onImage);
    Graphics gOff (offImage);
    
    const int numSegments = (noRedSeg + noYellowSeg + noGreenSeg);
	const float segmentHeight = (h - m) / (float)numSegments;
    const int segWidth = w - (2 * m);
	
	for (int i = 1; i <= numSegments; ++i)
	{
		if (i <= noGreenSeg)
		{
			gOn.setColour(Colours::green.brighter(0.8));
            gOff.setColour(Colours::green.darker());
		}
		else if (i <= (noYellowSeg+noGreenSeg))
		{
			gOn.setColour(Colours::orange.brighter());
            gOff.setColour(Colours::orange.darker());
		}
		else
		{
			gOn.setColour(Colours::red.brighter());
            gOff.setColour(Colours::red.darker());
		}

		gOn.fillRect((float)m, h-m-(i*(segmentHeight)), (float)segWidth, segmentHeight);
		gOn.setColour(Colours::black);
		gOn.drawLine((float)m, h-m-(i*segmentHeight), w-m, h-m-(i*segmentHeight), m);
		
        gOff.fillRect((float)m, h-m-(i*(segmentHeight)), (float)segWidth, segmentHeight);
		gOff.setColour(Colours::black);
		gOff.drawLine((float)m, h-m-(i*segmentHeight), w-m, h-m-(i*segmentHeight), m);
	}
	
	gOn.setColour(Colours::black);
	gOn.drawRect(0, 0, w, h, m);    

    gOff.setColour(Colours::black);
	gOff.drawRect(0, 0, w, h, m);    

    needsRepaint = true;
}

void SegmentedMeter::paint (Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();

    if (onImage.isValid()) 
    {
        const int onHeight = roundToInt ((noSegs.getCurrent() / (float)totalSegs) * onImage.getHeight());
        const int offHeight = h - onHeight;
        
        g.drawImage(onImage,
                    0, offHeight, w, onHeight, 
                    0, offHeight, w, onHeight,
                    false);

        g.drawImage(offImage,
                    0, 0, w, offHeight, 
                    0, 0, w, offHeight,
                    false);
    }
    
    needsRepaint = false;

//	for (int i=1; i <= noSeg; i++)
//	{
//		if (i <= noGreenSeg)
//		{
//			i <= noReq ? g.setColour(Colours::green.brighter(0.8))
//						: g.setColour(Colours::green.darker());
//		}
//		else if (i <= (noYellowSeg+noGreenSeg))
//		{
//			i <= noReq ? g.setColour(Colours::orange.brighter())
//						: g.setColour(Colours::orange.darker());
//		}
//		else
//		{
//			i <= noReq ? g.setColour(Colours::red.brighter())
//						: g.setColour(Colours::red.darker());
//		}
//		g.fillRect((float)m, h-m-(i*(segHeight)), (float)segWidth, segHeight);
//
//		g.setColour(Colours::black);
//		g.drawLine((float)m, h-m-(i*segHeight), w-m, h-m-(i*segHeight), m);
//	}
//	
//	g.setColour(Colours::black);
//	g.drawRect(0, 0, w, h, m);
}

void SegmentedMeter::process()
{
	// calculate current meter level
	if (samples.getData() != nullptr)
	{
		for (int i = 0; i < numSamples; ++i)
		{
			float sample = fabsf (samples[i]);
			
			if (sample > sampleMax)
				sampleMax = sample;
			
			if (++sampleCount == samplesToCount) 
            {
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

END_DROWAUDIO_NAMESPACE