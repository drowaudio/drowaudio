/*
 *  DeckTransport.cpp
 *  Decks
 *
 *  Created by David Rowland on 05/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "DeckTransport.h"

DeckTransport::DeckTransport(int deckNo_)
:	deckNo(deckNo_),
	showLoopAndCuePoints(true),
	settings(DeckManager::getInstance()),
	filePlayer(settings->getDeck(deckNo)->getMainFilePlayer())
{
	getLookAndFeel().setColour(TextButton::textColourOffId, Colour::greyLevel(0.8));
	filePlayer->addChangeListener(this);
	
	addAndMakeVisible(infoBox = new TrackInfo(deckNo, filePlayer));
	
	for (int i = 0; i < noTransportButtons; i++) {
		transportButtons.add(new DrawableButton(String(i), DrawableButton::ImageOnButtonBackground));
		
		addAndMakeVisible(transportButtons[i]);
		transportButtons[i]->addListener(this);
		transportButtons[i]->setBackgroundColours(Colours::darkgrey, Colours::lightgrey);
	}
	transportButtons[loop]->setClickingTogglesState(true);
	transportButtons[playPause]->setClickingTogglesState(true);
	
	DrawablePath ejectIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Eject, findColour(TextButton::textColourOffId)));
	DrawablePath loopOffIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Infinity, findColour(TextButton::textColourOffId)));
	DrawablePath loopOnIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Infinity, findColour(TextButton::textColourOnId)));
	DrawablePath cueIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Cue, findColour(TextButton::textColourOffId)));
	DrawablePath previousIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Previous, findColour(TextButton::textColourOffId)));
	DrawablePath shuffleBackIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::ShuffleBack, findColour(TextButton::textColourOffId)));
	DrawablePath stopIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Stop, findColour(TextButton::textColourOffId)));
	DrawablePath playIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Play, findColour(TextButton::textColourOffId)));
	DrawablePath pauseIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Pause, findColour(TextButton::textColourOnId)));
	DrawablePath shuffleForwardIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::ShuffleForward, findColour(TextButton::textColourOffId)));
	DrawablePath nextIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Next, findColour(TextButton::textColourOffId)));
	
	transportButtons[eject]->setImages(&ejectIcon);
	transportButtons[loop]->setImages(&loopOffIcon, 0, 0, 0, &loopOnIcon);
	transportButtons[cue]->setImages(&cueIcon);
	transportButtons[previous]->setImages(&previousIcon);
	transportButtons[shuffleBack]->setImages(&shuffleBackIcon);
	transportButtons[stop]->setImages(&stopIcon);
	transportButtons[playPause]->setImages(&playIcon, 0, 0, 0, &pauseIcon);
	transportButtons[shuffleForward]->setImages(&shuffleForwardIcon);
	transportButtons[next]->setImages(&nextIcon);
	
	addAndMakeVisible(waveDisplay = new PositionableWaveDisplay(filePlayer));
	
	addAndMakeVisible(loopAndCuePoints = new LoopAndCuePoints());
	
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

DeckTransport::~DeckTransport()
{
	speedSlider->removeListener(this);
	jogSlider->removeListener(this);
	for (int i = 0; i < transportButtons.size(); i++) {
		transportButtons[i]->removeListener(this);
	}
	
	transportButtons.clear();
	deleteAllChildren();
	
	String message("DeckTransport ");
	message << deckNo << " deleted";
	DBG(message);
}

//==============================================================================
void DeckTransport::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	infoBox->setBounds(0, 0, w-35, 40);
	
	const int buttonWidth = /*jmin(*/((infoBox->getWidth()-2)/(float)noTransportButtons) - 1;
	for (int i = 0; i < noTransportButtons; i++) {
		transportButtons[i]->setBounds(2+(i*(buttonWidth+1)), infoBox->getBottom()+2, buttonWidth, 20);
	}
	
	waveDisplay->setBounds(0, transportButtons[0]->getBottom()+2, w-35, 50);
	
	if (showLoopAndCuePoints) {
		loopAndCuePoints->setBounds(0, waveDisplay->getBottom()+m, w, h - waveDisplay->getBottom() - m);
	}
	else {
		loopAndCuePoints->setBounds(0, waveDisplay->getBottom()+m, w, h - waveDisplay->getBottom() - m);
	}
	
	speedSlider->setBounds(w-30, 0, 10, waveDisplay->getBottom());
	jogSlider->setBounds(w-15, 0, 10, waveDisplay->getBottom());
}

void DeckTransport::paint(Graphics &g)
{
	g.setColour(DecksColours::getInstance()->getColour(DecksColours::panelColour));
	g.fillAll();
}

//==============================================================================
void DeckTransport::buttonClicked(Button* button)
{
	if (button == transportButtons[loop])
		settings->getDeck(deckNo)->getMainFilePlayer()->setLooping(transportButtons[loop]->getToggleState());
	else if (button == transportButtons[cue])
		settings->getDeck(deckNo)->startFromZero();
	else if (button == transportButtons[eject])
		settings->getDeck(deckNo)->getMainFilePlayer()->setFile(String::empty);
	else if (button == transportButtons[previous])
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

void DeckTransport::buttonStateChanged (Button* button)
{
	if (button == transportButtons[shuffleForward])
	{
		if (transportButtons[shuffleForward]->isDown())
			settings->getDeck(deckNo)->setResamplingRatio ((speedSlider->getValue() + jogSlider->getValue())*2);
		else
			settings->getDeck(deckNo)->setResamplingRatio ((speedSlider->getValue() + jogSlider->getValue())*1);
	}
	
}

void DeckTransport::sliderValueChanged(Slider* changedSlider)
{
	if (changedSlider == speedSlider)
		settings->getDeck(deckNo)->setResamplingRatio (speedSlider->getValue() + jogSlider->getValue());
	else if (changedSlider == jogSlider)
		settings->getDeck(deckNo)->setResamplingRatio (speedSlider->getValue() + jogSlider->getValue());
}

void DeckTransport::sliderDragEnded(Slider* changedSlider)
{
	if (changedSlider == jogSlider)
		jogSlider->setValue(0.0);
}

void DeckTransport::changeListenerCallback(ChangeBroadcaster *object)
{
	//		filePlayer->setPlayDirection(false);
	
	if (object == filePlayer) {
		if (filePlayer->isPlaying())
			transportButtons[playPause]->setToggleState(true, false);
		else
			transportButtons[playPause]->setToggleState(false, false);
	}
}

void DeckTransport::setLoopAndCueValueToReferTo(Value &valueToReferTo)
{
	loopAndCuePoints->setToggleValueToReferTo(valueToReferTo);
}
