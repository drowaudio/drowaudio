/*
  ==============================================================================

    QuitScreen.cpp
    Created: 26 Nov 2010 11:43:02pm
    Author:  David Rowland

  ==============================================================================
*/

#include "QuitScreen.h"

QuitScreen::QuitScreen ()
{
	setWantsKeyboardFocus(true);
	
	addAndMakeVisible( yesButton = new TextButton("Yes") );
	yesButton->addListener(this);
	addAndMakeVisible( noButton = new TextButton("No") );
	noButton->addListener(this);	
}

QuitScreen::~QuitScreen ()
{
	yesButton->removeListener(this);
	noButton->removeListener(this);

	deleteAllChildren();
}

//==============================================================================
void QuitScreen::resized ()
{
	const int w = getWidth();
	const int h = getHeight();

	yesButton->setBounds(w*0.5-60, h*0.5, 55, 20);
	noButton->setBounds(w*0.5+5, h*0.5, 55, 20);
}

void QuitScreen::paint (Graphics& g)
{
	const int w = getWidth();
	const int h = getHeight();

	g.setColour(Colours::darkgrey);
	g.fillAll();
	
	g.setColour(Colours::lightgrey);
	g.drawText ("Are you sure you want to quit?",
				w*0.5 - 100, h*0.5-50, 200, 20,
				Justification(Justification::centred),
				true);
}

void QuitScreen::visibilityChanged()
{
	if (isVisible()) {
		grabKeyboardFocus();
	}
}

bool QuitScreen::keyPressed (const KeyPress& key)
{
	if (key == KeyPress::returnKey) {
		yesButton->triggerClick();
	}
	else
		setVisible(false);
	
	return true;
}

//==============================================================================
void QuitScreen::buttonClicked(Button *button)
{
	if (button == yesButton) {
		JUCEApplication::getInstance()->quit();
	}
	else {
		setVisible(false);
	}

}
