/*
 *  SettingsComponent.h
 *  Decks
 *
 *  Created by David Rowland on 18/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKS_SETTINGSCOMPONENT__H_
#define _DECKS_SETTINGSCOMPONENT__H_

#include <juce/juce.h>
#include "../../main/DeckManager.h"
#include "../../main/AudioEngine.h"

class SettingsComponent  :	public Component
{
public:
	//==============================================================================
	SettingsComponent ();
	
	~SettingsComponent ();
	
	//==============================================================================
	void resized ();
	
	void paint (Graphics& g);
	
	//==============================================================================
	
private:
	TabbedComponent *settingsSelector;
	
	AudioDeviceSelectorComponent *mainAudioSettings, *monitorAudioSettings;
};

#endif //_DECKS_SETTINGSCOMPONENT__H_