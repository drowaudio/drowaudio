/*
  ==============================================================================

    SettingsComp.h
    Created: 11 Jun 2011 12:37:21am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __SETTINGSCOMP_H_34551A63__
#define __SETTINGSCOMP_H_34551A63__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "AudioSettingsComponent.h"

class SettingsComponent	:	public Component,
							public ChangeListener,
							public AsyncUpdater
{
public:
	SettingsComponent();
	
	~SettingsComponent();
	
	void resized();
	
	void paint(Graphics &g);
	
	void changeListenerCallback(ChangeBroadcaster *changedBroadcaster);
	
	void handleAsyncUpdate();
	
	void childBoundsChanged(Component *child);
	
private:
	
	TooltipWindow tooltipWindo;
	ValueTree settingsTree;
	ToggleButton compressAudioButton;
	ScopedPointer<AudioSettingsComponent> audioSettings; 
};

#endif  // __SETTINGSCOMP_H_34551A63__
