/*
 *  DeckManager.h
 *  Decks
 *
 *  Created by David Rowland on 06/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DeckManager__H_
#define _DeckManager__H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>
#include "Settings.h"

class Deck	:	public Value::Listener
{
public:
	
	enum DeckSetting {
		on,
		bypass,
		gain,
		level,
		highGain,
		midGain,
		lowGain,		
		highKill,
		midKill,
		lowKill,
		cue,
		fxASend,
		fxBSend,
		noSettings
	};
	
	struct TransportSettings {
		bool playing;
		float speed;
	};
	
	Deck(int deckNo_);
	
	~Deck(){}
	
	int getDeckNo()								{	return deckNo;				}
	FilteringAudioFilePlayer* getFilePlayer()	{	return filePlayer;			}
//	ChannelSettings& getChannelSettings()		{	return *channelSettings;	}
	TransportSettings& getTransportSettings()	{	return *transportSettings;	}
	
	Value& getChannelSetting(DeckSetting setting)	{	return *channelSettingsValues[setting];	}
	const var getDeckSettingVar(DeckSetting setting)	{	return channelSettingsValues[setting]->getValue();	}
	
	void setDeckSetting(DeckSetting setting, var value)
	{
		channelSettingsValues[setting]->setValue(value);
		callListeners();
	}
	
	//==============================================================================
    /** Receives callbacks when a Deck object changes.
	 @see Deck::addListener
	 */
    class  Listener
    {
    public:
        Listener()          {}
        virtual ~Listener() {}
		
        /** Called when a Deck object is changed.
		 */
        virtual void deckChanged (Deck *deck) = 0;
    };
	
    /** Adds a listener to receive callbacks when the deck changes.
	 */
    void addListener (Listener* const listener);
	
    /** Removes a listener that was previously added with addListener(). */
    void removeListener (Listener* const listener);
	
    //==============================================================================
	void valueChanged(Value& value);
	
	//==============================================================================

private:
	const int deckNo;
	ScopedPointer<FilteringAudioFilePlayer> filePlayer;
	ScopedPointer<TransportSettings> transportSettings;
	
	OwnedArray <Value> channelSettingsValues;
	
	void callListeners();
	SortedSet <Listener*> listeners;
};


//==============================================================================
//
//==============================================================================
class DeckManager	:	public Value::Listener
{
public:
	
	enum MasterSettings {
		gain,
		cue,
		faderCurve,
		xFaderCurve,
		noMasterSettings
	};
	
	enum xFaderSetting {
		assignX,
		assignY,
		level,
		noXFaderSettings
	};
	
	DeckManager();
	
	~DeckManager();

	juce_DeclareSingleton (DeckManager, false)

	void valueChanged (Value& value);
	
	int getMaxNoDecks()				{	return decks.size();	}
	
	Deck* getDeck(int deckNo)		{	return decks[deckNo];	}
	
	/** Returns the number of currently enabled decks. */
	static const int getNoEnabledDecks()
	{
		DeckManager *settings = DeckManager::getInstance();
		const int noDecks = Settings->getInstance()->getPropertyOfChild("noChannels", "noChannels");
//		Value tempTrue(true);
		
		int noEnabledDecks = 0;
		for (int i = 0; i < noDecks; i++)
			if (settings->getDeck(i)->getDeckSettingVar(Deck::on) == var(true))
				noEnabledDecks++;
		
		return noEnabledDecks;
	}
		
	//==============================================================================
	Value& getMasterValueObject(MasterSettings setting)		{	return *masterSettingsValues[setting];	}
	const var getMasterSettingVar(MasterSettings setting)	{	return masterSettingsValues[setting]->getValue();	}
	
	Value& getXfaderValueObject(xFaderSetting setting)		{	return *xFaderSettingsValues[setting];	}
	const var getXFaderSettingVar(xFaderSetting setting)	{	return xFaderSettingsValues[setting]->getValue();	}
	//==============================================================================
	//==============================================================================

private:
	
	OwnedArray <Deck> decks;
	OwnedArray <Value> masterSettingsValues;
	OwnedArray <Value> xFaderSettingsValues;
};

#endif //_DeckManager__H_