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

#include <juce/juce.h>
#include "../../DecksLookAndFeel.h"
#include "../../../main/DeckManager.h"
#include "../../../main/DefaultSettings.h"
//#include "dRowAudio_IntegerSlider.h"

class Mixer;

class CrossFader :	public Component,
					public ButtonListener,
					public SliderListener
{
public:
	CrossFader(Mixer *mixer_);
	
	~CrossFader()
	{
		deleteAllChildren();
	}
	
	//================================================================
	void resized();
	
	void paint(Graphics &g);
	
	//================================================================
	void buttonClicked(Button *button) {}
	
	void sliderValueChanged (Slider* slider);
	
	void sliderDragStarted (Slider *slider)
	{
		Desktop::getInstance().getMainMouseSource().enableUnboundedMouseMovement(true, false);
	}
	
	//================================================================
	float getXFaderLevelX()		{	return	1.0f-xFaderSlider->getValue();	}
	float getXFaderLevelY()		{	return	xFaderSlider->getValue();		}
	
	int getAssignX()			{	return xAssignSlider->getValue();		}
	int getAssignY()			{	return yAssignSlider->getValue();		}

	//================================================================

private:
	DeckManager *manager;
	Mixer *mixer;

	Slider *xFaderSlider;
	Slider *xAssignSlider, *yAssignSlider;
	Label *xAssginLabel, *yAssignLabel;
};

#endif //_DECKS_CROSSFADER__H_