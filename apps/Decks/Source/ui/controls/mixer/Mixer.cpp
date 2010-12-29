/*
 *  Mixer.cpp
 *  Decks
 *
 *  Created by David Rowland on 03/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "Mixer.h"

Mixer::Mixer()
{
	meterManager = new GraphicalComponentManager();
	
	addAndMakeVisible( masterStrip = new MasterChannelStrip() );
	addAndMakeVisible( xFader = new CrossFader() );
	
	meterManager->addGraphicalComponent(masterStrip->getMeterL());
	meterManager->addGraphicalComponent(masterStrip->getMeterR());
	
	const int noDecks = Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels");
	for (int i = 0; i < noDecks; i++)
	{
		channelStrips.add(new MixerChannelStrip(i, this));
		meterManager->addGraphicalComponent(channelStrips[i]->getMeter());
		addAndMakeVisible(channelStrips[i]);
	}
	
	centreWithSize(815, 425);
}

Mixer::~Mixer()
{
	const int noDecks = Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels");
	meterManager->removeGraphicalComponent(masterStrip->getMeterL());
	meterManager->removeGraphicalComponent(masterStrip->getMeterR());
	for (int i = 0; i < noDecks; i++)
		meterManager->removeGraphicalComponent(channelStrips[i]->getMeter());
	
	channelStrips.clear();
	deleteAllChildren();
	DBG("Mixer deleted");
}

//================================================================
void Mixer::resized()
{
	const int stripWidth = 130;
	const int stripHeight = 320;
	const int m = 2;
	const int noDecks = Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels");
	
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

void Mixer::moved()
{
	const int noDecks = Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels");
	for (int i = 0; i < noDecks; i++) {
		channelStrips[i]->getMeter()->flagForRepaint();
	}
	masterStrip->getMeterL()->flagForRepaint();
	masterStrip->getMeterR()->flagForRepaint();
}

void Mixer::paint(Graphics &g) {}

void Mixer::updateMeterForChannel(int deckNo, float **data, int numSamples, int numChannels)
{
	SegmentedMeter *meter = channelStrips[deckNo]->getMeter();
	if (meter != 0)
		meter->copyValues(data, numSamples, numChannels);
}

void Mixer::updateMasterMeter(float **data, int numSamples, int numChannels)
{
	SegmentedMeter *meterL = masterStrip->getMeterL();
	SegmentedMeter *meterR = masterStrip->getMeterR();
	
	if (meterL != 0)
		meterL->copyValues(data[0], numSamples);
	
	if (numChannels > 1)
		meterR->copyValues(data[1], numSamples);
}
