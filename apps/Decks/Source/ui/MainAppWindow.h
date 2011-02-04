/*
 ==============================================================================
 
 JUCE library : Starting point code, v1.26
 Copyright 2005 by Julian Storer. [edited by haydxn, 3rd April 2007]
 
 ------------------------------------------------------------------------------
 
 MainAppWindow.h :
 
 This file is just a declaration of the MainAppWindow class. For more
 detailed information of its purpose, examine the MainAppWindow.cpp
 file.
 
 Having it as a separate header file may seem pointless, but it could
 save a bit of effort should you be embarking on a complex project and
 wish to still use this fileset as a starting point.
 
 ------------------------------------------------------------------------------
 
 Please feel free to do whatever you like with this code, bearing in mind that
 it's not guaranteed to be bug-free!
 
 ==============================================================================
 */

#ifndef _MainAppWindow_H__
#define _MainAppWindow_H__

#include <juce/juce.h>
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../main/CommandIDs.h"
#include "DecksLookAndFeel.h"
#include "ContainerComponent.h"

//==============================================================================
class MainAppWindow  :  public DocumentWindow,
						public ApplicationCommandTarget,
						public DragAndDropContainer
{
public:
	//==============================================================================
	MainAppWindow();
	~MainAppWindow();
	
	//==============================================================================
	// called when the close button is pressed or esc is pushed
	void closeButtonPressed();
	
	const BorderSize<int> getBorderThickness();
		
	// It is most likely that your program will be happy thinking of the window's
	// content component as the 'base' level of the application; it can be responsible
	// for storing and maintaining anything considered crucial to the running of the
	// program.
	
	// However, if you want to have anything live above even that, you may
	// want to put it here. You may even wish to step further outside of things and keep
	// some higher management system within the JUCEApplication class that drives the
	// whole shebang, but that's probably not necessary, and can be tricky to maintain!

	void showQuitScreen();
	
	void lookAndFeelChanged();
	
	//==============================================================================
    ApplicationCommandTarget* getNextCommandTarget();
	
    void getAllCommands (Array <CommandID>& commands);
	
	void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result);
	
    bool perform (const InvocationInfo& info);
	
	//==============================================================================
	
private:
	
	ContainerComponent* contentComponent;
	void setUpLookAndFeel();
};


#endif 