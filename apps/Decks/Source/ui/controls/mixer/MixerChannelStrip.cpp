/*
 *  MixerChannelStrip.cpp
 *  Decks
 *
 *  Created by David Rowland on 03/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "MixerChannelStrip.h"
#include "../../../main/DefaultSettings.h"
#include "Mixer.h"

MixerChannelStrip::MixerChannelStrip(int deckNo_, Mixer *mixer_)
:	settings(DeckManager::getInstance()),
	deckNo(deckNo_),
	mixer(mixer_)
{
//	mixer->getMasterChannelStrip()->faderCurveSlider->addListener(this);
	Settings::getInstance()->getValueTreePointer()->addListener(this);
		
	DrawablePath powerOffIcon(DecksLookAndFeel::createIcon(DecksLookAndFeel::Power, findColour(TextButton::textColourOffId)));
	DrawablePath powerOnIcon(DecksLookAndFeel::createIcon(DecksLookAndFeel::Power, findColour(TextButton::textColourOnId)));
	addAndMakeVisible(onButon = new DrawableButton(T("onButon"), DrawableButton::ImageOnButtonBackground));
	onButon->setImages(&powerOffIcon, 0, 0, 0, &powerOnIcon);
	onButon->setBackgroundColours(Colours::darkgrey, Colours::lightgrey);
	onButon->setClickingTogglesState(true);
	onButon->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(on)));

	DrawablePath bypassOffIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Bypass, findColour(TextButton::textColourOffId)));
	DrawablePath bypassOnIcon (DecksLookAndFeel::createIcon(DecksLookAndFeel::Bypass, findColour(TextButton::textColourOnId)));
	addAndMakeVisible(bypassButton = new DrawableButton(T("bypass"), DrawableButton::ImageOnButtonBackground));
	bypassButton->setImages(&bypassOffIcon, 0, 0, 0, &bypassOnIcon);
	bypassButton->setClickingTogglesState(true);
	bypassButton->setBackgroundColours(Colours::darkgrey, Colours::lightgrey);
	bypassButton->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(bypass)));

	addAndMakeVisible(gainSlider = new Slider(T("gainSlider")));
	gainSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	gainSlider->setRange(Defaults::Mixer::Channel::gainMin, Defaults::Mixer::Channel::gainMax);
	gainSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(gain)));

	addAndMakeVisible(meter = new SegmentedMeter());
	
	for (int i = 0; i < noBands; i++)
	{
		eqSliders.add(new Slider(T("")));
		addAndMakeVisible(eqSliders[i]);
		eqSliders[i]->setSliderStyle(Slider::RotaryVerticalDrag);
		eqSliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
		
		eqKillButtons.add(new TextButton(T("K")));
		addAndMakeVisible(eqKillButtons[i]);
		eqKillButtons[i]->setClickingTogglesState(true);
		
		eqLabels.add(new Label(T(""), T("")));
		addAndMakeVisible(eqLabels[i]);
		eqLabels[i]->setFont(Font(12));
		eqLabels[i]->setJustificationType(Justification(Justification::horizontallyCentred));
	}
	eqLabels[high]->setText(T("High"), false);
	eqLabels[mid]->setText(T("Mid"), false);
	eqLabels[low]->setText(T("Low"), false);
	
	eqSliders[high]->setRange(Defaults::Mixer::Channel::HighEQ::gainMin, Defaults::Mixer::Channel::HighEQ::gainMax);
	eqSliders[high]->getValueObject().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(highGain)));
	eqKillButtons[high]->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(highKill)));
	
	eqSliders[mid]->setRange(Defaults::Mixer::Channel::MidEQ::gainMin, Defaults::Mixer::Channel::MidEQ::gainMax);
	eqSliders[mid]->getValueObject().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(midGain)));
	eqKillButtons[mid]->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(midKill)));

	eqSliders[low]->setRange(Defaults::Mixer::Channel::LowEQ::gainMin, Defaults::Mixer::Channel::LowEQ::gainMax);
	eqSliders[low]->getValueObject().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(lowGain)));
	eqKillButtons[low]->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(lowKill)));
	
	
	addAndMakeVisible(levelSlider = new Slider(T("levelSlider")));
	levelSlider->setSliderStyle(Slider::LinearVertical);
	levelSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	levelSlider->setRange(Defaults::Mixer::Channel::faderMin, Defaults::Mixer::Channel::faderMax);
	levelSlider->setSkewFactor(Settings::getInstance()->getPropertyOfMaster(MASTER_SETTING(faderCurve)));
	levelSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(level)));

	addAndMakeVisible(cueButton = new TextButton(T("cue")));
//	cueButton->addListener(this);
	cueButton->setClickingTogglesState(true);
	cueButton->setColour(TextButton::buttonOnColourId, Colours::orangered);
	cueButton->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(cue)));
	

	addAndMakeVisible( fxASlider = new Slider(T("fxASlider")) );
	fxASlider->setSliderStyle(Slider::RotaryVerticalDrag);
	fxASlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	(new Label(T("fxALabel"), T("fxA")))->attachToComponent(fxASlider, true);
	fxASlider->setRange(Defaults::Mixer::Channel::fxSendMin, Defaults::Mixer::Channel::fxSendMax);
	fxASlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(fxASend)));

	addAndMakeVisible( fxBSlider = new Slider(T("fxBSlider")) );
	fxBSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	fxBSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	(new Label(T("fxBSlider"), T("fxB")))->attachToComponent(fxBSlider, true);
	fxBSlider->setRange(Defaults::Mixer::Channel::fxSendMin, Defaults::Mixer::Channel::fxSendMax);
	fxBSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfChannelAsValue(deckNo, CHANNEL_SETTING(fxBSend)));
}

MixerChannelStrip::~MixerChannelStrip()
{
	Settings::getInstance()->getValueTreePointer()->removeListener(this);
	
	eqSliders.clear();
	eqKillButtons.clear();
	eqLabels.clear();
	deleteAllChildren();
	
	String message("MixerChannelStrip ");
	message << deckNo << " deleted";
	DBG(message);		
}

void MixerChannelStrip::resized()
{
	int w = getWidth();
	int c = w/2;
	int h = getHeight();
	int m = 5;
	
	int topButtonSize = (w-m)/4 - m;//30;
	onButon->setBounds(c-(topButtonSize+(m/2)), m, topButtonSize, topButtonSize);
	bypassButton->setBounds(c+(m/2), m, topButtonSize, topButtonSize);
	gainSlider->setBounds(c+(w/4)+(m/2), m, topButtonSize, topButtonSize);
	
	int eqSize = 50;//w/3;
	for (int i = 0; i < noBands; i++)
	{
		eqSliders[i]->setBounds(c-(eqSize/2), bypassButton->getBottom()+5+(i*(eqSize+4*m)), eqSize, eqSize);
		eqKillButtons[i]->setBounds(eqSliders[i]->getX()-20, eqSliders[i]->getBottom()-20, 20, 20);
		eqLabels[i]->setBounds(eqSliders[i]->getX(), eqSliders[i]->getBottom(), eqSliders[i]->getWidth(), 15);
	}
	
	levelSlider->setBounds(eqSliders[0]->getRight()+5, gainSlider->getBottom()+5, w-eqSliders[0]->getRight()-2*m, 200);
	
	int cueWidth = 0.6*w;
	cueButton->setBounds(c-(cueWidth/2), levelSlider->getBottom()+2*m, cueWidth, 15);
	
	int fxWidth = topButtonSize;//w/4;
	fxASlider->setBounds(c-fxWidth, cueButton->getBottom()+m, fxWidth, fxWidth);
	fxBSlider->setBounds(c+fxWidth, cueButton->getBottom()+m, fxWidth, fxWidth);
	
	meter->setBounds(m, 40, 8, 200);

}

void MixerChannelStrip::paint(Graphics &g)
{
	g.setColour(DecksColours::getInstance()->getColour(DecksColours::panelColour));
	g.fillAll();
	
	int m = 5;
	const int noSize = onButon->getWidth()-2;
	g.setColour(Colours::white);
	g.setFont(noSize);
	g.drawEllipse(m, m, noSize, noSize, 1);
	g.drawFittedText(String(deckNo),
					 m, m, noSize, noSize,
					 Justification::centred, 2, 1);	
	
	// draw fader lines
	const int indent = DecksLookAndFeel::getInstance()->getSliderThumbRadius(*levelSlider);
	const int start = levelSlider->getY()+indent;
	const int end = levelSlider->getBottom()-indent;
	const int left = levelSlider->getX()+5;
	const int right = levelSlider->getRight()-5;
	const float lineSpace = (end-start)/11.0f;
	
	g.setColour(DecksColours::getInstance()->getColour(DecksColours::meterLineColour));
	g.drawLine(left-5, start, right+5, start, 2);
	g.drawLine(left-5, end, right+5, end, 2);
	for (int i=1; i<11; i++) {
		g.drawHorizontalLine(start+(i*lineSpace), left, right);
	}
}

void MixerChannelStrip::valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property)
{
	if (property == MASTER_SETTING(faderCurve))
	{
		DBG("fader curve adjusting");
		DBG(treeWhosePropertyHasChanged.getProperty(property).toString());
		double proportion = levelSlider->valueToProportionOfLength(levelSlider->getValue());
		levelSlider->setSkewFactor(Settings::getInstance()->getPropertyOfMaster(MASTER_SETTING(faderCurve)));
		levelSlider->setValue(levelSlider->proportionOfLengthToValue(proportion));
	}
}