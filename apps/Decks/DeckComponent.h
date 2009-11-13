/*
 *  DeckComponent.h
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DECKCOMPONENT_H_
#define _DECKCOMPONENT_H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

#include "TrackInfo.h"

class DeckComponent : public Component,
					  public ButtonListener,
					  public SliderListener
{
public:
	DeckComponent(juce_wchar deckNumber_);
	~DeckComponent();
	
	//============================================================
	void resized();
	
	void paint(Graphics& g);
	//============================================================
	void buttonClicked(Button* button);
	
	void sliderValueChanged(Slider* changedSlider);
	void sliderDragEnded(Slider* changedSlider);
	
	//============================================================
	TextButton* getStopButton()			{	return stopButton;		}
	TextButton* getPlayPauseButton()	{	return playPauseButton;	}
	TextButton* getCueButton()			{	return cueButton;		}

	//============================================================
	FilteringAudioFilePlayer* getAudioFilePlayer() { return filePlayer;	}
	
private:
	juce_wchar deckNumber;
	
	// audio objects
	FilteringAudioFilePlayer *filePlayer;
	
	// deck controls
	TextButton* stopButton;
	TextButton* playPauseButton;
	TextButton* cueButton;
		
	Slider* speedSlider;
	Slider* speedSliderFine;
	Slider* gainSlider;
	
	Slider *highEQSlider, *midEQSlider, *lowEQSlider;
		
	void setupEQSlider(Slider* EQslider);
};

#endif //_DECKCOMPONENT_H_