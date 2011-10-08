/*
 ==============================================================================
 
 JUCE library : Starting point code, v1.26
 Copyright 2005 by Julian Storer. [edited by haydxn, 3rd April 2007]
 
 ==============================================================================
 */

#ifndef _DECKS_CONTROLCOMPONENT__H_
#define _DECKS_CONTROLCOMPONENT__H_

#include "../JuceLibraryCode/JuceHeader.h"

#include "ControlComponent.h"
//#include "TableDemo.h"
//#include "mixer/MixerComponent.h"
//#include "DeckComponent.h"
//#include "TrackInfo.h"
#include "mixer/Mixer.h"
#include "transport/Transport.h"
#include "draggable_display/DraggableDisplay.h"
#include "../../main/DecksAudioFormatManager.h"
#include "../../main/DeckManager.h"
#include "../../main/AudioEngine.h"
#include "../DecksLookAndFeel.h"
#include "library/Library.h"

class ControlComponent  : public Component
{
private:
		
	TooltipWindow tooltipWindow;
	DeckManager *deckManager;
	
	
	Transport *transport;
	DraggableDisplay *draggableDisplay;
		
	TabbedComponent* tabbedComponent;
	Mixer *mixer;
	CentreAlignViewport *mixerViewport;
	
//	ColumnFileBrowser* fileBrowser;
		
	double currentSampleRate;
	
	CpuMeter* cpuMeter;
	
public:
	//==============================================================================
	ControlComponent ();
	
	~ControlComponent ();
	
	//==============================================================================
	void resized ();
	
	void paint (Graphics& g);
		
	//==============================================================================
	
	//==============================================================================

};

#endif//_DECKS_CONTROLCOMPONENT__H_ 