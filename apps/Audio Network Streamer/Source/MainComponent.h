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
#include "MainView.h"

/**	@todo	Improve UI
	@todo	Keep settings in a more convenient place
	@todo	Sleep prevention when disconnected (possibly add and remove audio callback on connection)
	@todo	Make buffersize slider and label as latency
	@todo	Sample rate conversion if needed
	@todo	Reciever - adjustable buffersize with overrun if size limit reached
	@todo	Sending - use buffer to add samples to which will send them one a certain number has been reached
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
	
	TextButton settingsButton;
	MainView mainView;
	SettingsComponent settingsComp;
};

#endif  // __MAINCOMPONENT_H_CCF3F3A9__
