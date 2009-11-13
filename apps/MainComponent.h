/*
 ==============================================================================
 
 JUCE library : Starting point code, v1.26
 Copyright 2005 by Julian Storer. [edited by haydxn, 3rd April 2007]
 
 ==============================================================================
 */

#ifndef _MAINCOMPONENT_H_
#define _MAINCOMPONENT_H_

#include <juce/juce.h>
#include "MixerComponent.h"
#include "DeckComponent.h"
#include "TrackInfo.h"
#include "AudioFileBrowser.h"

class MainComponent  : public Component,
					   public AudioIODeviceCallback,
					   public SliderListener,
					   public ButtonListener,
					   public ApplicationCommandTarget
{
private:
	
	enum CommandIDs
    {
        goToKioskMode              = 0x200f
    };
	
	TooltipWindow tooltipWindow;
	dRowLookAndFeel* lookandFeel;

	// classes for audio playback
	AudioDeviceManager audioDeviceManager;
	AudioSourcePlayer audioSourcePlayer;
	MixerAudioSource mixerSource;
	
	StereoMeter* masterMeter;
	float peakLeft, peakRight;
	
	TabbedComponent* tabbedComponent;
	MixerComponent* mixerComponent;
	ColumnFileBrowser* fileBrowser;
	DeckComponent *deck1, *deck2;
	TrackInfo *leftInfoPane, *rightInfoPane;
	
	// controls
	Slider* zoomSlider;
	Slider* playheadPosSlider;
	
	TextButton *settingsButton;
	
	double currentSampleRate;
	float zoomFactor;
					
	PositionableWaveDisplay *deck1PosWaveDisplay, *deck2PosWaveDisplay;
	DraggableWaveDisplay *deck1DragWaveDisplay, *deck2DragWaveDisplay;
	
	CpuMeter* cpuMeter;
	
public:
	//==============================================================================
	MainComponent ();
	
	~MainComponent ();
	
	//==============================================================================
	void resized ();
	
	void paint (Graphics& g);
	
	void paintOverChildren (Graphics& g);
	
	//==============================================================================
	void sliderValueChanged(Slider *slider);
	
	void buttonClicked(Button *button);
	
	//==============================================================================
    ApplicationCommandTarget* getNextCommandTarget();
	
    void getAllCommands (Array <CommandID>& commands);
	
	void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result);
	
    bool perform (const InvocationInfo& info);
	
	//==============================================================================
	void audioDeviceIOCallback (const float** inputChannelData,
								int totalNumInputChannels,
								float** outputChannelData,
								int totalNumOutputChannels,
								int numSamples);
	
	void audioDeviceAboutToStart (AudioIODevice* device);	
	
	void audioDeviceStopped();
	
	//==============================================================================
	MixerComponent* getMixerComponent()		{ return mixerComponent;	}

};

#endif//_MAINCOMPONENT_H_ 