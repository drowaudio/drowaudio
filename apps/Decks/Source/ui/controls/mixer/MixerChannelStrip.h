/*
 *  MixerChannelStrip.h
 *  Decks
 *
 *  Created by David Rowland on 03/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKS_MIXERCHANNELSTRIP__H_
#define _DECKS_MIXERCHANNELSTRIP__H_

#include <dRowAudio/dRowAudio.h>
#include "../../DecksLookAndFeel.h"
#include "../../../main/Settings.h"
#include "../../../main/DeckManager.h"


class Mixer;

class MixerChannelStrip :	public Component,
							public ValueTree::Listener
{
public:
		
	MixerChannelStrip(int deckNo_, Mixer *mixer_);
	
	~MixerChannelStrip();
	
	//================================================================
	void resized();
	
	void paint(Graphics &g);
	
	//================================================================
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property);
	void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)	{}
	void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved) {}
	void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved) {}
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) {}
	
	//================================================================
	SegmentedMeter* getMeter()	{	return meter;	}
	
	//================================================================
	
private:
	
	enum EQBand {
		high,
		mid,
		low,
		noBands
	};
//	static const char *eqNames;
	
	DeckManager *settings;
	const int deckNo;
	
	Mixer *mixer;
	
	DrawableButton *onButon, *bypassButton;
	SegmentedMeter *meter;
	OwnedArray <Slider> eqSliders;
	OwnedArray <TextButton> eqKillButtons;
	OwnedArray <Label> eqLabels;
	TextButton *cueButton;

	Slider *gainSlider, *levelSlider, *fxASlider, *fxBSlider;
};

#endif //_DECKS_MIXERCHANNELSTRIP__H_