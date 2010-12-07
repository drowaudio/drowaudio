/*
  ==============================================================================

    MixerSettingsComponent.h
    Created: 6 Jun 2010 3:03:24am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MIXERSETTINGSCOMPONENT_H_E5B212E2__
#define __MIXERSETTINGSCOMPONENT_H_E5B212E2__

#include <juce/juce.h>
#include "/Source/main/DeckManager.h"
#include "/Source/main/AudioEngine.h"
#include "/Source/main/Settings.h"

class MixerSettingsComponent  :	public Component
{
public:
	//==============================================================================
	MixerSettingsComponent ()
	{
		MixerSettingsComponent::getInstance();
	}
	
	~MixerSettingsComponent ()
	{
		DBG("MixerSettingComponent deleted");
	}
	
	//==============================================================================
	void resized ();
	
	void paint (Graphics& g);
	
	//==============================================================================
	//==============================================================================
	
private:
};

#endif  // __MIXERSETTINGSCOMPONENT_H_E5B212E2__
