/*
 *  ControlComponent.cpp
 *  resampling_test
 *
 *  Created by David Rowland on 31/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "ControlComponent.h"

ControlComponent::ControlComponent ()
:	deckManager(DeckManager::getInstance())
{	
//	setUpLookAndFeel();
//	lookandFeel = DecksLookAndFeel::getInstance();
//	LookAndFeel::setDefaultLookAndFeel(lookandFeel);
			
	// transport
	addAndMakeVisible( transport = new Transport() );
	addAndMakeVisible( draggableDisplay = new DraggableDisplay() );
	
	// set up the decks
	addAndMakeVisible( tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop) );
	tabbedComponent->setTabBarDepth(25);
	
//	tabbedComponent->addTab(T("Library"), Colours::darkgrey, new TableDemoComponent(), true);

//	mixerViewport = new Viewport(T("mixerViewport"));
//	mixer = new Mixer();
//	AudioEngine::getInstance()->setCurrentMixer(mixer);
//	mixerViewport->setViewedComponent(new Mixer());
//	mixerViewport->getViewedComponent()->setSize(815, 425);
	tabbedComponent->addTab(T("Mixer"), Colours::darkgrey, new Mixer()/*mixerViewport*/, true);
	
//	fileBrowser = new ColumnFileBrowser(new WildcardFileFilter(deckManager->getDeck(0)->getMainFilePlayer()->getAudioFormatManager()->getWildcardForAllFormats(), T("*"), T("Audio Filter")));
//	tabbedComponent->addTab(T("Browse"), Colours::darkgrey, fileBrowser, true);
//	tabbedComponent->setCurrentTabIndex(1);
		
			
	addAndMakeVisible(cpuMeter = new CpuMeter(AudioEngine::getInstance()->getMainAudioDeviceManager()));
	cpuMeter->setTextColour(Colours::red);
}

ControlComponent::~ControlComponent ()
{
	deleteAllChildren();
}

//==============================================================================
void ControlComponent::resized ()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
		
	const int transportHeight = transport->getHeight();
	const int waveDisplayHeight = 150;//(meterHeight - margin) / 2;
		
	transport->setBounds(m, m, w-2*m, transportHeight-2*m);
	draggableDisplay->setBounds(m, transport->getBottom()+m, w-2*m, waveDisplayHeight);
	
	tabbedComponent->setBounds(2, draggableDisplay->getBottom(),
							   getWidth()-4, h - draggableDisplay->getBottom() - 2);
//	mixerViewport->setViewPosition((-(w/2)+(mixerViewport->getViewedComponent()->getWidth()*0.5)), 0);
//	mixerViewport->setViewPosition(100, 0);
	
#ifdef JUCE_DEBUG
	cpuMeter->setBounds(w-60, h-20, 60, 20);
#endif
}

void ControlComponent::paint (Graphics& g)
{
}
//==============================================================================


//==============================================================================