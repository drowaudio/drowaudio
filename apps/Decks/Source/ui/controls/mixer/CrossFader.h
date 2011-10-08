/*
 *  CrossFader.h
 *  Decks
 *
 *  Created by David Rowland on 04/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKS_CROSSFADER__H_
#define _DECKS_CROSSFADER__H_

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../DecksLookAndFeel.h"
#include "../../../main/Settings.h"
#include "../../../main/DeckManager.h"
#include "../../../main/DefaultSettings.h"
//#include "dRowAudio_IntegerSlider.h"

class CrossFader :	public Component,
					public SliderListener,
					public ValueTree::Listener
{
public:
	CrossFader();
	
	~CrossFader()
	{
		xAssignSlider->removeListener(this);
		yAssignSlider->removeListener(this);
		Settings::getInstance()->getValueTreePointer()->removeListener(this);

		deleteAllChildren();
		DBG("CrossFader deleted");
	}
	
	//================================================================
	void resized();
	
	void paint(Graphics &g);
	
	//================================================================
	void sliderValueChanged (Slider* slider){}
	
	void sliderDragStarted (Slider *slider)
	{
		Desktop::getInstance().getMainMouseSource().enableUnboundedMouseMovement(true, false);
	}
	
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property);
	void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)	{}
	void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved) {}
	void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved) {}
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged);

	//================================================================

private:
	DeckManager *manager;

	Slider *xFaderSlider;
	Slider *xAssignSlider, *yAssignSlider;
	Label *xAssginLabel, *yAssignLabel;
};

#endif //_DECKS_CROSSFADER__H_