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
	compressAudioButton.setTooltip("Checking this will perform basic non-lossy compression on the audio that is transferred. "
								   "This will reduce network load but increase CPU usage.");
	
//	audioSettings = new AudioDeviceSelectorComponent(deviceManager,
//													 1, 2,
//													 1, 2,
//													 false,
//													 false,
//													 true,
//													 false);
}

SettingsComponent::~SettingsComponent()
{
}

void SettingsComponent::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	const int cx = w * 0.5;
	
	compressAudioButton.setBounds(cx-125, 40+2*m, 250, 20);
}

void SettingsComponent::paint(Graphics &g)
{
	const int m = 5;
	
	g.fillAll(Colours::azure);
	
	const int fontH = 20;
	g.setColour(Colours::black);
	g.drawText("Compression Settings:",
			   m, compressAudioButton.getY() - fontH - m,
			   Font(fontH).getStringWidth("Compression Settings:"), fontH, Justification(Justification::centredLeft), true);
	g.drawText("Audio Settings:",
			   m, compressAudioButton.getBottom()+m,
			   Font(fontH).getStringWidth("Audio Settings:"), fontH, Justification(Justification::centredLeft), true);
}
