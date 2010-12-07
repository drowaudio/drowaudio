/*
 *  ContainerComponent.h
 *  Decks
 *
 *  Created by David Rowland on 18/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKS_CONTAINERCOMPONENT__H_
#define _DECKS_CONTAINERCOMPONENT__H_

#include <juce/juce.h>
#include "../main/DeckManager.h"
//#include "DecksLookAndFeel.h"
#include "TitleBarComponent.h"
#include "QuitScreen.h"
#include "controls/ControlComponent.h"
#include "settings/SettingsComponent.h"

class ContainerComponent  :	public Component,
							public ButtonListener,
							public ApplicationCommandTarget
{
public:
	//==============================================================================
	enum CommandIDs
    {
        goToKioskMode              = 0x200f
    };
	
	//==============================================================================
	ContainerComponent ();
	
	~ContainerComponent ();
	
	//==============================================================================
	void resized ();
	
	void paint (Graphics& g);
	
	void showQuitScreen();
	
	//==============================================================================
	void buttonClicked(Button *button);
	
	//==============================================================================
    ApplicationCommandTarget* getNextCommandTarget();
	
    void getAllCommands (Array <CommandID>& commands);
	
	void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result);
	
    bool perform (const InvocationInfo& info);
	
	//==============================================================================
	
private:
	TextButton *settingsButton;
	
	TitleBarComponent *titlebarComponent;
	QuitScreen *quitScreen;
	ControlComponent *controlComponent;
	SettingsComponent *settingsComponent;
};

#endif //_DECKS_CONTAINERCOMPONENT__H_