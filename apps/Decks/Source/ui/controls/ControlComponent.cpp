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
	tabbedComponent->setOutline(0);
	tabbedComponent->setIndent(0);
	
//	tabbedComponent->addTab(T("Library"), Colours::darkgrey, new TableDemoComponent(), true);

	mixerViewport = new CentreAlignViewport(T("mixerViewport"));
	mixerViewport->setScrollBarThickness(10);
//	mixerViewport->getViewedComponent()->setSize(815, 425);
	mixer = new Mixer();
	mixerViewport->setViewedComponent(mixer);
	AudioEngine::getInstance()->setCurrentMixer(mixer);
	tabbedComponent->addTab(T("Mixer"), Colour::greyLevel(0.4), mixerViewport, true);
	tabbedComponent->addTab(T("Library"), Colour::greyLevel(0.4), new Library(), true);
	ColumnFileBrowser *columnFileBrowser = new ColumnFileBrowser(new WildcardFileFilter(DecksAudioFormatManager::getInstance()->getWildcardForAllFormats(), "*", "Audio Filter"));
//	columnFileBrowser->setScrollBarThickness(10);
	tabbedComponent->addTab(T("Files"), Colour::greyLevel(0.4), columnFileBrowser, true);

		
//	fileBrowser = new ColumnFileBrowser(new WildcardFileFilter(deckManager->getDeck(0)->getMainFilePlayer()->getAudioFormatManager()->getWildcardForAllFormats(), T("*"), T("Audio Filter")));
//	tabbedComponent->addTab(T("Browse"), Colours::darkgrey, fileBrowser, true);
//	tabbedComponent->setCurrentTabIndex(1);
		
			
	addAndMakeVisible(cpuMeter = new CpuMeter(AudioEngine::getInstance()->getMainAudioDeviceManager()));
	cpuMeter->setTextColour(Colours::red);
}

ControlComponent::~ControlComponent ()
{
	// need to zero this as the AudioEngine does a null check on it
	AudioEngine::getInstance()->setCurrentMixer(0);

	deleteAllChildren();
	DBG("ControlComponent deleted");
}

//==============================================================================
void ControlComponent::resized ()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
		
	const int transportHeight = transport->getHeight();
	const int waveDisplayHeight = 120;//(meterHeight - margin) / 2;
		
	transport->setBounds(m, m, w-2*m, transportHeight-2*m);
	draggableDisplay->setBounds(m, transport->getBottom()+m, w-2*m, waveDisplayHeight);
	
	tabbedComponent->setBounds(0, draggableDisplay->getBottom(),
							   getWidth(), h - draggableDisplay->getBottom());
	mixerViewport->setViewPosition(0, 0);
	
#ifdef JUCE_DEBUG
	cpuMeter->setBounds(w-60, h-20, 60, 20);
#endif
}

void ControlComponent::paint (Graphics& g)
{
}

//==============================================================================

//==============================================================================