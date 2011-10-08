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

    filePlayer = new AudioFilePlayer();
	filePlayer->setAudioFormatManager(DecksAudioFormatManager::getInstance());

    thumbnailCache = new MultipleAudioThumbnailCache(10);
    thumbnail = new ColouredAudioThumbnail(512, *DecksAudioFormatManager::getInstance(), *thumbnailCache);
    
	//monitorFilePlayer = new AudioFilePlayer();

	settings.on = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(on));
	settings.bypass = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(bypass));
	settings.gain = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(gain));
	filePlayer->getAudioTransportSource()->setGain (settings.gain.get());
	settings.level = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(level));
	settings.highGain = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(highGain));
	filePlayer->getFilteringAudioSource()->setHighEQGain(settings.highGain.get());
	settings.midGain = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(midGain));
	filePlayer->getFilteringAudioSource()->setMidEQGain(settings.midGain.get());
	settings.lowGain = settingsManager->getPropertyOfChannel(deckNo, CHANNEL_SETTING(lowGain));
	filePlayer->getFilteringAudioSource()->setLowEQGain(settings.lowGain.get());
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
//	ScopedLock currentLock(lock);
	
	if (setting == CHANNEL_SETTING(on))
		settings.on.set(newValue);
	else if (setting == CHANNEL_SETTING(bypass))
		settings.bypass.set(newValue);
	else if (setting == CHANNEL_SETTING(gain))
	{
		settings.gain.set(newValue);
		filePlayer->getAudioTransportSource()->setGain(settings.gain.get());
	}
	else if (setting == CHANNEL_SETTING(level))
		settings.level.set(newValue);
	else if (setting == CHANNEL_SETTING(highGain))
	{
		settings.highGain.set(newValue);
		filePlayer->getFilteringAudioSource()->setHighEQGain(settings.highGain.get());
	}
	else if (setting == CHANNEL_SETTING(midGain))
	{
		settings.midGain.set(newValue);
		filePlayer->getFilteringAudioSource()->setMidEQGain(settings.midGain.get());
	}
	else if (setting == CHANNEL_SETTING(lowGain))
	{
		settings.lowGain.set(newValue);
		filePlayer->getFilteringAudioSource()->setLowEQGain(settings.lowGain.get());
	}
	else if (setting == CHANNEL_SETTING(highKill))
		settings.highKill.set(newValue);
	else if (setting == CHANNEL_SETTING(midKill))
		settings.midKill.set(newValue);
	else if (setting == CHANNEL_SETTING(lowKill))
		settings.lowKill.set(newValue);
	else if (setting == CHANNEL_SETTING(cue))
		settings.cue.set(newValue);
	else if (setting == CHANNEL_SETTING(fxASend))
		settings.fxASend.set(newValue);
	else if (setting == CHANNEL_SETTING(fxBSend))
		settings.fxBSend.set(newValue);
}

const var Deck::getSetting(const Identifier &setting)
{
	if (setting == CHANNEL_SETTING(on))
		return settings.on.get();
	else if (setting == CHANNEL_SETTING(bypass))
		return settings.bypass.get();
	else if (setting == CHANNEL_SETTING(gain))
		return settings.gain.get();
	else if (setting == CHANNEL_SETTING(level))
		return settings.level.get();
	else if (setting == CHANNEL_SETTING(highGain))
		return settings.highGain.get();
	else if (setting == CHANNEL_SETTING(midGain))
		return settings.midGain.get();
	else if (setting == CHANNEL_SETTING(lowGain))
		return settings.lowGain.get();
	else if (setting == CHANNEL_SETTING(highKill))
		return settings.highKill.get();
	else if (setting == CHANNEL_SETTING(midKill))
		return settings.midKill.get();
	else if (setting == CHANNEL_SETTING(lowKill))
		return settings.lowKill.get();
	else if (setting == CHANNEL_SETTING(cue))
		return settings.cue.get();
	else if (setting == CHANNEL_SETTING(fxASend))
		return settings.fxASend.get();
	else if (setting == CHANNEL_SETTING(fxBSend))
		return settings.fxBSend.get();
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
	
	masterSettings.gain.set(settingsManager->getPropertyOfMaster(MASTER_SETTING(gain)));
	masterSettings.cue.set(settingsManager->getPropertyOfMaster(MASTER_SETTING(cue)));
	masterSettings.faderCurve.set(settingsManager->getPropertyOfMaster(MASTER_SETTING(faderCurve)));
	masterSettings.xFaderCurve.set(settingsManager->getPropertyOfMaster(MASTER_SETTING(xFaderCurve)));
	masterSettings.monitorGain.set(settingsManager->getPropertyOfMaster(MASTER_SETTING(monitorGain)));

	xFaderSettings.level.set(settingsManager->getPropertyOfXFader(XFADER_SETTING(level)));
	xFaderSettings.assignX.set(settingsManager->getPropertyOfXFader(XFADER_SETTING(assignX)));
	xFaderSettings.assignY.set(settingsManager->getPropertyOfXFader(XFADER_SETTING(assignY)));

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
//	ScopedLock currentLock(lock);
	
	if (setting == MASTER_SETTING(gain))
		masterSettings.gain.set(newValue);
	else if (setting == MASTER_SETTING(cue))
		masterSettings.cue.set(newValue);
	else if (setting == MASTER_SETTING(faderCurve))
		masterSettings.faderCurve.set(newValue);
	else if (setting == MASTER_SETTING(xFaderCurve))
		masterSettings.xFaderCurve.set(newValue);
	else if (setting == MASTER_SETTING(monitorGain))
		masterSettings.monitorGain.set(newValue);
}

const var DeckManager::getMasterSetting(const Identifier &setting)
{
	if (setting == MASTER_SETTING(gain))
		return masterSettings.gain.get();
	else if (setting == MASTER_SETTING(cue))
		return masterSettings.cue.get();
	else if (setting == MASTER_SETTING(faderCurve))
		return masterSettings.faderCurve.get();
	else if (setting == MASTER_SETTING(xFaderCurve))
		return masterSettings.xFaderCurve.get();
	else if (setting == MASTER_SETTING(monitorGain))
		return masterSettings.monitorGain.get();
	else
		return var::null;
}

void DeckManager::setXFaderSetting(const Identifier &setting, const var &newValue)
{
	// need to lock the variables when writing incase the audio thread is reading
//	ScopedLock currentLock(lock);
	
	if (setting == XFADER_SETTING(level))
		xFaderSettings.level.set(newValue);
	else if (setting == XFADER_SETTING(assignX))
		xFaderSettings.assignX.set(newValue);
	else if (setting == XFADER_SETTING(assignY))
		xFaderSettings.assignY.set(newValue);
}

const var DeckManager::getXFaderSetting(const Identifier &setting)
{
	if (setting == XFADER_SETTING(level))
		return xFaderSettings.level.get();
	else if (setting == XFADER_SETTING(assignX))
		return xFaderSettings.assignX.get();
	else if (setting == XFADER_SETTING(assignY))
		return xFaderSettings.assignY.get();
	else
		return var::null;	
}