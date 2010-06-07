/*
 *  DeckManager.cpp
 *  Decks
 *
 *  Created by David Rowland on 06/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "DeckManager.h"
#include "DefaultSettings.h"

Deck::Deck(int deckNo_)
:	deckNo(deckNo_)
{
	filePlayer = new FilteringAudioFilePlayer();
//	channelSettings = new ChannelSettings();
	transportSettings = new TransportSettings();
	
	for (int i = 0; i < noSettings; i++) {
		channelSettingsValues.add(new Value());
		channelSettingsValues[i]->addListener(this);
	}
	
	channelSettingsValues[on]->setValue(Defaults::Mixer::Channel::on);
	channelSettingsValues[bypass]->setValue(Defaults::Mixer::Channel::bypass);
	channelSettingsValues[gain]->setValue(Defaults::Mixer::Channel::gain);
	channelSettingsValues[level]->setValue(Defaults::Mixer::Channel::fader);
	channelSettingsValues[highGain]->setValue(Defaults::Mixer::Channel::HighEQ::gain);
	channelSettingsValues[midGain]->setValue(Defaults::Mixer::Channel::MidEQ::gain);
	channelSettingsValues[lowGain]->setValue(Defaults::Mixer::Channel::LowEQ::gain);
	channelSettingsValues[highKill]->setValue(Defaults::Mixer::Channel::HighEQ::killOn);
	channelSettingsValues[midKill]->setValue(Defaults::Mixer::Channel::MidEQ::killOn);
	channelSettingsValues[lowKill]->setValue(Defaults::Mixer::Channel::LowEQ::killOn);
	channelSettingsValues[cue]->setValue(Defaults::Mixer::Channel::cue);
	channelSettingsValues[fxASend]->setValue(Defaults::Mixer::Channel::fxASend);
	channelSettingsValues[fxBSend]->setValue(Defaults::Mixer::Channel::fxBSend);
	
	filePlayer->setGain(channelSettingsValues[level]->getValue());
}

void Deck::addListener (Listener* const listener)
{
    jassert (listener != 0);
    if (listener != 0)
        listeners.add (listener);
}

void Deck::removeListener (Listener* const listener)
{
    listeners.removeValue (listener);
}

void Deck::callListeners()
{
    for (int i = listeners.size(); --i >= 0;)
    {
        Listener* const l = listeners[i];
		
        if (l != 0)
            l->deckChanged (this);
    }
}

void Deck::valueChanged(Value& value)
{
	if (value.refersToSameSourceAs(*channelSettingsValues[gain])) {
		filePlayer->setGain(value.getValue());
	}
	if (value.refersToSameSourceAs(*channelSettingsValues[on])) {
		callListeners();
	}
}

//==============================================================================
juce_ImplementSingleton (DeckManager)


DeckManager::DeckManager()
{
	for (int i = 0; i < int(Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels")); i++) {
		decks.add( new Deck(i) );
	}
	
	for (int i = 0; i < noMasterSettings; i++) {
		masterSettingsValues.add(new Value());
		masterSettingsValues[i]->addListener(this);
	}
	
	masterSettingsValues[gain]->setValue(Defaults::Mixer::Master::gain);
	masterSettingsValues[cue]->setValue(Defaults::Mixer::Master::cue);
	masterSettingsValues[faderCurve]->setValue(Defaults::Mixer::Master::faderCurve);
	masterSettingsValues[xFaderCurve]->setValue(Defaults::Mixer::Master::xFaderCurve);
	
	
	for (int i = 0; i < noXFaderSettings; i++) {
		xFaderSettingsValues.add(new Value());
		xFaderSettingsValues[i]->addListener(this);
	}
	
	xFaderSettingsValues[assignX]->setValue(Defaults::Mixer::XFader::assignX);
	xFaderSettingsValues[assignY]->setValue(Defaults::Mixer::XFader::assignY);
	xFaderSettingsValues[level]->setValue(Defaults::Mixer::XFader::fader);
}

DeckManager::~DeckManager()
{
	decks.clear();
	
	clearSingletonInstance();
}

void DeckManager::valueChanged (Value &value)
{
	DBG("Master or xFader setting changed");
}