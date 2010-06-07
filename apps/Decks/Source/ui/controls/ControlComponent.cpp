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
	
	tabbedComponent->addTab(T("Library"), Colours::darkgrey, 0/*new TableDemoComponent()*/, true);

	mixerViewport = new Viewport(T("mixerViewport"));
	mixer = new Mixer();
	AudioEngine::getInstance()->setCurrentMixer(mixer);
	mixerViewport->setViewedComponent(mixer);
	tabbedComponent->addTab(T("Mixer"), Colours::darkgrey, mixerViewport, true);
	
	fileBrowser = new ColumnFileBrowser(new WildcardFileFilter(deckManager->getDeck(0)->getFilePlayer()->getAudioFormatManager()->getWildcardForAllFormats(), T("*"), T("Audio Filter")));
	tabbedComponent->addTab(T("Browse"), Colours::darkgrey, fileBrowser, true);
	tabbedComponent->setCurrentTabIndex(1);
		
			
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
	
	mixerViewport->setViewPosition(-(w/2)+(mixerViewport->getViewedComponent()->getWidth()/2), 0);
	
#ifdef JUCE_DEBUG
	cpuMeter->setBounds(w-60, h-20, 60, 20);
#endif
}

void ControlComponent::paint (Graphics& g)
{
	g.setColour(Colours::red);
}
void ControlComponent::paintOverChildren (Graphics& g)
{
	g.setColour(Colours::red);
}
//==============================================================================

void ControlComponent::buttonClicked(Button *button)
{
}

//==============================================================================
// The following methods implement the ApplicationCommandTarget interface, allowing
// this window to publish a set of actions it can perform, and which can be mapped
// onto menus, keypresses, etc.

ApplicationCommandTarget* ControlComponent::getNextCommandTarget()
{
	// this will return the next parent component that is an ApplicationCommandTarget (in this
	// case, there probably isn't one, but it's best to use this method in your own apps).
	return findFirstTargetParentComponent();
}

void ControlComponent::getAllCommands (Array <CommandID>& commands)
{
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = { 
#if ! JUCE_LINUX
//		goToKioskMode
#endif
	};
	
	commands.addArray (ids, numElementsInArray (ids));
}

void ControlComponent::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
	const String generalCategory (T("General"));
	
	switch (commandID)
	{

#if ! JUCE_LINUX
//		case goToKioskMode:
//			result.setInfo (T("Show full-screen kiosk mode"), String::empty, generalCategory, 0);
//			result.setTicked (Desktop::getInstance().getKioskModeComponent() != 0);
//			result.addDefaultKeypress (T('f'), ModifierKeys::commandModifier);
//			break;
#endif
			
		default:
			break;
			
	};
}

// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool ControlComponent::perform (const InvocationInfo& info)
{
	DBG("Perform on ControlComp called");
	switch (info.commandID)
	{
			
#if ! JUCE_LINUX
//		case goToKioskMode:
//			if (Desktop::getInstance().getKioskModeComponent() == 0)
//			{
//				TopLevelWindow::getActiveTopLevelWindow()->setUsingNativeTitleBar(false);
//				Desktop::getInstance().setKioskModeComponent (getTopLevelComponent(), false);
//			}
//			else
//			{
//				TopLevelWindow::getActiveTopLevelWindow()->setUsingNativeTitleBar(true);
//				Desktop::getInstance().setKioskModeComponent (0);
//			}
//			break;
#endif
			
		default:
			return false;
	};
	
	return true;
}	

//==============================================================================
//void ControlComponent::setUpLookAndFeel()
//{
//	DecksLookAndFeel *laf = DecksLookAndFeel::getInstance();
//	laf->setColour(TextButton::buttonColourId, Colours::darkgrey);
//	laf->setColour(TextButton::buttonOnColourId, Colours::lightgrey);
//	laf->setColour(Slider::rotarySliderFillColourId, Colours::darkgrey);
//	laf->setColour(Slider::rotarySliderOutlineColourId, Colour::greyLevel(1));
//	laf->setColour(Slider::trackColourId, Colour::greyLevel(0.1));
//	laf->setColour(Label::textColourId, Colour::greyLevel(0.4f));	
//}