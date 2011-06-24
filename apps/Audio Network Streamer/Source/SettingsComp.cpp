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
	addAndMakeVisible(&compressAudioComboBox);
	for (int i = 1; i <= (Settings::numCompressionLevels); i++)
	{
		compressAudioComboBox.addItem(CompressionLevelNames[i-1], i);
	}
	compressAudioComboBox.getSelectedIdAsValue().referTo(settingsTree.getPropertyAsValue(SettingsNames[Settings::compress], nullptr));
	compressAudioComboBox.setTooltip("Checking this will perform basic non-lossy compression on the audio that is transferred. "
									 "This will reduce network load but increase CPU usage.");
	
	audioSettings = new AudioSettingsComponent(*Settings::getInstance()->audioManager,
											   1, 2,
											   1, 2,
											   false,
											   false,
											   true,
											   false);
	addAndMakeVisible(audioSettings);
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
	
	compressAudioComboBox.setBounds(cx-125, 40+2*m, 250, 20);
	audioSettings->setBounds(m, compressAudioComboBox.getBottom()+20+2*m, w-2*m, audioSettings->getHeight());
}

void SettingsComponent::paint(Graphics &g)
{
	const int m = 5;
	
	const int fontH = 20;
	g.setColour(Colours::black);
	g.drawText("Compression Settings:",
			   m, compressAudioComboBox.getY() - fontH - m,
			   Font(fontH).getStringWidth("Compression Settings:"), fontH, Justification(Justification::centredLeft), true);
	g.drawText("Audio Settings:",
			   m, compressAudioComboBox.getBottom()+m,
			   Font(fontH).getStringWidth("Audio Settings:"), fontH, Justification(Justification::centredLeft), true);
	
	g.setColour(Colours::azure.darker(0.1));
	Rectangle<int> audioBounds(audioSettings->getBounds());
	g.fillRoundedRectangle(audioBounds.getX(), audioBounds.getY(), audioBounds.getWidth(), audioSettings->getHeight(), 5);
}