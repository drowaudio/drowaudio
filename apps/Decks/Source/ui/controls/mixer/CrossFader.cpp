/*
 *  CrossFader.cpp
 *  Decks
 *
 *  Created by David Rowland on 04/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "CrossFader.h"
#include "Mixer.h"

CrossFader::CrossFader()
:	manager(DeckManager::getInstance())
{
	// listen for the XFader curve slider in the master strip
//	mixer->getMasterChannelStrip()->xFaderCurveSlider->addListener(this);
//	Settings::getInstance()->getValueTreePointer()->getChildWithName("master").getPropertyAsValue("xFaderCurve", 0).addListener(this);
	Settings::getInstance()->getValueTreePointer()->addListener(this);
	
	addAndMakeVisible( xFaderSlider = new Slider("xFaderSlider") );
	xFaderSlider->setSliderStyle(Slider::LinearHorizontal);
	xFaderSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	xFaderSlider->setRange(0.0f, 1.0f);
//	xFaderSlider->setValue(Defaults::Mixer::XFader::fader);
	xFaderSlider->setSkewFactor(Settings::getInstance()->getPropertyOfMaster(MASTER_SETTING(xFaderCurve)));
	xFaderSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfXFaderAsValue(XFADER_SETTING(level)));

	
	addAndMakeVisible( xAssignSlider = new Slider("xAssignSlider") );
	xAssignSlider->setRange(0, manager->getMaxNoDecks(), 1);
	xAssignSlider->setRotaryParameters(-1.5, 1.5, true);
	xAssignSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	xAssignSlider->addListener(this);
	xAssignSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
//	xAssignSlider->setValue(Defaults::Mixer::XFader::assignX);
	xAssignSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfXFaderAsValue(XFADER_SETTING(assignX)));

	addAndMakeVisible( xAssginLabel = new Label("xAssginLabel", "X Assign") );
	xAssginLabel->setJustificationType(Justification::centred);
	xAssginLabel->setFont(12);
	
	addAndMakeVisible( yAssignSlider = new Slider("yAssignSlider") );
	yAssignSlider->setRange(0, manager->getMaxNoDecks(), 1);
	yAssignSlider->setRotaryParameters(-1.5, 1.5, true);
	yAssignSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	yAssignSlider->addListener(this);
	yAssignSlider->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
//	yAssignSlider->setValue(Defaults::Mixer::XFader::assignY);
	yAssignSlider->getValueObject().referTo(Settings::getInstance()->getPropertyOfXFaderAsValue(XFADER_SETTING(assignY)));
	
	addAndMakeVisible( yAssignLabel = new Label("yAssginLabel", "Y Assign") );
	yAssignLabel->setJustificationType(Justification::centred);
	yAssignLabel->setFont(12);
}

void CrossFader::resized()
{
	int w = getWidth();
	int c = w/2;
	int h = getHeight();
	int ch = h/2;
	int m = 5;
	
	int xFaderWidth = w*0.5;
	xFaderSlider->centreWithSize(xFaderWidth, h*0.6);

	int assignSize = h*0.5;
	xAssignSlider->setBounds(xFaderSlider->getX()-(1.5*assignSize), ch-(assignSize/2), assignSize, assignSize);
	xAssginLabel->setBounds(xAssignSlider->getX(), xAssignSlider->getBottom()+m, xAssignSlider->getWidth(), 12);
	yAssignSlider->setBounds(xFaderSlider->getRight()+(0.5*assignSize), ch-(assignSize/2), assignSize, assignSize);
	yAssignLabel->setBounds(yAssignSlider->getX(), yAssignSlider->getBottom()+m, yAssignSlider->getWidth(), 12);
}

void CrossFader::paint(Graphics &g)
{
	g.setColour(DecksLookAndFeel::getInstance()->getDecksColour(DecksLookAndFeel::panelColour));
	g.fillAll();
	
	// draw selection backgrounds
	float anglePerDeck = 3.0f / (manager->getMaxNoDecks());
	float Xcx = getCentreX(xAssignSlider);
	float Xcy = getCentreY(xAssignSlider);
	float Ycx = getCentreX(yAssignSlider);
	float Ycy = getCentreY(yAssignSlider);

	g.setColour(DecksLookAndFeel::getInstance()->getDecksColour(DecksLookAndFeel::meterLineColour));

	Path pX, pY;
	pX.clear();
	pX.startNewSubPath(Xcx, Xcy);
	pX.lineTo(Xcx, Xcy-(xAssignSlider->getWidth()*0.6));
	
	pY.clear();
	pY.startNewSubPath(Ycx, Ycy);
	pY.lineTo(Ycx, Ycy-(yAssignSlider->getWidth()*0.6));
	
		
	g.setFont(12);
	for (int i=0; i <= int(Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels")); i++) {
		AffineTransform xTranform(AffineTransform::identity.rotated((-1.5 + (i*anglePerDeck)), Xcx, Xcy));
		g.strokePath(pX, PathStrokeType(2, PathStrokeType::curved, PathStrokeType::rounded), xTranform);

		AffineTransform yTranform(AffineTransform::identity.rotated((-1.5 + (i*anglePerDeck)), Ycx, Ycy));
		g.strokePath(pY, PathStrokeType(2, PathStrokeType::curved, PathStrokeType::rounded), yTranform);

		float xTempX = Xcx, xTempY = Xcy-35;
		xTranform.transformPoint(xTempX, xTempY);

		float yTempX = Ycx, yTempY = Ycy-35;
		yTranform.transformPoint(yTempX, yTempY);

		String text(i);
		i == 0 ? text = "off" : text = String(i);
		g.drawText(text, xTempX-7.5, xTempY-5, 15, 10, Justification::centred, false);
		g.drawText(text, yTempX-7.5, yTempY-5, 15, 10, Justification::centred, false);
	}
	
	// draw x-fader lines
	const int start = xFaderSlider->getX()+20;
	const int end = xFaderSlider->getRight()-20;
	const int top = xFaderSlider->getY()-5;
	const int bottom = xFaderSlider->getBottom()+5;
	const float lineSpace = (end-start)/12.0f;

	g.setColour(DecksLookAndFeel::getInstance()->getDecksColour(DecksLookAndFeel::meterLineColour));
	g.drawLine(start, top, start, bottom, 2);
	g.drawLine(end, top, end, bottom, 2);
	for (int i=1; i<12; i++) {
		if (i == 6)
			g.drawLine(start+(6*lineSpace), top, start+(6*lineSpace), bottom, 2);	
		else
			g.drawVerticalLine(start+(i*lineSpace), top, bottom);
	}
}

void CrossFader::valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property)
{
	if (property == MASTER_SETTING(xFaderCurve))
	{
		DBG("xFader Curve adjusting");
		// we need to keep the slider posisition the same and adjust the level
		double proportion = xFaderSlider->valueToProportionOfLength(xFaderSlider->getValue());
		xFaderSlider->setSkewFactor(Settings::getInstance()->getPropertyOfMaster(MASTER_SETTING(xFaderCurve)));
		xFaderSlider->setValue(xFaderSlider->proportionOfLengthToValue(proportion));
	}
}

void CrossFader::valueTreeChildrenChanged (ValueTree &treeWhoseChildHasChanged)
{
//	DBG("child changed");
}

void CrossFader::valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged)
{
//	DBG("parent changed");
}