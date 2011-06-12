/*
  ==============================================================================

    SettingsComp.cpp
    Created: 11 Jun 2011 12:37:21am
    Author:  David Rowland

  ==============================================================================
*/

#include "SettingsComp.h"

SettingsComponent::SettingsComponent()
:	settingsTree(Settings::getInstance()->getValueTree())
{
	addAndMakeVisible(&compressAudioButton);
	compressAudioButton.setButtonText("Compress Transfer When Sending");
	compressAudioButton.getToggleStateValue().referTo(settingsTree.getPropertyAsValue(SettingsNames[Settings::compress], nullptr));
}

SettingsComponent::~SettingsComponent()
{
}

void SettingsComponent::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	compressAudioButton.setBounds(m, m, w-2*m, 20);
}

void SettingsComponent::paint(Graphics &g)
{
	g.fillAll(Colours::azure);
}
