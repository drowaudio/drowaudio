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
	settingsButton->addButtonListener(this);
	
	addAndMakeVisible( controlComponent = new ControlComponent() );
	addChildComponent( settingsComponent = new SettingsComponent() );
	addChildComponent( quitScreen = new QuitScreen() );
}

ContainerComponent::~ContainerComponent ()
{
	settingsButton->removeButtonListener(this);
	
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

//==============================================================================
// The following methods implement the ApplicationCommandTarget interface, allowing
// this window to publish a set of actions it can perform, and which can be mapped
// onto menus, keypresses, etc.

ApplicationCommandTarget* ContainerComponent::getNextCommandTarget()
{
	// this will return the next parent component that is an ApplicationCommandTarget (in this
	// case, there probably isn't one, but it's best to use this method in your own apps).
	return findFirstTargetParentComponent();
}

void ContainerComponent::getAllCommands (Array <CommandID>& commands)
{
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = { 
#if ! JUCE_LINUX
		goToKioskMode
#endif
	};
	
	commands.addArray (ids, numElementsInArray (ids));
}

void ContainerComponent::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
	const String generalCategory (T("General"));
	
	switch (commandID)
	{
			
#if ! JUCE_LINUX
		case goToKioskMode:
			result.setInfo (T("Show full-screen kiosk mode"), String::empty, generalCategory, 0);
			result.setTicked (Desktop::getInstance().getKioskModeComponent() != 0);
			result.addDefaultKeypress (T('f'), ModifierKeys::commandModifier);
			break;
#endif
			
		default:
			break;
			
	};
}

// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool ContainerComponent::perform (const InvocationInfo& info)
{
	DBG("Perform called");
	switch (info.commandID)
	{
			
#if ! JUCE_LINUX
		case goToKioskMode:
			if (Desktop::getInstance().getKioskModeComponent() == 0)
			{
				titlebarComponent->showTiteBarButtons(true);
				TopLevelWindow::getActiveTopLevelWindow()->setUsingNativeTitleBar(false);
				Desktop::getInstance().setKioskModeComponent (getTopLevelComponent(), false);
			}
			else
			{
				titlebarComponent->showTiteBarButtons(false);
				Desktop::getInstance().setKioskModeComponent (0);
				TopLevelWindow::getActiveTopLevelWindow()->setUsingNativeTitleBar(true);
			}
			break;
#endif
			
		default:
			return false;
	};
	
	return true;
}