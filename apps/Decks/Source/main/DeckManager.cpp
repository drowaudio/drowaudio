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
	Settings* settingsManager = Settings::getInstance();

	filePlayer = new FilteringAudioFilePlayer();
	monitorFilePlayer = new FilteringAudioFilePlayer();

	settings.on = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(on));
	settings.bypass = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(bypass));
	settings.gain = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(gain));
	filePlayer->setGain(settings.gain);
	settings.level = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(level));
	settings.highGain = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(highGain));
	settings.midGain = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(midGain));
	settings.lowGain = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(lowGain));
	settings.highKill = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(highKill));
	settings.midKill = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(midKill));
	settings.lowKill = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(lowKill));
	settings.cue = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(cue));
	settings.fxASend = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(fxASend));
	settings.fxBSend = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(fxBSend));
	
	settingsManager->getValueTreePointer()->addListener(this);
}

Deck::~Deck()
{
	Settings::getInstance()->getValueTreePointer()->removeListener(this);
	
	String message("Deck ");
	message << deckNo << " deleted";
	DBG(message);
}

void Deck::valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property)
{
	if (treeWhosePropertyHasChanged.hasProperty(CHANNEL_SETTING(number)))
	{
		if (int(treeWhosePropertyHasChanged.getProperty(CHANNEL_SETTING(number))) == deckNo)
		{
			setSetting(property, treeWhosePropertyHasChanged.getProperty(property));
		}
	}
}

void Deck::setSetting(const Identifier &setting, const var &newValue)
{
	// need to lock the variables when writing incase the audio thread is reading
	ScopedLock currentLock(lock);
	
	if (setting == CHANNEL_SETTING(on))
		settings.on = newValue;
	else if (setting == CHANNEL_SETTING(bypass))
		settings.bypass = newValue;
	else if (setting == CHANNEL_SETTING(gain))
	{
		settings.gain = newValue;
		filePlayer->setGain(settings.gain);
	}
	else if (setting == CHANNEL_SETTING(level))
		settings.level = newValue;
	else if (setting == CHANNEL_SETTING(highGain))
	{
		settings.highGain = newValue;
		filePlayer->setHighEQGain(settings.highGain);
	}
	else if (setting == CHANNEL_SETTING(midGain))
	{
		settings.midGain = newValue;
		filePlayer->setMidEQGain(settings.midGain);
	}
	else if (setting == CHANNEL_SETTING(lowGain))
	{
		settings.lowGain = newValue;
		filePlayer->setLowEQGain(settings.lowGain);
	}
	else if (setting == CHANNEL_SETTING(highKill))
		settings.highKill = newValue;
	else if (setting == CHANNEL_SETTING(midKill))
		settings.midKill = newValue;
	else if (setting == CHANNEL_SETTING(lowKill))
		settings.lowKill = newValue;
	else if (setting == CHANNEL_SETTING(cue))
		settings.cue = newValue;
	else if (setting == CHANNEL_SETTING(fxASend))
		settings.fxASend = newValue;
	else if (setting == CHANNEL_SETTING(fxBSend))
		settings.fxBSend = newValue;
}

const var& Deck::getSetting(const Identifier &setting)
{
	if (setting == CHANNEL_SETTING(on))
		return settings.on;
	else if (setting == CHANNEL_SETTING(bypass))
		return settings.bypass;
	else if (setting == CHANNEL_SETTING(gain))
		return settings.gain;
	else if (setting == CHANNEL_SETTING(level))
		return settings.level;
	else if (setting == CHANNEL_SETTING(highGain))
		return settings.highGain;
	else if (setting == CHANNEL_SETTING(midGain))
		return settings.midGain;
	else if (setting == CHANNEL_SETTING(lowGain))
		return settings.lowGain;
	else if (setting == CHANNEL_SETTING(highKill))
		return settings.highKill;
	else if (setting == CHANNEL_SETTING(midKill))
		return settings.midKill;
	else if (setting == CHANNEL_SETTING(lowKill))
		return settings.lowKill;
	else if (setting == CHANNEL_SETTING(cue))
		return settings.cue;
	else if (setting == CHANNEL_SETTING(fxASend))
		return settings.fxASend;
	else if (setting == CHANNEL_SETTING(fxBSend))
		return settings.fxBSend;
	else
		return var::null;
}


//void Deck::addListener (Listener* const listener)
//{
//    jassert (listener != 0);
//    if (listener != 0)
//        listeners.add (listener);
//}
//
//void Deck::removeListener (Listener* const listener)
//{
//    listeners.removeValue (listener);
//}
//
//void Deck::callListeners()
//{
//    for (int i = listeners.size(); --i >= 0;)
//    {
//        Listener* const l = listeners[i];
//		
//        if (l != 0)
//            l->deckChanged (this);
//    }
//}

//==============================================================================
juce_ImplementSingleton (DeckManager)


DeckManager::DeckManager()
{
	Settings* settingsManager = Settings::getInstance();
	
	masterSettings.gain = settingsManager->getPropertyOfMaster(MASTER_SETTING(gain));
	masterSettings.cue = settingsManager->getPropertyOfMaster(MASTER_SETTING(cue));
	masterSettings.faderCurve = settingsManager->getPropertyOfMaster(MASTER_SETTING(faderCurve));
	masterSettings.xFaderCurve = settingsManager->getPropertyOfMaster(MASTER_SETTING(xFaderCurve));

	xFaderSettings.level = settingsManager->getPropertyOfXFader(XFADER_SETTING(level));
	xFaderSettings.assignX = settingsManager->getPropertyOfXFader(XFADER_SETTING(assignX));
	xFaderSettings.assignY = settingsManager->getPropertyOfXFader(XFADER_SETTING(assignY));

	for (int i = 0; i < int(Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels")); i++) {
		decks.add( new Deck(i) );
	}
	
	settingsManager->getValueTreePointer()->addListener(this);
}

DeckManager::~DeckManager()
{
	Settings::getInstance()->getValueTreePointer()->removeListener(this);

	decks.clear();
	
	DBG("DeckManger deleted");
}

void DeckManager::valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property)
{
	if (treeWhosePropertyHasChanged.hasType(MixerSettings::MasterSettings::SectionName))
	{
		setMasterSetting(property, treeWhosePropertyHasChanged.getProperty(property));
	}
	if (treeWhosePropertyHasChanged.hasType(MixerSettings::xFaderSettings::SectionName))
	{
		setXFaderSetting(property, treeWhosePropertyHasChanged.getProperty(property));
	}	
}

void DeckManager::setMasterSetting(const Identifier &setting, const var &newValue)
{
	// need to lock the variables when writing incase the audio thread is reading
	ScopedLock currentLock(lock);
	
	if (setting == MASTER_SETTING(gain))
		masterSettings.gain = newValue;
	else if (setting == MASTER_SETTING(cue))
		masterSettings.cue = newValue;
	else if (setting == MASTER_SETTING(faderCurve))
		masterSettings.faderCurve = newValue;
	else if (setting == MASTER_SETTING(xFaderCurve))
		masterSettings.xFaderCurve = newValue;
}

const var& DeckManager::getMasterSetting(const Identifier &setting)
{
	if (setting == MASTER_SETTING(gain))
		return masterSettings.gain;
	else if (setting == MASTER_SETTING(cue))
		return masterSettings.cue;
	else if (setting == MASTER_SETTING(faderCurve))
		return masterSettings.faderCurve;
	else if (setting == MASTER_SETTING(xFaderCurve))
		return masterSettings.xFaderCurve;
	else
		return var::null;
}

void DeckManager::setXFaderSetting(const Identifier &setting, const var &newValue)
{
	// need to lock the variables when writing incase the audio thread is reading
	ScopedLock currentLock(lock);
	
	if (setting == XFADER_SETTING(level))
		xFaderSettings.level = newValue;
	else if (setting == XFADER_SETTING(assignX))
		xFaderSettings.assignX = newValue;
	else if (setting == XFADER_SETTING(assignY))
		xFaderSettings.assignY = newValue;
}

const var& DeckManager::getXFaderSetting(const Identifier &setting)
{
	if (setting == XFADER_SETTING(level))
		return xFaderSettings.level;
	else if (setting == XFADER_SETTING(assignX))
		return xFaderSettings.assignX;
	else if (setting == XFADER_SETTING(assignY))
		return xFaderSettings.assignY;
	else
		return var::null;	
}