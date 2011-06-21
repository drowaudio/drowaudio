/*
  ==============================================================================

    MainView.cpp
    Created: 12 Jun 2011 6:06:42am
    Author:  David Rowland

  ==============================================================================
*/

#include "MainView.h"

//==============================================================================
MainView::MainView()
:	settingsTree(Settings::getInstance()->getValueTree())
{
	addAndMakeVisible(&status);
	
	// create all our UI bits and pieces..
	addAndMakeVisible (&modeSelector);
	labels.add(new Label("Mode:", "Mode:"));
	labels.getLast()->attachToComponent (&modeSelector, true);
	
	modeSelector.addItem ("Disconnected", 1);
	modeSelector.addSeparator();
	modeSelector.addItem ("Reciever", 2);
	modeSelector.addItem ("Sender", 3);
	
	addAndMakeVisible (&socketNumber);
	socketNumber.setMultiLine (false);
	socketNumber.setInputRestrictions (5, "0123456789");
	labels.add(new Label("Port:", "Port:"));
	labels.getLast()->attachToComponent (&socketNumber, true);
	
	addAndMakeVisible (&socketHost);
	socketHost.setMultiLine (false);
//	socketNumber.setInputRestrictions (512);
	labels.add(new Label("Destination IP:", "Destination IP:"));
	labels.getLast()->attachToComponent (&socketHost, true);
	
	// bind UI to tree properties
	settingsTree.addListener(this);
	modeSelector.getSelectedIdAsValue().referTo(settingsTree.getPropertyAsValue(SettingsNames[Settings::mode], nullptr));
	socketHost.getTextValue().referTo(settingsTree.getPropertyAsValue(SettingsNames[Settings::host], nullptr));
	socketNumber.getTextValue().referTo(settingsTree.getPropertyAsValue(SettingsNames[Settings::port], nullptr));
}

MainView::~MainView()
{
	settingsTree.removeListener(this);
}

void MainView::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	const int cx = w * 0.5;
	
	status.setBounds(m, m, 200, 20);
	
	modeSelector.setBounds (cx-75, 20+4*m, 150, 20);
	socketNumber.setBounds (cx - 75, modeSelector.getBottom()+2*m, 150, 20);
	socketHost.setBounds (cx - 75, socketNumber.getBottom()+2*m, 150, 20);
}

//==============================================================================
void MainView::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
	DBG("MainView: "<<property.toString());

//	if (property == SettingsNames[Settings::status])
//	{
//		DBG("status changed");
//		status->setStatus(static_cast<StatusComponent::Status>(int(settingsTree[SettingsNames[Settings::status]])));
//	}
}