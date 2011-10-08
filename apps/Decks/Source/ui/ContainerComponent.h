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

#include "../JuceLibraryCode/JuceHeader.h"
#include "../main/DeckManager.h"
//#include "DecksLookAndFeel.h"
#include "TitleBarComponent.h"
#include "QuitScreen.h"
#include "controls/ControlComponent.h"
#include "settings/SettingsComponent.h"

class ContainerComponent  :	public Component,
							public ButtonListener
{
public:
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
	void showTitleBarButtons(bool shouldShowButtons);
	
	//==============================================================================
	
private:
	TextButton *settingsButton;
	
	TitleBarComponent *titlebarComponent;
	QuitScreen *quitScreen;
	ControlComponent *controlComponent;
	SettingsComponent *settingsComponent;
};

#endif //_DECKS_CONTAINERCOMPONENT__H_