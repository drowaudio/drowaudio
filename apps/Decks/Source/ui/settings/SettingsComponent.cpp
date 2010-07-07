/*
 *  SettingsComponent.cpp
 *  Decks
 *
 *  Created by David Rowland on 18/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "SettingsComponent.h"

SettingsComponent::SettingsComponent ()
{
//	addAndMakeVisible(settingsSelector = new TabbedComponent(TabbedButtonBar::TabsAtTop));
//	settingsSelector->addTab("Audio Device 1",
//							 Colours::darkgrey,
//							 new AudioDeviceSelectorComponent(*(AudioEngine::getInstance()->getMainAudioDeviceManager()),
//																								   1, 2, 1, 2,
//																								   true, true,
//																								   true, false),
//							 false);
//	settingsSelector->addTab("Audio Device 2",
//							 Colours::darkgrey,
//							 new AudioDeviceSelectorComponent(*(AudioEngine::getInstance()->getMonitorAudioDeviceManager()),
//															  0, 2, 1, 2,
//															  false, false,
//															  true, false),
//							 false);
	
//	addAndMakeVisible(mainAudioSettings = new AudioDeviceSelectorComponent(*(AudioEngine::getInstance()->getMainAudioDeviceManager()),
//																		   1, 2, 1, 2,
//																		   true, true,
//																		   true, false));
//	addAndMakeVisible(monitorAudioSettings = new AudioDeviceSelectorComponent(*(AudioEngine::getInstance()->getMonitorAudioDeviceManager()),
//																			  1, 2, 1, 2,
//																			  true, true,
//																			  true, false));

	getLookAndFeel().setColour(Label::textColourId, Colour::greyLevel(0.9));
}

SettingsComponent::~SettingsComponent ()
{
	deleteAllChildren();
}

//==============================================================================
void SettingsComponent::resized ()
{
//	settingsSelector->setBounds(0, 0, getWidth(), getHeight());
	
//	mainAudioSettings->setBounds(0, 20, getWidth(), (getHeight()-20)*0.5);
//	monitorAudioSettings->setBounds(0, mainAudioSettings->getBottom(), getWidth(), (getHeight()-20)*0.5);
}

void SettingsComponent::paint (Graphics& g)
{
//	g.setColour(Colours::red);
//	g.fillAll();
}

//==============================================================================
