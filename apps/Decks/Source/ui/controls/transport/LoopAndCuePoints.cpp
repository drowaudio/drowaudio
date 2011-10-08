/*
  ==============================================================================

    LoopAndCuePoints.cpp
    Created: 28 Nov 2010 7:05:04am
    Author:  David Rowland

  ==============================================================================
*/

#include "LoopAndCuePoints.h"

#include "LoopPointList.h"
#include "CuePointList.h"

LoopAndCuePoints::LoopAndCuePoints(AudioFilePlayer* filePlayer)
{
	addAndMakeVisible(showHideButton = new DrawableButton("Loops", DrawableButton::ImageOnButtonBackground));
	DrawablePath offPath(DecksLookAndFeel::createIcon(DecksLookAndFeel::Play, Colours::lightgrey));
	DrawablePath onPath(DecksLookAndFeel::createIcon(DecksLookAndFeel::DownTriangle, Colours::lightgrey));
	showHideButton->setImages(&offPath, 0, 0, 0, &onPath);
	showHideButton->setBackgroundColours(Colours::darkgrey, Colours::darkgrey);
	showHideButton->setClickingTogglesState(true);
	
	tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtRight);
	addAndMakeVisible(tabbedComponent);
	tabbedComponent->setTabBarDepth(20);
	tabbedComponent->setOutline(0);
	tabbedComponent->addTab("Loop", Colour::greyLevel(0.3), new LoopPointList(filePlayer), true);
	tabbedComponent->addTab("Cue", Colour::greyLevel(0.3), new CuePointList(filePlayer), true);
}

LoopAndCuePoints::~LoopAndCuePoints()
{
	deleteAllChildren();
}

void LoopAndCuePoints::paint(Graphics &g)
{
	const int h = getHeight();
	const int w = getWidth();
	const int m = 5;
	
	const int cellH = 20;
	const int numPoints = h / cellH;

//	g.setColour(Colours::red);
//	for (int i = 0; i < numPoints; i++) {
//		g.fillRect(1, 1 + (i * (cellH + 1)), w-2, cellH-2);
//	}
}

void LoopAndCuePoints::resized()
{
	const int h = getHeight();
	const int w = getWidth();
	const int m = 5;
	
//	tabbedComponent->setBounds(getBounds());
	showHideButton->setBounds(0, 0, w, 10);
	tabbedComponent->setBounds(0, showHideButton->getBottom() + m, w, h - showHideButton->getBottom()-m);
}

void LoopAndCuePoints::setToggleValueToReferTo(Value &valueToReferTo)
{
	showHideButton->getToggleStateValue().referTo(valueToReferTo);
}