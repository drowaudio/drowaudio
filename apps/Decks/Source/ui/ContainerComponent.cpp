/*
 *  ContainerComponent.cpp
 *  Decks
 *
 *  Created by David Rowland on 18/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "ContainerComponent.h"

ContainerComponent::ContainerComponent ()
{
	addAndMakeVisible( titlebarComponent = new TitleBarComponent() );
	settingsButton = titlebarComponent->getSettingsButton();
	settingsButton->addListener(this);
	
	addAndMakeVisible( controlComponent = new ControlComponent() );
	addChildComponent( settingsComponent = new SettingsComponent() );
	addChildComponent( quitScreen = new QuitScreen() );
}

ContainerComponent::~ContainerComponent ()
{
	settingsButton->removeListener(this);
	
	deleteAllChildren();
	DBG("ContainerComponent deleted");
}

//==============================================================================
void ContainerComponent::resized ()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 2;
	
	titlebarComponent->setBounds(0, 0, w, 30);
	
	quitScreen->setBounds(0, titlebarComponent->getBottom(), w, h - titlebarComponent->getHeight());
	controlComponent->setBounds(0, titlebarComponent->getBottom(), w, h - titlebarComponent->getHeight());
	settingsComponent->setBounds(0, titlebarComponent->getBottom(), w, h - titlebarComponent->getHeight());
	
}

void ContainerComponent::paint (Graphics& g){}

void ContainerComponent::showQuitScreen()
{
	quitScreen->setVisible(true);
}

//==============================================================================
void ContainerComponent::buttonClicked(Button *button)
{
	if (button == settingsButton) {
		if (button->getToggleState()) {
			DBG("set control");
			settingsComponent->setVisible(true);
			controlComponent->setVisible(false);
		}
		else {
			DBG("set settings");
			controlComponent->setVisible(true);
			settingsComponent->setVisible(false);
		}
	}
}

void ContainerComponent::showTitleBarButtons(bool shouldShowButtons)
{
	titlebarComponent->showTiteBarButtons(shouldShowButtons);
}
