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
#include "../../../main/Settings.h"
#include "../../../main/DeckManager.h"
#include "DeckTransport.h"

class Transport :	public Component,
					public ButtonListener,
					public ValueTree::Listener,
					public Value::Listener
{
public:
	Transport()
	:	settings(DeckManager::getInstance())
	{
		int noDecks = Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels");
		Settings::getInstance()->getValueTreePointer()->addListener(this);

		// = new TextButton(T("Loops"));
		showLoopsValue.setValue(false);
		showLoopsValue.addListener(this);
		
		for (int i = 0; i < noDecks; i++) {
			decksTransports.add(new DeckTransport(i));
			addAndMakeVisible(decksTransports[i]);
			decksTransports[i]->setLoopAndCueValueToReferTo(showLoopsValue);
		}

//		addAndMakeVisible(showLoopsButton);
//		showLoopsButton->setClickingTogglesState(true);
//		showLoopsButton->addButtonListener(this);
	}
	
	~Transport()
	{
		showLoopsValue.removeListener(this);
		Settings::getInstance()->getValueTreePointer()->removeListener(this);

		decksTransports.clear();
		deleteAllChildren();
		
		DBG("Transport deleted");
	}
	
	//================================================================
	void resized()
	{
		const int width = getWidth();
		const int height = showLoopsValue.getValue() ? 250 : 132;
		const int margin = 5;
		setSize(width, height);
		
		
		//============================================================
		const int noDecks = decksTransports.size();
		int noEnabledDecks = DeckManager::getNoEnabledDecks();
		if (noEnabledDecks)
		{
			const int deckWidth = ((width - ((noEnabledDecks-1)*margin)) / (double)noEnabledDecks);

			int pos = 0;
			for (int i = 0; i < noDecks; i++)
			{
				if (bool(Settings::getInstance()->getPropertyOfChannel(i, CHANNEL_SETTING(on))) == true) {
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

//		showLoopsButton->setBounds(getWidth()-50, 125, 40, 20);
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
	void buttonClicked(Button *button)
	{
		getParentComponent()->resized();
	}
		
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property)
	{
		for (int i = 0; i < int(Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels")); i++) {
			if (treeWhosePropertyHasChanged.getProperty(property) == Settings::getInstance()->getPropertyOfChannelAsValue(i, CHANNEL_SETTING(on))) {
				resized();
			}
		}
	}
	
	void valueTreeChildrenChanged (ValueTree &treeWhoseChildHasChanged)
	{
	}
	
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged)
	{
	}
	
	void valueChanged (Value &value)
	{
		if (value.refersToSameSourceAs(showLoopsValue)) {
			getParentComponent()->resized();
		}
	}
	//================================================================
	
private:
	DeckManager *settings;

	Value showLoopsValue;
	OwnedArray<DeckTransport> decksTransports;
};

#endif //_TRANSPORT__H_