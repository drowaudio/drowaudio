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
#include "../../../ui/DecksColours.h"
#include "../../../main/Settings.h"
#include "../../../main/DeckManager.h"
#include "DeckTransport.h"

class Transport :	public Component,
					public ButtonListener,
					public ValueTree::Listener,
					public Value::Listener
{
public:
	Transport();
	
	~Transport();
	
	//================================================================
	void resized();
	
	void paint(Graphics &g);

	//================================================================	
	void buttonClicked(Button *button);
		
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property);
	
	void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)	{}
	void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved) {}
	void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved) {}
	
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged);
	
	void valueChanged (Value &value);
	
	//================================================================
	
private:
	DeckManager *settings;

	Value showLoopsValue;
	OwnedArray<DeckTransport> decksTransports;
};

#endif //_TRANSPORT__H_