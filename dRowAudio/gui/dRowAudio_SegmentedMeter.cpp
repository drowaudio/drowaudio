/*
 *  dRowAudio_SegmentedMeter.cpp
 *  Decks
 *
 *  Created by David Rowland on 15/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

SegmentedMeter::SegmentedMeter()
    : numRedSeg     (2),
      numYellowSeg  (4),
	  numGreenSeg   (9),
	  totalNumSegs  (numRedSeg + numYellowSeg + numGreenSeg),
	  decibelsPerSeg(3.0f),
	  numSegs       (0),
	  sampleCount   (0),
	  samplesToCount(2048),
	  sampleMax     (0.0f),
	  level         (0.0f),
	  needsRepaint  (true)
{
	setOpaque (true);
}

SegmentedMeter::~SegmentedMeter()
{
}

void SegmentedMeter::calculateSegments()
{
	float numDecibels = toDecibels (level.getCurrent());
	// map decibels to numSegs
	numSegs = jmax (0, roundToInt ((numDecibels / decibelsPerSeg) + (totalNumSegs - numRedSeg)));
	
	// impliment slow decay
	//	level.set((0.5f * level.getCurrent()) + (0.1f * level.getPrevious()));
	level *= 0.8;
	
	// only actually need to repaint if the numSegs has changed
	if (! numSegs.areEqual() || needsRepaint)
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
    
    const int numSegments = (numRedSeg + numYellowSeg + numGreenSeg);
	const float segmentHeight = (h - m) / (float) numSegments;
    const float segWidth = w - (2 * m);
	
	for (int i = 1; i <= numSegments; ++i)
	{
		if (i <= numGreenSeg)
		{
			gOn.setColour (Colours::green.brighter (0.8));
            gOff.setColour (Colours::green.darker());
		}
		else if (i <= (numYellowSeg + numGreenSeg))
		{
			gOn.setColour (Colours::orange.brighter());
            gOff.setColour (Colours::orange.darker());
		}
		else
		{
			gOn.setColour (Colours::red.brighter());
            gOff.setColour (Colours::red.darker());
		}

		gOn.fillRect ((float) m, h - m - (i * segmentHeight), segWidth, segmentHeight);
		gOn.setColour (Colours::black);
		gOn.drawLine ((float) m, h - m - (i * segmentHeight), w - m, h - m - (i * segmentHeight), m);
		
        gOff.fillRect ((float) m, h - m - (i * segmentHeight), segWidth, segmentHeight);
		gOff.setColour (Colours::black);
		gOff.drawLine ((float) m, h - m - (i * segmentHeight), w - m, h - m - (i * segmentHeight), m);
	}
	
	gOn.setColour (Colours::black);
	gOn.drawRect (0, 0, w, h, m);    

    gOff.setColour (Colours::black);
	gOff.drawRect (0, 0, w, h, m);    

    needsRepaint = true;
}

void SegmentedMeter::paint (Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();

    if (onImage.isValid()) 
    {
        const int onHeight = roundToInt ((numSegs.getCurrent() / (float) totalNumSegs) * onImage.getHeight());
        const int offHeight = h - onHeight;
        
//        g.drawImage (onImage,
//                     0, offHeight, w, onHeight, 
//                     0, offHeight, w, onHeight,
//                     false);
//
//        g.drawImage (offImage,
//                     0, 0, w, offHeight, 
//                     0, 0, w, offHeight,
//                     false);
        g.drawImage (onImage,
                     0, 0, w, h,
                     0, 0, w, h,
                     false);
        
        g.drawImage (offImage,
                     0, 0, w, offHeight, 
                     0, 0, w, offHeight,
                     false);
    }
    
    needsRepaint = false;
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

END_JUCE_NAMESPACE