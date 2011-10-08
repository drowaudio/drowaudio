/*
  ==============================================================================

    MixerSettingsComponent.h
    Created: 6 Jun 2010 3:03:24am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MIXERSETTINGSCOMPONENT_H_E5B212E2__
#define __MIXERSETTINGSCOMPONENT_H_E5B212E2__

#include "../JuceLibraryCode/JuceHeader.h"


class MixerSettingsComponent  :	public Component
{
public:
	//==============================================================================
	MixerSettingsComponent();
	
	~MixerSettingsComponent();
	
	//==============================================================================
	void resized();
	
	void paint(Graphics& g);
	
	//==============================================================================
	//==============================================================================
	
private:
};

#endif  // __MIXERSETTINGSCOMPONENT_H_E5B212E2__
