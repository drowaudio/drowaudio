/*
 *  MasterChannelStrip.h
 *  Decks
 *
 *  Created by David Rowland on 03/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKS_MASTERCHANNELSTRIP__H_
#define _DECKS_MASTERCHANNELSTRIP__H_

#include <juce/juce.h>
#include "../../../ui/DecksColours.h"
#include "../../DecksLookAndFeel.h"
#include "../../../main/DeckManager.h"
#include "../../../main/DefaultSettings.h"

class MasterChannelStrip :	public Component
{
public:
	MasterChannelStrip();
	
	~MasterChannelStrip();
	
	//================================================================
	void resized();
	
	void paint(Graphics &g);
	
	//================================================================	
	SegmentedMeter* getMeterL()	{	return meterL;	}
	SegmentedMeter* getMeterR()	{	return meterR;	}

	//================================================================
	
private:
	
	SegmentedMeter *meterL, *meterR;
	TextButton *cueButton;
	Slider *gainSlider, *faderCurveSlider, *xFaderCurveSlider;
	Label *faderCurveLabel, *xFaderCurveLabel;
};

#endif //_DECKS_MASTERCHANNELSTRIP__H_