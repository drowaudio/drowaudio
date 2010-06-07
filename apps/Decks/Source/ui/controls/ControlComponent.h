/*
 ==============================================================================
 
 JUCE library : Starting point code, v1.26
 Copyright 2005 by Julian Storer. [edited by haydxn, 3rd April 2007]
 
 ==============================================================================
 */

#ifndef _DECKS_CONTROLCOMPONENT__H_
#define _DECKS_CONTROLCOMPONENT__H_

#include <juce/juce.h>
#include "ControlComponent.h"
//#include "TableDemo.h"
//#include "mixer/MixerComponent.h"
//#include "DeckComponent.h"
//#include "TrackInfo.h"
#include "mixer/Mixer.h"
#include "transport/Transport.h"
#include "draggable_display/DraggableDisplay.h"
#include "../../main/DeckManager.h"
#include "../../main/AudioEngine.h"
#include "../DecksLookAndFeel.h"

class ControlComponent  : public Component,
					   public ButtonListener,
					   public ApplicationCommandTarget
{
private:
	
//	enum CommandIDs
//    {
//        goToKioskMode              = 0x200f
//    };
	
	TooltipWindow tooltipWindow;
	DeckManager *deckManager;
	
	
	Transport *transport;
	DraggableDisplay *draggableDisplay;
		
	TabbedComponent* tabbedComponent;
	Viewport *mixerViewport;
	Mixer *mixer;
	ColumnFileBrowser* fileBrowser;
		
	double currentSampleRate;
	
	CpuMeter* cpuMeter;
	
public:
	//==============================================================================
	ControlComponent ();
	
	~ControlComponent ();
	
	//==============================================================================
	void resized ();
	
	void paint (Graphics& g);
	
	void paintOverChildren (Graphics& g);
	
	//==============================================================================
	void buttonClicked(Button *button);
	
	//==============================================================================
    ApplicationCommandTarget* getNextCommandTarget();
	
    void getAllCommands (Array <CommandID>& commands);
	
	void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result);
	
    bool perform (const InvocationInfo& info);
	
	//==============================================================================
//	void setUpLookAndFeel();

};

#endif//_DECKS_CONTROLCOMPONENT__H_ 