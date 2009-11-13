/*
 *  MixerComponent.cpp
 *  resampling_test
 *
 *  Created by David Rowland on 30/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "MixerComponent.h"

MixerComponent::MixerComponent()
{
	addAndMakeVisible( deck1 = new DeckComponent('1') );
	addAndMakeVisible( deck2 = new DeckComponent('2') );
}

MixerComponent::~MixerComponent()
{
	deleteAllChildren();
}

void MixerComponent::resized()
{
	deck1->setBounds(0, 0,
					 getWidth()*0.5, getHeight());
	
	deck2->setBounds(getWidth()*0.5, 0,
					 getWidth()*0.5, getHeight());	
}

//==============================================================================
ApplicationCommandTarget* MixerComponent::getNextCommandTarget()
{
	return findFirstTargetParentComponent();
}

void MixerComponent::getAllCommands (Array <CommandID>& commands)
{
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = {
		playPauseDeck1,
		playPauseDeck2,
//		cueDeck1,
//		cueDeck2,
//		stopDeck1,
//		stopDeck2,
	};
	
	commands.addArray (ids, numElementsInArray (ids));
}

void MixerComponent::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
	const String generalCategory (T("General"));
	const String deckControls (T("Deck Controls"));
	
	switch (commandID)
	{
		case playPauseDeck1:
			result.setInfo (T("Play Deck 1"), T("Plays the first deck from the beginning"), deckControls, 0);
			result.addDefaultKeypress (T('1'), ModifierKeys::altModifier);
			break;
			
		case playPauseDeck2:
			result.setInfo (T("Play Deck 2"), T("Plays the second deck from the beginning"), deckControls, 0);
			result.addDefaultKeypress (T('2'), ModifierKeys::altModifier);
			break;
			
		default:
			break;
			
	};
}

bool MixerComponent::perform (const InvocationInfo& info)
{
	switch (info.commandID)
	{
		case playPauseDeck1:
			DBG("Play deck 1");
			deck1->getPlayPauseButton()->triggerClick();
			break;
		case playPauseDeck2:
			DBG("Play deck 2");
			deck2->getPlayPauseButton()->triggerClick();
			break;
			
		default:
			return false;
	};
	
	return true;
}	
