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

class Deck
{
public:
	
	Deck(int deckNo_);
	
	~Deck(){}
	
	int getDeckNo()										{	return deckNo;				}
//	FilteringAudioFilePlayer* getFilePlayer()			{	return filePlayer;			}
	FilteringAudioFilePlayer* getMainFilePlayer()		{	return filePlayer;			}
	FilteringAudioFilePlayer* getMonitorFilePlayer()	{	return monitorFilePlayer;	}
		
	void startFromZero()
	{
		filePlayer->startFromZero();
		monitorFilePlayer->startFromZero();
	}
	
	void stop()
	{
		filePlayer->stop();
		monitorFilePlayer->stop();
	}
	
	void pause()
	{
		filePlayer->pause();
		monitorFilePlayer->pause();
	}
	
	void setNextReadPosition(int position)
	{
		filePlayer->setNextReadPosition(position);
		monitorFilePlayer->setNextReadPosition(position);
	}
	
	void setResamplingRatio(const double samplesInPerOutputSample)
	{
		filePlayer->setResamplingRatio(samplesInPerOutputSample);
		monitorFilePlayer->setResamplingRatio(samplesInPerOutputSample);
	}
		
	//==============================================================================
    /** Receives callbacks when a Deck object changes.
	 @see Deck::addListener
	 */
//    class  Listener
//    {
//    public:
//        Listener()          {}
//        virtual ~Listener() {}
//		
//        /** Called when a Deck object is changed.
//		 */
//        virtual void deckChanged (Deck *deck) = 0;
//    };
//	
//    /** Adds a listener to receive callbacks when the deck changes.
//	 */
//    void addListener (Listener* const listener);
//	
//    /** Removes a listener that was previously added with addListener(). */
//    void removeListener (Listener* const listener);
	
	//==============================================================================

private:
	const int deckNo;
	ScopedPointer<FilteringAudioFilePlayer> filePlayer;
	ScopedPointer<FilteringAudioFilePlayer> monitorFilePlayer;
		
//	void callListeners();
//	SortedSet <Listener*> listeners;
};


//==============================================================================
//
//==============================================================================
class DeckManager	:	public DeletedAtShutdown
{
public:
		
	DeckManager();
	
	~DeckManager();

	juce_DeclareSingleton (DeckManager, false)
	
	int getMaxNoDecks()				{	return decks.size();	}
	
	Deck* getDeck(int deckNo)		{	return decks[deckNo];	}
	
	/** Returns the number of currently enabled decks. */
	static const int getNoEnabledDecks()
	{
		const int noDecks = Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels");
		
		int noEnabledDecks = 0;
		for (int i = 0; i < noDecks; i++)
			if (bool(Settings::getInstance()->getPropertyOfChannel(i, CHANNEL_SETTING(on))) == true)
				noEnabledDecks++;
		
		return noEnabledDecks;
	}
		
	//==============================================================================

private:
	
	OwnedArray <Deck> decks;
};

#endif //_DeckManager__H_