/*
 *  DeckComponent.cpp
 *
 *  Created by David Rowland on 24/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "DeckComponent.h"

DeckComponent::DeckComponent(juce_wchar deckNumber_)
	:	deckNumber(deckNumber_),
		filePlayer(0)
{
	filePlayer = new FilteringAudioFilePlayer();
	
	// set up buttons
	addAndMakeVisible( stopButton = new TextButton("Stop"));
	stopButton->setConnectedEdges(Button::ConnectedOnRight);
	stopButton->addButtonListener(this);
	addAndMakeVisible( playPauseButton = new TextButton("Play/Pause"));
	playPauseButton->setConnectedEdges(Button::ConnectedOnLeft);
	playPauseButton->addButtonListener(this);
	addAndMakeVisible( cueButton = new TextButton("Cue"));
	cueButton->setConnectedEdges(Button::ConnectedOnRight + Button::ConnectedOnLeft);
	cueButton->addButtonListener(this);
	
	
	addAndMakeVisible( speedSlider = new Slider("speedSlider"));
	speedSlider->setTextBoxStyle(Slider::TextBoxRight, false, 50, 20);
	speedSlider->setRange(0.8, 1.2, 0.0001);
	speedSlider->setValue(1.0);
	speedSlider->addListener(this);
	
	addAndMakeVisible( speedSliderFine = new Slider("speedSliderFine"));
	speedSliderFine->setTextBoxStyle(Slider::TextBoxRight, false, 50, 20);
	speedSliderFine->setRange(-0.05, 0.05, 0.0001);
	speedSliderFine->setValue(0.0);
	speedSliderFine->addListener(this);
	
	addAndMakeVisible( gainSlider = new Slider("gainSlider") );
	gainSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
	gainSlider->setSliderStyle(Slider::LinearVertical);
	gainSlider->setRange(0.0, 1.00, 0.0001);
	gainSlider->setSkewFactor(0.5);
	gainSlider->setValue(1.0);
	gainSlider->addListener(this);
	
	// EQ sliders
	addAndMakeVisible(highEQSlider = new Slider("highEQSlider"));
	addAndMakeVisible(midEQSlider = new Slider("midEQSlider"));
	addAndMakeVisible(lowEQSlider = new Slider("lowEQSlider"));

	setupEQSlider(highEQSlider);
	setupEQSlider(midEQSlider);
	setupEQSlider(lowEQSlider);
}

void DeckComponent::setupEQSlider(Slider* EQslider)
{
	EQslider->setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
	EQslider->setSliderStyle(Slider::RotaryVerticalDrag);
	EQslider->setColour(Slider::rotarySliderFillColourId, Colours::grey);
	EQslider->setRange(0.1, 2.00, 0.0001);
	EQslider->setSkewFactorFromMidPoint(1.0);
	EQslider->setValue(1.0);
	EQslider->addListener(this);
}

DeckComponent::~DeckComponent()
{
	deleteAllChildren();
}

//============================================================
void DeckComponent::resized()
{
	const int currentWidth = getWidth();
	const int currentHeight = getHeight();
	
	int buttonHeight = 20;
	int buttonWidth = (currentWidth - 10) / 3.0;
	int buttonYPos = 5;
	stopButton->setBounds(5, buttonYPos, buttonWidth, buttonHeight);
	cueButton->setBounds(stopButton->getRight(), buttonYPos, buttonWidth, buttonHeight);
	playPauseButton->setBounds(cueButton->getRight(), buttonYPos, buttonWidth, buttonHeight);
	
	speedSlider->setBounds(5, stopButton->getBottom() + 5,
						   currentWidth-10, 20);
	speedSliderFine->setBounds(5, speedSlider->getBottom() + 5,
							   currentWidth-10, 20);
	
	if (deckNumber == '1')
		gainSlider->setBounds(getRight()-25, speedSliderFine->getBottom()+5,
							  20, currentHeight-(speedSliderFine->getBottom())-10);
	else if (deckNumber == '2')
		gainSlider->setBounds(5, speedSliderFine->getBottom()+5,
							  20, currentHeight-(speedSliderFine->getBottom())-10);
	
	if (deckNumber == '1')
	{
		highEQSlider->setBounds(gainSlider->getX()-55, speedSliderFine->getBottom()+5, 50, 50);
		midEQSlider->setBounds(gainSlider->getX()-55, highEQSlider->getBottom()+5, 50, 50);
		lowEQSlider->setBounds(gainSlider->getX()-55, midEQSlider->getBottom()+5, 50, 50);
	}
	else if (deckNumber == '2')
	{
		highEQSlider->setBounds(30, speedSliderFine->getBottom()+5, 50, 50);
		midEQSlider->setBounds(30, highEQSlider->getBottom()+5, 50, 50);
		lowEQSlider->setBounds(30, midEQSlider->getBottom()+5, 50, 50);
	}
}

void DeckComponent::paint(Graphics& g)
{
	
}
//============================================================
void DeckComponent::buttonClicked(Button* button)
{
	if (button == cueButton)
		filePlayer->startFromZero();
	else if (button == stopButton)
	{
		filePlayer->stop();
		filePlayer->setNextReadPosition(0);
	}
	else if (button == playPauseButton)
		filePlayer->pause();
}

void DeckComponent::sliderValueChanged(Slider* changedSlider)
{
	if (changedSlider == gainSlider)
		filePlayer->setGain(gainSlider->getValue());
	else if (changedSlider == speedSlider)
		filePlayer->setResamplingRatio (speedSlider->getValue() + speedSliderFine->getValue());
	else if (changedSlider == speedSliderFine)
		filePlayer->setResamplingRatio (speedSlider->getValue() + speedSliderFine->getValue());
	else if (changedSlider == highEQSlider)
		filePlayer->setHighEQGain(highEQSlider->getValue());
	else if (changedSlider == midEQSlider)
		filePlayer->setMidEQGain(midEQSlider->getValue());
	else if (changedSlider == lowEQSlider)
		filePlayer->setLowEQGain(lowEQSlider->getValue());
}
void DeckComponent::sliderDragEnded(Slider* changedSlider)
{
	if (changedSlider == speedSliderFine)
		speedSliderFine->setValue(0.0);
}