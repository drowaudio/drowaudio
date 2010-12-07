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
//							public SliderListener,
							public ValueTree::Listener
{
public:
	
	/** This holds all the possible settings a MixerChanelStrip can have. */
	struct ChannelSettings {
		bool on;
		bool bypass;
		float gain;
		float level;
		float highGain;
		float midGain;
		float lowGain;
		bool highKill;
		bool midKill;
		bool lowKill;
		bool cue;
		float fxASend;
		float fxBSend;
	};
	
	MixerChannelStrip(int deckNo_, Mixer *mixer_);
	
	~MixerChannelStrip()
	{
		Settings::getInstance()->getValueTreePointer()->removeListener(this);

		eqSliders.clear();
		eqKillButtons.clear();
		eqLabels.clear();
		deleteAllChildren();
		
		String message("MixerChannelStrip ");
		message << deckNo << " deleted";
		DBG(message);		
	}
	
	//================================================================
	void resized();
	
	void paint(Graphics &g);
	
	//================================================================
//	void sliderValueChanged (Slider* slider);
	
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property);
	void valueTreeChildrenChanged (ValueTree &treeWhoseChildHasChanged) {}
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