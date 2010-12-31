/*
 *  DeckTransport.h
 *  Decks
 *
 *  Created by David Rowland on 05/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKTRANSPORT__H_
#define _DECKTRANSPORT__H_

#include <juce/juce.h>
#include "../../../ui/DecksColours.h"
#include "../../../main/DeckManager.h"
#include "../../DecksLookAndFeel.h"
#include "TrackInfo.h"
#include "LoopAndCuePoints.h"

class DeckTransport :	public Component,
						public Button::Listener,
						public Slider::Listener,
						public ChangeListener
{
public:
	
	enum TransportButtons {
		eject,
		loop,
		cue,
		previous,
		shuffleBack,
		stop,
		playPause,
		shuffleForward,
		next,
		noTransportButtons
	};
	
	DeckTransport(int deckNo_);
	
	~DeckTransport();
	
	//==============================================================================
	void resized();
	
	void paint(Graphics &g);
	
	//==============================================================================
	void buttonClicked(Button* button);
	
	void buttonStateChanged (Button* button);
	
	void sliderValueChanged(Slider* changedSlider);
	void sliderDragEnded(Slider* changedSlider);
	
	void changeListenerCallback(ChangeBroadcaster *object);

	//==============================================================================
	void setLoopAndCueValueToReferTo(Value &valueToReferTo);
	
	//==============================================================================

private:
	const int deckNo;
	bool showLoopAndCuePoints;
	DeckManager *settings;
	FilteringAudioFilePlayer *filePlayer;
	
	TrackInfo *infoBox;
	OwnedArray<DrawableButton> transportButtons;
	PositionableWaveDisplay *waveDisplay;
	LoopAndCuePoints *loopAndCuePoints;
	
	Slider *speedSlider, *jogSlider;
	
	//==============================================================================
};

#endif //_DECKTRANSPORT__H_