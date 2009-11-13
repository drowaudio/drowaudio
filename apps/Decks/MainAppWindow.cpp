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
#include "MainComponent.h"

//==============================================================================
MainAppWindow::MainAppWindow()
:   
// Initialise the base 'DocumentWindow'...
DocumentWindow (
				T(PROJECT_NAME),				// Set the text to use for the title
				Colours::darkgrey,				// Set the colour of the window
				DocumentWindow::allButtons,		// Set which buttons are displayed
				true							// This window should be added to the desktop
				)
{
    setResizable (true, false); // resizability is a property of ResizableWindow, which is
	// a parent class of DocumentWindow (which is our base class),
	// so we have access to this setting here.

	setTitleBarHeight (20);      // Set the height of the titlebar on our window.
	
	// create the main component, which is described in MainComponent.h
    MainComponent* contentComponent = new MainComponent ();
	
	commandManager = new ApplicationCommandManager();
	commandManager->registerAllCommandsForTarget (contentComponent);
	commandManager->registerAllCommandsForTarget (JUCEApplication::getInstance());
	commandManager->registerAllCommandsForTarget (contentComponent->getMixerComponent());
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