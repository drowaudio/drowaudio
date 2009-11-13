/*
 *  MainComponent.cpp
 *  resampling_test
 *
 *  Created by David Rowland on 31/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "MainComponent.h"

MainComponent::MainComponent ()
{	
	lookandFeel = new dRowLookAndFeel();
	setLookAndFeel(lookandFeel);
	lookandFeel->setColour(TextButton::buttonColourId, Colours::lightgrey);
	lookandFeel->setColour(Slider::thumbColourId, Colours::lightgrey);
	
	// set up the audio
	audioDeviceManager.initialise (1, /* number of input channels */
								   2, /* number of output channels */
								   0, /* no XML settings.. */
								   true  /* select default device on failure */);
	audioDeviceManager.addAudioCallback (this);
	currentSampleRate = (audioDeviceManager.getCurrentAudioDevice())->getCurrentSampleRate();
	
	// set up the decks
	addAndMakeVisible( tabbedComponent = new TabbedComponent(TabbedButtonBar::TabsAtTop) );
	
	mixerComponent = new MixerComponent();
	tabbedComponent->addTab(T("Mixer"), Colours::darkgrey, mixerComponent, true);
	
	deck1 = ((MixerComponent*)tabbedComponent->getTabContentComponent(tabbedComponent->getTabNames().indexOf(T("Mixer"))))->getDeck1();
	deck2 = ((MixerComponent*)tabbedComponent->getTabContentComponent(tabbedComponent->getTabNames().indexOf(T("Mixer"))))->getDeck2();		

	fileBrowser = new ColumnFileBrowser(new WildcardFileFilter(deck1->getAudioFilePlayer()->getAudioFormatManager()->getWildcardForAllFormats(), T("Audio Filter")));
	tabbedComponent->addTab(T("Browse"), Colours::darkgrey, fileBrowser, true);

	addAndMakeVisible( leftInfoPane = new TrackInfo(deck1->getAudioFilePlayer()) );
	addAndMakeVisible( rightInfoPane = new TrackInfo(deck2->getAudioFilePlayer()) );
	
	addAndMakeVisible( deck1DragWaveDisplay = new DraggableWaveDisplay(deck1->getAudioFilePlayer(), currentSampleRate) );
	addAndMakeVisible( deck1PosWaveDisplay = new PositionableWaveDisplay(deck1->getAudioFilePlayer(), currentSampleRate) );
	
	addAndMakeVisible( deck2DragWaveDisplay = new DraggableWaveDisplay(deck2->getAudioFilePlayer(), currentSampleRate) );
	addAndMakeVisible( deck2PosWaveDisplay = new PositionableWaveDisplay(deck2->getAudioFilePlayer(), currentSampleRate) );
	
	addAndMakeVisible(settingsButton = new TextButton("Settings"));
	settingsButton->addButtonListener(this);
	
	mixerSource.addInputSource (deck1->getAudioFilePlayer(), false);
	mixerSource.addInputSource (deck2->getAudioFilePlayer(), false);
	audioSourcePlayer.setSource (&mixerSource);
	
	// create the output meter
	addAndMakeVisible( masterMeter = new StereoMeter(&peakLeft, &peakLeft, &peakRight, &peakRight) );
	
	// create the zoom slider
	addAndMakeVisible( zoomSlider = new Slider("zoomSlider") );
	zoomSlider->setSliderStyle(Slider::LinearVertical);
	zoomSlider->setRange(0.0, 200.0, 0.0001);
	zoomSlider->setSkewFactorFromMidPoint(10);
	zoomSlider->setValue(10);
	zoomSlider->addListener(this);
	
	addAndMakeVisible( playheadPosSlider = new Slider("playheadPosSlider") );
	playheadPosSlider->setSliderStyle(Slider::LinearVertical);
	playheadPosSlider->setRange(0.0, 1.0, 0.0001);
	playheadPosSlider->setValue(0.5);
	playheadPosSlider->addListener(this);
	
	addAndMakeVisible(cpuMeter = new CpuMeter(&audioDeviceManager));
	cpuMeter->setTextColour(Colours::red);
}

MainComponent::~MainComponent ()
{
	// zero the device manager and source player to avoid crashing
	audioDeviceManager.removeAudioCallback (this);
	audioSourcePlayer.setSource (0);
	
	deleteAllChildren();
	deleteAndZero(lookandFeel);
}

//==============================================================================
void MainComponent::resized ()
{
	const int currentWidth = getWidth();
	const int currentHeight = getHeight();
	
	const int waveDisplayWidth = (currentWidth*0.5) - 6 - 22;
	const int waveDisplayHeight = 50;
	
	masterMeter->setBounds(currentWidth*0.5f - 20, 5, 40, (currentHeight-110)*0.25);
	
	const int posWaveHeight = masterMeter->getHeight()*0.5;
	
	leftInfoPane->setBounds(5, 5, waveDisplayWidth, posWaveHeight-5);
	rightInfoPane->setBounds(masterMeter->getRight()+2, 5, waveDisplayWidth, posWaveHeight-5);
	
	deck1PosWaveDisplay->setBounds(5, 5+posWaveHeight,
								   waveDisplayWidth, posWaveHeight);
	deck1DragWaveDisplay->setBounds(25, deck1PosWaveDisplay->getBottom()+5,
									currentWidth-50, waveDisplayHeight);
	
	deck2PosWaveDisplay->setBounds(masterMeter->getRight()+2, 5+posWaveHeight,
								   waveDisplayWidth, posWaveHeight);
	deck2DragWaveDisplay->setBounds(25, deck1DragWaveDisplay->getBottom()+5,
									currentWidth-50, waveDisplayHeight);
	
	
	// set component bounds
	zoomSlider->setBounds(5, deck1DragWaveDisplay->getY(),
						  20, deck2DragWaveDisplay->getBottom() - deck1DragWaveDisplay->getY());
	playheadPosSlider->setBounds(deck2DragWaveDisplay->getRight(), deck1DragWaveDisplay->getY(),
								 20, deck2DragWaveDisplay->getBottom() - deck1DragWaveDisplay->getY());		
	
	tabbedComponent->setBounds(2, deck2DragWaveDisplay->getBottom(),
							   getWidth()-4, currentHeight - deck2DragWaveDisplay->getBottom() - 2);
	settingsButton->setBounds(currentWidth-60, tabbedComponent->getY()+5, 60, 20);
	
//#ifdef NDEBUG
	cpuMeter->setBounds(5, 5, 60, 20);
//#endif
}

void MainComponent::paint (Graphics& g)
{
	g.setColour(Colours::red);
}
void MainComponent::paintOverChildren (Graphics& g)
{
	g.setColour(Colours::red);
}
//==============================================================================
void MainComponent::sliderValueChanged(Slider *slider)
{
	if (slider == zoomSlider)
	{
		zoomFactor = zoomSlider->getValue();
		deck1DragWaveDisplay->setZoomFactor(zoomFactor);
		deck2DragWaveDisplay->setZoomFactor(zoomFactor);
	}
	else if (slider == playheadPosSlider)
	{
		deck1DragWaveDisplay->setPlayheadPosition(playheadPosSlider->getValue());
		deck2DragWaveDisplay->setPlayheadPosition(playheadPosSlider->getValue());
	}
}

void MainComponent::buttonClicked(Button *button)
{
//	if (button = settingsButton)
//		audioDeviceManager.
}

//==============================================================================
// The following methods implement the ApplicationCommandTarget interface, allowing
// this window to publish a set of actions it can perform, and which can be mapped
// onto menus, keypresses, etc.

ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
	// this will return the next parent component that is an ApplicationCommandTarget (in this
	// case, there probably isn't one, but it's best to use this method in your own apps).
	return findFirstTargetParentComponent();
}

void MainComponent::getAllCommands (Array <CommandID>& commands)
{
	// this returns the set of all commands that this target can perform..
	const CommandID ids[] = { 
#if ! JUCE_LINUX
		goToKioskMode
#endif
	};
	
	commands.addArray (ids, numElementsInArray (ids));
}

void MainComponent::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
	const String generalCategory (T("General"));
	
	switch (commandID)
	{

#if ! JUCE_LINUX
		case goToKioskMode:
			result.setInfo (T("Show full-screen kiosk mode"), String::empty, generalCategory, 0);
			result.setTicked (Desktop::getInstance().getKioskModeComponent() != 0);
			result.addDefaultKeypress (T('f'), ModifierKeys::commandModifier);
			break;
#endif
			
		default:
			break;
			
	};
}

// this is the ApplicationCommandTarget method that is used to actually perform one of our commands..
bool MainComponent::perform (const InvocationInfo& info)
{
	DBG("Perform called");
	switch (info.commandID)
	{
			
#if ! JUCE_LINUX
		case goToKioskMode:
			if (Desktop::getInstance().getKioskModeComponent() == 0)
			{
				TopLevelWindow::getActiveTopLevelWindow()->setUsingNativeTitleBar(false);
				Desktop::getInstance().setKioskModeComponent (getTopLevelComponent(), false);
			}
			else
			{
				TopLevelWindow::getActiveTopLevelWindow()->setUsingNativeTitleBar(true);
				Desktop::getInstance().setKioskModeComponent (0);
			}
			break;
#endif
			
		default:
			return false;
	};
	
	return true;
}	

//==============================================================================
void MainComponent::audioDeviceIOCallback (const float** inputChannelData,
							int totalNumInputChannels,
							float** outputChannelData,
							int totalNumOutputChannels,
							int numSamples)
{		
	// pass the audio callback on to our player source
	audioSourcePlayer.audioDeviceIOCallback (inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
	
	// find peak levels
	peakLeft = 0.0f;
	peakRight = 0.0f;
	for (int i = 0; i < numSamples; i++)
	{
		if (outputChannelData[0][i] > peakLeft)
			peakLeft = outputChannelData[0][i];
		if (outputChannelData[1][i] > peakRight)
			peakRight = outputChannelData[1][i];
	}		
}

void MainComponent::audioDeviceAboutToStart (AudioIODevice* device)
{
	DBG("Audio device about to start");
	audioSourcePlayer.audioDeviceAboutToStart (device);
}

void MainComponent::audioDeviceStopped()
{
	DBG("Audio device stopped");
	audioSourcePlayer.audioDeviceStopped();
}
//==============================================================================