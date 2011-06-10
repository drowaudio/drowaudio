/*
  ==============================================================================

    MainComponent.cpp
    Created: 20 May 2011 5:08:02am
    Author:  David Rowland

  ==============================================================================
*/

#include "MainComponent.h"
#include "AudioNetworkStreamer.h"

MainComponent::MainComponent()
:	tabbedComponent(TabbedButtonBar::TabsAtTop)
{
	addAndMakeVisible(&tabbedComponent);
//	ClientComponent *client = new ClientComponent();
//	tabbedComponent.addTab("Server", Colours::azure, new ServerComponent(), true);
//	tabbedComponent.addTab("Client", Colours::azure, client, true);
	tabbedComponent.addTab("Demo", Colours::azure, new InterprocessCommsDemo(), true);
}

MainComponent::~MainComponent()
{
}

void MainComponent::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	tabbedComponent.setBounds(0, 0, w, h);
}

//==============================================================================
