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
#include "../../../main/DeckManager.h"
#include "../../DecksLookAndFeel.h"
#include "TrackInfo.h"

class DeckTransport :	public Component,
						public ButtonListener,
						public SliderListener
{
public:
	
	enum TransportButtons {
		eject,
		previous,
		shuffleBack,
		stop,
		playPause,
		shuffleForward,
		next,
		noTransportButtons
	};
	
	DeckTransport(int deckNo_)
	:	deckNo(deckNo_),
		settings(DeckManager::getInstance()),
		filePlayer(settings->getDeck(deckNo)->getMainFilePlayer())
	{
		getLookAndFeel().setColour(TextButton::textColourOffId, Colour::greyLevel(0.8));
		
		addAndMakeVisible(infoBox = new TrackInfo(deckNo, filePlayer));
		
		for (int i = 0; i < noTransportButtons; i++) {
//			transportButtons.add(new DrawableButton(String(i), DrawableButton::ImageOnButtonBackground));
			
			transportButtons.add(new TextButton(String(i)));

			addAndMakeVisible(transportButtons[i]);
			transportButtons[i]->addButtonListener(this);
//			transportButtons[i]->setBackgroundColours(Colours::darkgrey, Colours::lightgrey);
		}
		transportButtons[playPause]->setClickingTogglesState(true);
		
		transportButtons[eject]->setButtonText(T("ejct"));
		transportButtons[previous]->setButtonText(T("<-"));
		transportButtons[shuffleBack]->setButtonText(T("<<"));
		transportButtons[stop]->setButtonText(T("[]"));
		transportButtons[playPause]->setButtonText(T(">"));
		transportButtons[shuffleForward]->setButtonText(T(">>"));
		transportButtons[next]->setButtonText(T("->"));
		
		//================
//		transportButtons[stop]->setImages(createDrawableFromSVGFile("/Users/Dave/Documents/Developement/dRowAudio/apps/Decks/src/resources/buttons/controls_stop.svg"));
//		transportButtons[stop]->setImages(createDrawableFromSVGFile(getResourcesFolder().getChildFile("controls_stop.svg")));
//		transportButtons[playPause]->setImages(createPlayButton(), 0, 0, 0,
//											   createDrawableFromSVGFile(getResourcesFolder().getChildFile("controls_pause.svg")));
		//================
		
		addAndMakeVisible(waveDisplay = new PositionableWaveDisplay(filePlayer, 44100));
		
		addAndMakeVisible(speedSlider = new Slider("speedSlider"));
		speedSlider->setSliderStyle(Slider::LinearVertical);
		speedSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
		speedSlider->setRange(0.8, 1.2, 0.0001);
		speedSlider->setValue(1.0);
		speedSlider->addListener(this);
		
		addAndMakeVisible(jogSlider = new Slider("jogSlider"));
		jogSlider->setSliderStyle(Slider::LinearVertical);
		jogSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
		jogSlider->setRange( -0.05, 0.05, 0.0001);
		jogSlider->setValue(0.0);
		jogSlider->addListener(this);		
	}
	
	~DeckTransport()
	{
		transportButtons.clear();
		deleteAllChildren();
	}
	
	//==============================================================================
	void resized()
	{
		const int width = getWidth();
		const int height = getHeight();
		const int margin = 5;
		
		infoBox->setBounds(0, 0, width-35, 50);
		
		const int buttonWidth = /*jmin(*/((infoBox->getWidth()-2)/noTransportButtons)-1;//, 21);
		for (int i = 0; i < noTransportButtons; i++) {
			transportButtons[i]->setBounds(2+(i*(buttonWidth+1)), infoBox->getBottom()+2, buttonWidth, 20);
		}
		
		waveDisplay->setBounds(0, 100, width-35, 50);
		
		speedSlider->setBounds(width-30, 0, 10, waveDisplay->getBottom());
		jogSlider->setBounds(width-15, 0, 10, waveDisplay->getBottom());
	}
	
	void paint(Graphics &g)
	{
		g.setColour(DecksLookAndFeel::getInstance()->getDecksColour(DecksLookAndFeel::panelColour));
		g.fillAll();
	}
	
	//==============================================================================
	void buttonClicked(Button* button)
	{
		if (button == transportButtons[previous])
			settings->getDeck(deckNo)->startFromZero();
		else if (button == transportButtons[stop])
		{
			settings->getDeck(deckNo)->stop();
			settings->getDeck(deckNo)->setNextReadPosition(0);
			transportButtons[playPause]->setToggleState(false, false);
		}
		else if (button == transportButtons[playPause])
			settings->getDeck(deckNo)->pause();
	}
	
	void buttonStateChanged (Button* button)
	{
		if (button == transportButtons[shuffleForward])
		{
			if (transportButtons[shuffleForward]->isDown())
				settings->getDeck(deckNo)->setResamplingRatio ((speedSlider->getValue() + jogSlider->getValue())*2);
			else
				settings->getDeck(deckNo)->setResamplingRatio ((speedSlider->getValue() + jogSlider->getValue())*1);
		}
		
	}
	
	void sliderValueChanged(Slider* changedSlider)
	{
		if (changedSlider == speedSlider)
			settings->getDeck(deckNo)->setResamplingRatio (speedSlider->getValue() + jogSlider->getValue());
		else if (changedSlider == jogSlider)
			settings->getDeck(deckNo)->setResamplingRatio (speedSlider->getValue() + jogSlider->getValue());
	}
	void sliderDragEnded(Slider* changedSlider)
	{
		if (changedSlider == jogSlider)
			jogSlider->setValue(0.0);
	}
	
	//==============================================================================

private:
	const int deckNo;
	DeckManager *settings;
	FilteringAudioFilePlayer *filePlayer;
	
	TrackInfo *infoBox;
	OwnedArray<TextButton> transportButtons;
	PositionableWaveDisplay *waveDisplay;
	
	Slider *speedSlider, *jogSlider;
	
	//==============================================================================
};

#endif //_DECKTRANSPORT__H_