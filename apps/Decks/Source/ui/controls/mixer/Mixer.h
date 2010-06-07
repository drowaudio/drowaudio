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

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>
#include "../../DecksLookAndFeel.h"
#include "../../../main/DeckManager.h"
#include "MixerChannelStrip.h"
#include "MasterChannelStrip.h"
#include "CrossFader.h"

//class Mixer : public Viewport {
//public:
//	Mixer()
//	{
//		mixer = new MixerContent;
//		setViewedComponent(mixer);
//	}
//	
//	~Mixer()
//	{
//		deleteAllChildren();
//	}
//		
//private:
//	MixerContent *mixer;
//};

class Mixer : public  Component
{
public:
	Mixer()
	{
		addAndMakeVisible( masterStrip = new MasterChannelStrip() );
		addAndMakeVisible( xFader = new CrossFader(this) );

		const int noDecks = DeckManager::getInstance()->getMaxNoDecks();
		for (int i = 0; i < noDecks; i++)
		{
			channelStrips.add(new MixerChannelStrip(i, this));
			addAndMakeVisible(channelStrips[i]);
		}

		setSize(815, 425);
	}
	
	~Mixer()
	{
		channelStrips.clear();
		deleteAllChildren();
	}
	
	//================================================================
	void resized()
	{
		const int stripWidth = 130;
		const int stripHeight = 320;
		const int m = 2;
		const int noDecks = DeckManager::getInstance()->getMaxNoDecks();

		int currentDeck = 0;
		channelStrips[currentDeck++]->setBounds(m, m, stripWidth, stripHeight);
		for (int i = currentDeck; i < noDecks; i++) {
			channelStrips[i]->setBounds(channelStrips[i-1]->getRight()+m, m, stripWidth, stripHeight);
			currentDeck = i;
		}
		
		int xFaderHeight = 90;
		xFader->setBounds(m, channelStrips[0]->getBottom()+m,
						  channelStrips[currentDeck]->getRight()-channelStrips[0]->getX(), xFaderHeight);
		masterStrip->setBounds(channelStrips[currentDeck]->getRight()+m, m, stripWidth/2, xFader->getBottom()-channelStrips[0]->getY());

		setSize(masterStrip->getRight()+m, xFader->getBottom()+m);
	}
	
	void paint(Graphics &g) {}
	
	//================================================================
	// Master acessor methods
	//================================================================
	double getMasterGain()	{	return masterStrip->getGainLevel();	}

	const MasterChannelStrip* getMasterChannelStrip()	{	return masterStrip;	}

	//================================================================
	// XFader acessor methods
	//================================================================
	float getXFaderLevelX()		{	return xFader->getXFaderLevelX();	}
	float getXFaderLevelY()		{	return xFader->getXFaderLevelY();	}

	int getXFaderAssignX()		{	return xFader->getAssignX();		}
	int getXFaderAssignY()		{	return xFader->getAssignY();		}
	//================================================================
	void updateMeterForChannel(int deckNo, float **data, int numSamples, int numChannels)
	{
		SegmentedMeter *meter = channelStrips[deckNo]->getMeter();
		if (meter != 0)
			meter->processBlock(data, numSamples, numChannels);
	}
	
	void updateMasterMeter(float **data, int numSamples, int numChannels)
	{
		SegmentedMeter *meterL = masterStrip->getMeterL();
		SegmentedMeter *meterR = masterStrip->getMeterR();

		if (meterL != 0)
			meterL->processBlock(data[0], numSamples);

		if (numChannels > 1)
			meterR->processBlock(data[1], numSamples);
	}
	//================================================================

private:
	
	OwnedArray <MixerChannelStrip> channelStrips;
	CrossFader *xFader;
	MasterChannelStrip *masterStrip;
};

#endif //_DECKS_MIXER__H_