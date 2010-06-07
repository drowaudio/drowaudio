/*
 *  DraggableDisplay.h
 *  Decks
 *
 *  Created by David Rowland on 05/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DRAGGABLEDISPLAY__H_
#define _DRAGGABLEDISPLAY__H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>
#include "../../../main/DeckManager.h"
#include "../../DecksLookAndFeel.h"

class DraggableDisplay :	public Component,
							public Deck::Listener,
							public SliderListener
{
public:
	DraggableDisplay()
	:	noDecks(DeckManager::getInstance()->getMaxNoDecks()),
		settings(DeckManager::getInstance())
	{
		for (int i = 0; i < noDecks; i++)
		{
			settings->getDeck(i)->addListener(this);

			draggableWaveDisplays.add(new DraggableWaveDisplay(DeckManager::getInstance()->getDeck(i)->getFilePlayer(), 44100));
			addAndMakeVisible(draggableWaveDisplays[i]);
		}
				
		// create the zoom & playhead sliders
		addAndMakeVisible( zoomSlider = new Slider("zoomSlider") );
		zoomSlider->setSliderStyle(Slider::LinearVertical);
		zoomSlider->setRange(0.0, 200.0, 0.0001);
		zoomSlider->setSkewFactorFromMidPoint(10);
		zoomSlider->setValue(10);
		zoomSlider->addListener(this);
		
		addAndMakeVisible( playheadPosSlider = new Slider("playheadPosSlider") );
		playheadPosSlider->setSliderStyle(Slider::LinearVertical);
		playheadPosSlider->setRange(0.0, 1.0, 0.0001);
		playheadPosSlider->setValue(0.5);
		playheadPosSlider->addListener(this);
	}
	
	~DraggableDisplay()
	{
		draggableWaveDisplays.clear();
		
		deleteAllChildren();
	}
	
	//================================================================
	void resized()
	{
		const int height = getHeight();
		const int width = getWidth();
		const int margin = 5;
		const int sliderWidth = 20;
		
		zoomSlider->setBounds(margin, margin,
							  sliderWidth, height-2*margin);

		playheadPosSlider->setBounds(width-margin-sliderWidth, margin,
									 sliderWidth, height-2*margin);				

		
		//================================================================
		const int noDecks = settings->getMaxNoDecks();
		int noEnabledDecks = DeckManager::getNoEnabledDecks();
		
		if (noEnabledDecks)
		{
			Value tempTrue(true);
			const int waveDisplayWidth = (playheadPosSlider->getX()-zoomSlider->getRight())-2*margin;
			const int waveDisplayHeight = (height - (noEnabledDecks+1)*margin)/noEnabledDecks;
			
			int pos = 0;
			for (int i = 0; i < noDecks; i++)
			{
				if (settings->getDeck(i)->getChannelSetting(Deck::on) == tempTrue) {
					draggableWaveDisplays[i]->setVisible(true);
					draggableWaveDisplays[i]->setBounds(zoomSlider->getRight()+margin, margin+(pos*(waveDisplayHeight+margin)), waveDisplayWidth, waveDisplayHeight);
					pos++;
				}
				else
					draggableWaveDisplays[i]->setVisible(false);
			}
		}
		else {
			for (int i = 0; i < noDecks; i++)
				draggableWaveDisplays[i]->setVisible(false);
		}
		//================================================================
		
		// set up first display for other to run off
//		int firstDisplayNo = 0;
//		for (int i = 0; i < noDecks; i++) {
//			if (draggableWaveDisplays[i])
//				firstDisplayNo = i;
//			break;
//		}
//		
//		draggableWaveDisplays[firstDisplayNo]->setBounds(zoomSlider->getRight()+margin, margin,
//														 (playheadPosSlider->getX()-zoomSlider->getRight())-2*margin,
//														 waveDisplayHeight);
//		int nextDisplayNo = firstDisplayNo;
//		for (int i = firstDisplayNo+1; i < noDecks; i++)
//		{
//			if (draggableWaveDisplays[i])
//				draggableWaveDisplays[i]->setBounds(zoomSlider->getRight()+margin, draggableWaveDisplays[nextDisplayNo]->getBottom()+margin,
//													(playheadPosSlider->getX()-zoomSlider->getRight())-2*margin, waveDisplayHeight);
//			nextDisplayNo = i;
//		}
	}
	
	void paint (Graphics &g)
	{
		g.setColour(DecksLookAndFeel::getInstance()->getDecksColour(DecksLookAndFeel::panelColour));
		g.fillAll();
	}
	
	//================================================================
	void deckChanged (Deck *deck)
	{
		resized();
	}

	void sliderValueChanged(Slider *slider)
	{
		if (slider == zoomSlider)
		{
			const int zoomFactor = zoomSlider->getValue();
			for (int i = 0; i < noDecks; i++)
				draggableWaveDisplays[i]->setZoomFactor(zoomFactor);
		}
		else if (slider == playheadPosSlider)
		{
			for (int i = 0; i < noDecks; i++)
				draggableWaveDisplays[i]->setPlayheadPosition(playheadPosSlider->getValue());
		}
	}
	
	//================================================================
private:
	Slider *zoomSlider, *playheadPosSlider;

	int noDecks;
	DeckManager *settings;
	OwnedArray <DraggableWaveDisplay> draggableWaveDisplays;
	
};

#endif //_DRAGGABLEDISPLAY__H_