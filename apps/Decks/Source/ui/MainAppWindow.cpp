/*
 ==============================================================================
 
 JUCE library : Starting point code, v1.26
 Copyright 2005 by Julian Storer. [edited by haydxn, 3rd April 2007]
 
 ------------------------------------------------------------------------------
 
 MainAppWindow.cpp :
 
 This file defines the configuration of the main application window, which
 is the class MainAppWindow.
 
 The JUCE class 'DocumentWindow' is the base for this class, so examine the
 documentation for any functions that you may wish to use, or any other
 configuration options you may wish to change.
 
 The 'window' is the main bit that the program sits within. That means 'the
 bit with the title bar and the overall size/shape of the program, but without
 the actual program on it yet'. Your program lives in the 'MainComponent' class,
 and is added to this window as the 'content component'.
 
 ------------------------------------------------------------------------------
 
 Please feel free to do whatever you like with this code, bearing in mind that
 it's not guaranteed to be bug-free!
 
 ==============================================================================
 */

#include "MainAppWindow.h"

ApplicationCommandManager* commandManager = 0;

//==============================================================================
MainAppWindow::MainAppWindow()
:   
// Initialise the base 'DocumentWindow'...
DocumentWindow (
				ProjectInfo::projectName,		// Set the text to use for the title
				Colours::darkgrey,				// Set the colour of the window
				DocumentWindow::allButtons,		// Set which buttons are displayed
				true							// This window should be added to the desktop
				)
{
	setUpLookAndFeel();
	LookAndFeel::setDefaultLookAndFeel(DecksLookAndFeel::getInstance());

	setResizable (true, false); // resizability is a property of ResizableWindow, which is
	// a parent class of DocumentWindow (which is our base class),
	// so we have access to this setting here.

	setTitleBarHeight (0);      // Set the height of the titlebar on our window.
		
	// create the main component, which is described in MainComponent.h
//    ControlComponent* contentComponent = new ControlComponent ();
	contentComponent = new ContainerComponent ();
	
	commandManager->registerAllCommandsForTarget (this);
	commandManager->registerAllCommandsForTarget (JUCEApplication::getInstance());
	addKeyListener (commandManager->getKeyMappings());
	
    // This sets the main content component for the window to be whatever MainComponent
    // is. The nature of DocumentWindow means that the contentComponent will fill the main
	// area of the window, and will be deleted automatically when the window is deleted.
    setContentComponent (contentComponent);
	
//	contentComponent->setApplicationCommandManagerToWatch (commandManager);
}

MainAppWindow::~MainAppWindow()
{
    // Our 'content component' will get deleted by the destructor in the DialogWindow
	// base class, and that will, in turn (assuming the MainComponent has been coded
	// properly), clean up the other components contained inside it. Therefore, we have
	// nothing much to do here!
	
//	deleteAndZero(contentComponent);
	DBG("MainAppWindow deleted");
}

void MainAppWindow::closeButtonPressed()
{
	// This is a virtual function provided by the DocumentWindow class, allowing us
	// to define the action taken when the window's 'close' button is pressed.
	
    // The correct thing to do when you want the app to quit is to call the
    // JUCEApplication::systemRequestedQuit() method.
	
    // That means that requests to quit that come from your own UI, or from other
    // OS-specific sources (e.g. the dock menu on the mac) all get handled in the
    // same way.
	
	// So, here, we get the application instance (from the static getInstance()
	// function in JUCEApplication), and call the function we need from it...

    JUCEApplication::getInstance()->systemRequestedQuit();
}

const BorderSize MainAppWindow::getBorderThickness()
{
    return BorderSize (isUsingNativeTitleBar() ? 0 : ((resizableBorder != 0 && ! isFullScreen()) ? 0 : 0));
}

void MainAppWindow::showQuitScreen()
{
	contentComponent->showQuitScreen();
}

//==============================================================================
void MainAppWindow::setUpLookAndFeel()
{
	DecksLookAndFeel *laf = DecksLookAndFeel::getInstance();
	laf->setColour(TextButton::buttonColourId, Colours::darkgrey);
	laf->setColour(TextButton::buttonOnColourId, Colours::lightgrey);
	laf->setColour(Slider::rotarySliderFillColourId, Colours::darkgrey);
	laf->setColour(Slider::rotarySliderOutlineColourId, Colour::greyLevel(1));
	laf->setColour(Slider::trackColourId, Colour::greyLevel(0.1));
	laf->setColour(Label::textColourId, Colour::greyLevel(0.4f));	
}

//==============================================================================
// The following methods implement the ApplicationCommandTarget interface, allowing
// this window to publish a set of actions it can perform, and which can be mapped
// onto menus, keypresses, etc.

ApplicationCommandTarget* MainAppWindow::getNextCommandTarget()
{
	// this will return the next parent component that is an ApplicationCommandTarget (in this
	// case, there probably isn't one, but it's best to use this method in your own apps).
	return 0;
}

void MainAppWindow::getAllCommands (Array <CommandID>& commands)
{
	const CommandID ids[] = { CommandIDs::goToKioskMode };
	
    commands.addArray (ids, numElementsInArray (ids));
}

void MainAppWindow::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{	
	switch (commandID)
	{
		case CommandIDs::goToKioskMode:
			result.setInfo ("Fullscreen", "Enters fullscreen mode", CommandCategories::view, 0);
			result.setTicked (Desktop::getInstance().getKioskModeComponent() != 0);
			result.defaultKeypresses.add (KeyPress ('f', ModifierKeys::commandModifier, 0));
			break;
		default:
			break;
			
	};
}

// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool MainAppWindow::perform (const InvocationInfo& info)
{
	DBG("Perform called");
	switch (info.commandID)
	{
		case CommandIDs::goToKioskMode:
			if (Desktop::getInstance().getKioskModeComponent() == 0)
			{
				contentComponent->showTitleBarButtons(true);
				TopLevelWindow::getActiveTopLevelWindow()->setUsingNativeTitleBar(false);
				Desktop::getInstance().setKioskModeComponent (getTopLevelComponent(), false);
			}
			else
			{
				contentComponent->showTitleBarButtons(false);
				Desktop::getInstance().setKioskModeComponent (0);
				TopLevelWindow::getActiveTopLevelWindow()->setUsingNativeTitleBar(true);
			}
			break;
		default:
			return false;
	};
	
	return true;
}