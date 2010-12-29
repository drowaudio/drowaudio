/*
 *  Mixer.h
 *  Decks
 *
 *  Created by David Rowland on 03/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKS_MIXER__H_
#define _DECKS_MIXER__H_

#include <dRowAudio/dRowAudio.h>
#include "../../DecksLookAndFeel.h"
#include "../../../main/Settings.h"
#include "../../../main/DeckManager.h"
#include "MixerChannelStrip.h"
#include "MasterChannelStrip.h"
#include "CrossFader.h"

class Mixer : public  Component
{
public:
	Mixer();
	
	~Mixer();
	
	//================================================================
	void resized();
	
	void moved();
	
	void paint(Graphics &g);
	
	const MasterChannelStrip* getMasterChannelStrip()	{	return masterStrip;	}

	void updateMeterForChannel(int deckNo, float **data, int numSamples, int numChannels);
	
	void updateMasterMeter(float **data, int numSamples, int numChannels);
	//================================================================

private:
	
	ScopedPointer<GraphicalComponentManager> meterManager;

	OwnedArray <MixerChannelStrip> channelStrips;
	CrossFader *xFader;
	MasterChannelStrip *masterStrip;
};

#endif //_DECKS_MIXER__H_