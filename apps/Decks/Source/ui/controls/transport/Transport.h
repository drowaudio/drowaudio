/*
 *  Transport.h
 *  Decks
 *
 *  Created by David Rowland on 05/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _TRANSPORT__H_
#define _TRANSPORT__H_

#include <juce/juce.h>
#include "../../../main/DeckManager.h"
#include "DeckTransport.h"

class Transport :	public Component,
					public Deck::Listener,
					public ButtonListener
{
public:
	Transport()
	:	settings(DeckManager::getInstance())
	{
		int noDecks = settings->getMaxNoDecks();
		for (int i = 0; i < noDecks; i++) {
			settings->getDeck(i)->addListener(this);
			
			decksTransports.add(new DeckTransport(i));
			addAndMakeVisible(decksTransports[i]);
		}

		addAndMakeVisible( showLoopsButton = new TextButton(T("Loops")) );
		showLoopsButton->setClickingTogglesState(true);
		showLoopsButton->addButtonListener(this);
	}
	
	~Transport()
	{
		decksTransports.clear();
		deleteAllChildren();
	}
	
	//================================================================
	void resized()
	{
		const int width = getWidth();
		const int height = showLoopsButton->getToggleState() ? 250 : 150;
		const int margin = 5;
		setSize(width, height);
		
		
		//============================================================
		Value tempTrue(true);
		
		const int noDecks = decksTransports.size();
		int noEnabledDecks = DeckManager::getNoEnabledDecks();
		
		if (noEnabledDecks)
		{
			const int deckWidth = ((width - ((noEnabledDecks-1)*margin)) / noEnabledDecks);

			int pos = 0;
			for (int i = 0; i < noDecks; i++)
			{
				if (settings->getDeck(i)->getChannelSetting(Deck::on) == tempTrue) {
					decksTransports[i]->setVisible(true);
					decksTransports[i]->setBounds((pos*(deckWidth+margin)), 0, deckWidth, height);
					pos++;
				}
				else
					decksTransports[i]->setVisible(false);
			}
		}
		else {
			for (int i = 0; i < noDecks; i++)
				decksTransports[i]->setVisible(false);
		}

		showLoopsButton->setBounds(getWidth()-50, 125, 40, 20);
	}
	
	void paint(Graphics &g)
	{
		if (!settings->getNoEnabledDecks()) {
			g.setColour(DecksLookAndFeel::getInstance()->getDecksColour(DecksLookAndFeel::panelLineColour));
			String message ("Enable Decks From The Mixer To Begin...");
			g.setFont(getHeight()*0.15);
			g.drawFittedText(message, getWidth()*0.25, getHeight()*0.25, getWidth()*0.5, getHeight()*0.5, Justification::centred, 1, 1);
			g.drawRect(getWidth()*0.25, getHeight()*0.25, getWidth()*0.5, getHeight()*0.5, 1);
		}
	}

	//================================================================
	void deckChanged (Deck *deck)
	{
		resized();
	}
	
	void buttonClicked(Button *button)
	{
		getParentComponent()->resized();
	}
	//================================================================
	
private:
	DeckManager *settings;

	TextButton *showLoopsButton;
	OwnedArray<DeckTransport> decksTransports;
};

#endif //_TRANSPORT__H_