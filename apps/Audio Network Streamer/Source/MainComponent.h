/*
  ==============================================================================

    MainComponent.h
    Created: 20 May 2011 5:08:02am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_CCF3F3A9__
#define __MAINCOMPONENT_H_CCF3F3A9__

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioNetworkStreamer.h"
#include "SettingsComp.h"

/**	@todo	Improve UI
	@todo	Keep settings in a more convenient place
	@todo	Repitition of buffer in listener mode
	@todo	Sleep prevention when disconnected (possibly add and remove audio callback on connection)
	@todo	Adjustable buffersize with overrun if size limit reached
	@todo	Change samples to int (or short) for better compression
	@todo	Re-use memory blocks to avoid malloc on the audio thread
 */

class MainComponent :	public Component,
						public Button::Listener
{
public:
	MainComponent();
	
	~MainComponent();
	
	void resized();
	
	void buttonClicked(Button *button);
	
private:
	
	TabbedComponent tabbedComponent;
	
	TextButton settingsButton;
	InterprocessCommsDemo connectionComp;
	SettingsComponent settingsComp;
};

#endif  // __MAINCOMPONENT_H_CCF3F3A9__
