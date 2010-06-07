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

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>
#include "../../DecksLookAndFeel.h"
#include "../../../main/DeckManager.h"
#include "dRowAudio_SegmentedMeter.h"


class Mixer;

class MixerChannelStrip :	public Component,
							public ButtonListener,
							public SliderListener
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
		eqSliders.clear();
		eqKillButtons.clear();
		eqLabels.clear();
		deleteAllChildren();
	}
	
	//================================================================
	void resized();
	
	void paint(Graphics &g);
	
	//================================================================
	void buttonClicked(Button *button);
	
	void sliderValueChanged (Slider* slider);
	
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