/*
  ==============================================================================

    MainComponent.cpp
    Created: 20 May 2011 5:08:02am
    Author:  David Rowland

  ==============================================================================
*/

#include "MainComponent.h"

MainComponent::MainComponent()
{
	settingsButton.setClickingTogglesState(true);
	settingsButton.setButtonText("Settings");
	settingsButton.addListener(this);
		
	addAndMakeVisible(&mainView);
	addChildComponent(&settingsComp);
	addAndMakeVisible(&settingsButton);
}

MainComponent::~MainComponent()
{
}

void MainComponent::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	settingsButton.setBounds(w-100-m, m, 100, 20);
	mainView.setBounds(getLocalBounds());
	settingsComp.setBounds(getLocalBounds());
}

void MainComponent::buttonClicked(Button *button)
{
	if (button == &settingsButton)
	{
		if (settingsButton.getToggleState())
		{
			settingsComp.setVisible(true);
			mainView.setVisible(false);
		}
		else
		{
			mainView.setVisible(true);
			settingsComp.setVisible(false);
		}
	}
}

//==============================================================================
