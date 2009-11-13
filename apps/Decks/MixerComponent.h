/*
 *  MixerComponent.h
 *  resampling_test
 *
 *  Created by David Rowland on 30/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _MIXERCOMPONENT_H_
#define _MIXERCOMPONENT_H_

#include "DeckComponent.h"

class MixerComponent : public Component,
					   public ApplicationCommandTarget
{
public:
	
	MixerComponent();
	~MixerComponent();
	
	void resized();
	
	DeckComponent* getDeck1()	{	return deck1;	};
	DeckComponent* getDeck2()	{	return deck2;	};
	
	//============================================================
	enum CommandIDs
    {
        playPauseDeck1			= 0x2D01,
        playPauseDeck2          = 0x2D02,
		cueDeck1				= 0x2D03,
		cueDeck2				= 0x2D04,
		stopDeck1				= 0x2D05,
		stopDeck2				= 0x2D06,
	};
	
	ApplicationCommandTarget* getNextCommandTarget();
	void getAllCommands (Array <CommandID>& commands);
	void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result);
	bool perform (const InvocationInfo& info);
	//============================================================
	
private:
	
	DeckComponent *deck1, *deck2;
};

#endif //_MIXERCOMPONENT_H_