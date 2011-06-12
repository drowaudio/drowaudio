/*
  ==============================================================================

    MainComponent.cpp
    Created: 20 May 2011 5:08:02am
    Author:  David Rowland

  ==============================================================================
*/

#include "MainComponent.h"

MainComponent::MainComponent()
:	tabbedComponent(TabbedButtonBar::TabsAtTop)
{
	addAndMakeVisible(&settingsButton);
	settingsButton.setClickingTogglesState(true);
	settingsButton.setButtonText("Settings");
	settingsButton.addListener(this);
	
//	addAndMakeVisible(&tabbedComponent);
//	ClientComponent *client = new ClientComponent();
//	tabbedComponent.addTab("Server", Colours::azure, new ServerComponent(), true);
//	tabbedComponent.addTab("Client", Colours::azure, client, true);
//	tabbedComponent.addTab("Demo", Colours::azure, new InterprocessCommsDemo(), true);
	
	addAndMakeVisible(&connectionComp);
	addChildComponent(&settingsComp);
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
	//tabbedComponent.setBounds(0, settingsButton.getBottom()+m, w, h-settingsButton.getHeight()-m);
	connectionComp.setBounds(0, settingsButton.getBottom()+m, w, h-settingsButton.getHeight()-m);
	settingsComp.setBounds(0, settingsButton.getBottom()+m, w, h-settingsButton.getHeight()-m);
}

void MainComponent::buttonClicked(Button *button)
{
	if (button == &settingsButton)
	{
		if (settingsButton.getToggleState())
		{
			settingsComp.setVisible(true);
			connectionComp.setVisible(false);
		}
		else
		{
			connectionComp.setVisible(true);
			settingsComp.setVisible(false);
		}

	}
}

//==============================================================================
