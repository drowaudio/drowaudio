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
	monitorFilePlayer = new FilteringAudioFilePlayer();
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
	for (int i = 0; i < int(Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels")); i++) {
		decks.add( new Deck(i) );
	}
}

DeckManager::~DeckManager()
{
	decks.clear();
}