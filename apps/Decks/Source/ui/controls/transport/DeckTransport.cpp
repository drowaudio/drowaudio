/*
 *  DeckTransport.cpp
 *  Decks
 *
 *  Created by David Rowland on 05/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "DeckTransport.h"

#include "ClickableCuePointComponent.h"

DeckTransport::DeckTransport(int deckNo_)
:	deckNo(deckNo_),
	showLoopAndCuePoints(true),
	settings(DeckManager::getInstance()),
	filePlayer(settings->getDeck(deckNo)->getMainFilePlayer())
{
	filePlayer->getAudioTransportSource()->addChangeListener(this);
	
	addAndMakeVisible(infoBox = new TrackInfo(deckNo, filePlayer));
	
	for (int i = 0; i < noTransportButtons; i++) {
		transportButtons.add(new DrawableButton(String(i), DrawableButton::ImageOnButtonBackground));
		
		addAndMakeVisible(transportButtons[i]);
		transportButtons[i]->addListener(this);
		transportButtons[i]->setBackgroundColours(Colours::darkgrey, Colours::lightgrey);
	}
	transportButtons[loop]->setClickingTogglesState(true);
	transportButtons[playPause]->setClickingTogglesState(true);

	refreshTransportButtons();
	
	addAndMakeVisible(waveDisplay = new ColouredPositionableWaveDisplay(filePlayer,
                                                                        settings->getDeck(deckNo)->getThumbnailCache(),
                                                                        settings->getDeck(deckNo)->getThumbnail()));
	
    addAndMakeVisible(clickableCuePointComponent = new ClickableCuePointComponent(filePlayer));
    
	addAndMakeVisible(loopAndCuePoints = new LoopAndCuePoints(filePlayer));
	
	addAndMakeVisible(speedSlider = new Slider("speedSlider"));
	speedSlider->setSliderStyle(Slider::LinearVertical);
	speedSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
	speedSlider->setRange(0.8, 1.2, 0.0001);
	speedSlider->setValue(1.0);
	speedSlider->addListener(this);
	speedSlider->setLookAndFeel(InvertedSliderLookAndFeel::getInstance());

	addAndMakeVisible(speedLabel = new Label("speedLabel", "P"));
	speedLabel->setBorderSize(0, 0);
	speedLabel->setJustificationType(Justification(Justification::centred));
	speedLabel->attachToComponent(speedSlider, false);
	
	addAndMakeVisible(jogSlider = new Slider("jogSlider"));
	jogSlider->setSliderStyle(Slider::LinearVertical);
	jogSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
	jogSlider->setRange( -0.05, 0.05, 0.0001);
	jogSlider->setValue(0.0);
	jogSlider->addListener(this);
	jogSlider->setLookAndFeel(InvertedSliderLookAndFeel::getInstance());

	addAndMakeVisible(jogLabel = new Label("jogLabel", "N"));
	jogLabel->setBorderSize(0, 0);
	jogLabel->setJustificationType(Justification(Justification::centred));
	jogLabel->attachToComponent(jogSlider, false);	
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
	
	DBG("DeckTransport "<< deckNo << " deleted");
}

//==============================================================================
void DeckTransport::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	infoBox->setBounds(0, 0, w-35, 40);
	
	const float buttonWidth = /*jmin(*/((infoBox->getWidth()-2)/(float)noTransportButtons) - 1;
	for (int i = 0; i < noTransportButtons; i++) {
//		transportButtons[i]->setBounds(2+(i*(buttonWidth+1)), infoBox->getBottom()+2, buttonWidth, 20);
		transportButtons[i]->setBounds(RelativeRectangle(Rectangle<float>(2+(i*(buttonWidth+1)), infoBox->getBottom()+2, buttonWidth, 20)));
	}
	
	waveDisplay->setBounds (0, transportButtons[0]->getBottom()+2, w-35, 40);
    
    const int cuePointHeight = 10;
	clickableCuePointComponent->setBounds (waveDisplay->getX(), waveDisplay->getBottom() - cuePointHeight,
                                           waveDisplay->getWidth(), cuePointHeight);
    
	if (showLoopAndCuePoints) {
		loopAndCuePoints->setBounds(0, waveDisplay->getBottom()+m, w, h - waveDisplay->getBottom() - m);
	}
	else {
		loopAndCuePoints->setBounds(0, waveDisplay->getBottom()+m, w, h - waveDisplay->getBottom() - m);
	}
	
	const int labelHeight = 20;
	speedSlider->setBounds(w-30, labelHeight, 10, waveDisplay->getBottom()-labelHeight);
	speedSlider->setTransform(AffineTransform::rotation(float_Pi,
														speedSlider->getX()+speedSlider->getWidth()*0.5,
														speedSlider->getY()+speedSlider->getHeight()*0.5));
	jogSlider->setBounds(w-15, labelHeight, 10, waveDisplay->getBottom()-labelHeight);
	jogSlider->setTransform(AffineTransform::rotation(float_Pi,
													  jogSlider->getX()+jogSlider->getWidth()*0.5,
													  jogSlider->getY()+jogSlider->getHeight()*0.5));
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
	
	if (object == static_cast<ChangeBroadcaster*>(filePlayer->getAudioTransportSource())) {
		if (filePlayer->getAudioTransportSource()->isPlaying())
			transportButtons[playPause]->setToggleState(true, false);
		else
			transportButtons[playPause]->setToggleState(false, false);
	}
}

void DeckTransport::setLoopAndCueValueToReferTo(Value &valueToReferTo)
{
	loopAndCuePoints->setToggleValueToReferTo(valueToReferTo);
}

void DeckTransport::refreshTransportButtons()
{
	Colour offColour = findColour(TextButton::textColourOffId);
	Colour onColour = findColour(TextButton::textColourOnId);
	
	DrawablePath ejectIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Eject, offColour));
	DrawablePath loopOffIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Infinity, offColour));
	DrawablePath loopOnIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Infinity, onColour));
	DrawablePath cueIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Cue, offColour));
	DrawablePath previousIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Previous, offColour));
	DrawablePath shuffleBackIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::ShuffleBack, offColour));
	DrawablePath stopIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Stop, offColour));
	DrawablePath playIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Play, offColour));
	DrawablePath pauseIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Pause, onColour));
	DrawablePath shuffleForwardIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::ShuffleForward, offColour));
	DrawablePath nextIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Next, offColour));
	
	transportButtons[eject]->setImages(&ejectIcon);
	transportButtons[loop]->setImages(&loopOffIcon, 0, 0, 0, &loopOnIcon);
	transportButtons[cue]->setImages(&cueIcon);
	transportButtons[previous]->setImages(&previousIcon);
	transportButtons[shuffleBack]->setImages(&shuffleBackIcon);
	transportButtons[stop]->setImages(&stopIcon);
	transportButtons[playPause]->setImages(&playIcon, 0, 0, 0, &pauseIcon);
	transportButtons[shuffleForward]->setImages(&shuffleForwardIcon);
	transportButtons[next]->setImages(&nextIcon);	
}