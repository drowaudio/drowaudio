/*
 *  MasterChannelStrip.cpp
 *  Decks
 *
 *  Created by David Rowland on 03/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "MasterChannelStrip.h"

MasterChannelStrip::MasterChannelStrip()
{
	addAndMakeVisible( gainSlider = new Slider(T("gainSlider")) );
	gainSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	gainSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	gainSlider->setRange(Defaults::Mixer::Master::gainMin, Defaults::Mixer::Master::gainMax);
	gainSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfMasterAsValue(MASTER_SETTING(gain)));
	
	addAndMakeVisible(meterL = new SegmentedMeter());
	addAndMakeVisible(meterR = new SegmentedMeter());
	
	addAndMakeVisible(cueButton = new TextButton(T("cue")));
	cueButton->setClickingTogglesState(true);
	cueButton->setColour(TextButton::buttonOnColourId, Colours::orangered);
	cueButton->setToggleState(Defaults::Mixer::Master::cue, false);
	cueButton->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfMasterAsValue(MASTER_SETTING(cue)));
	
	addAndMakeVisible( faderCurveSlider = new Slider(T("faderCurveSlider")) );
	faderCurveSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	faderCurveSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	faderCurveSlider->setRange(Defaults::Mixer::Master::faderCurveMin, Defaults::Mixer::Master::faderCurveMax);
	faderCurveSlider->setValue(Defaults::Mixer::Master::faderCurve);
	faderCurveSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfMasterAsValue(MASTER_SETTING(faderCurve)));
	
	addAndMakeVisible( faderCurveLabel = new Label(T("faderCurveLabel"), T("Ch Curve")) );
	faderCurveLabel->setJustificationType(Justification::centred);
	faderCurveLabel->setFont(12);
	
	addAndMakeVisible( xFaderCurveSlider = new Slider(T("xFaderCurveSlider")) );
	xFaderCurveSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	xFaderCurveSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	xFaderCurveSlider->setRange(Defaults::Mixer::Master::xFaderCurveMin, Defaults::Mixer::Master::xFaderCurveMax);
	xFaderCurveSlider->setValue(Defaults::Mixer::Master::xFaderCurve);
	xFaderCurveSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfMasterAsValue(MASTER_SETTING(xFaderCurve)));
	
	addAndMakeVisible( xFaderCurveLabel = new Label(T("xFaderCurveLabel"), T("X Curve")) );
	xFaderCurveLabel->setJustificationType(Justification::centred);
	xFaderCurveLabel->setFont(12);
}

MasterChannelStrip::~MasterChannelStrip()
{
	deleteAllChildren();
	
	DBG("MasterChannelStrip deleted");
}

//================================================================
void MasterChannelStrip::resized()
{
	int w = getWidth();
	int c = w/2;
	int r = 30;
	int m = 5;
	
	gainSlider->setBounds(c-r/2, m, r, r);
	
	int meterW = 8;
	meterL->setBounds(c-2.5*meterW, 40, meterW, 200);
	meterR->setBounds(c+1.75*meterW, 40, meterW, 200);
	
	int cueWidth = 0.6*w;
	cueButton->setBounds(c-(cueWidth/2), 250, cueWidth, 15);
	
	faderCurveSlider->setBounds(c-r/2, cueButton->getBottom()+m, r, r);
	faderCurveLabel->setBounds(0, faderCurveSlider->getBottom(), w, 12);
	xFaderCurveSlider->setBounds(c-r/2, faderCurveSlider->getBottom()+5*m, r, r);
	xFaderCurveLabel->setBounds(0, xFaderCurveSlider->getBottom(), w, 12);
}

void MasterChannelStrip::paint(Graphics &g)
{
	g.setColour(DecksColours::getInstance()->getColour(DecksColours::panelColour));
	g.fillAll();
	
	// draw meter numbers
	const int noSegments = meterL->getTotalNoSegments();
	const float segHeight = (meterL->getHeight()) / (float)noSegments;
	const float offset = segHeight/2.0f;//meterL->getHeight()/ (float)(noSegments+1);
	const int startY = meterL->getY()+offset;
	//		const int endY = meterL->getBottom();
	const int left = meterL->getRight() + 5;
	const int right = meterR->getX() - 5;
	const int centreX = left + (0.5f*(right - left));
	const int dbInc = meterL->getNoDbPerSegment();
	int db = dbInc*meterL->getNoOverSegments();
	
	g.setColour(DecksColours::getInstance()->getColour(DecksColours::meterLineColour));
	g.setFont(10);
	for (int i=0; i < noSegments; i++) {
		//			g.drawHorizontalLine(startY+(i*segHeight), left, right);
		String text("");
		if (db > 0) {
			text << String("+")<<String(db);
		}
		else {
			text << String(db);
		}
		
		g.drawText(text, centreX-10, startY+(i*segHeight)-5, 20, 10, Justification::centred, false);
		db -= dbInc;
	}
}
