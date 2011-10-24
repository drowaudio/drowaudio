/*
  ==============================================================================

    MainComponent.cpp
    Created: 8 Oct 2011 4:25:42pm
    Author:  David Rowland

  ==============================================================================
*/

#include "MainComponent.h"
#include "AudioPlaybackDemo.h"

MainComponent::MainComponent()
    : trackInfoComponent (audioFilePlayer),
      dropTarget (&audioFilePlayer, &trackInfoComponent),
      transport (audioFilePlayer),
      meterThread ("Meter Thread"),
      cpuMeter (&audioDeviceManager),
      tabbedComponent (TabbedButtonBar::TabsAtTop)
{
    addAndMakeVisible (&trackInfoComponent);
    addAndMakeVisible (&dropTarget);
    addAndMakeVisible (&transport);
    addAndMakeVisible (&meterL);
    addAndMakeVisible (&meterR);
    addAndMakeVisible (&tabbedComponent);
    addAndMakeVisible (&cpuMeter);
              
    meterThread.addTimeSliceClient (&meterL);
    meterThread.addTimeSliceClient (&meterR);
    meterThread.startThread (1);
//    meterThread.addGraphicalComponent (&meterL);
//    meterThread.addGraphicalComponent (&meterR);
    
    cpuMeter.setTextColour (Colours::red);
    cpuMeter.setJustificationType (Justification::centred);
    
    addAndMakeVisible (&searchBox);
    searchBox.addListener (this);
    
    tabbedComponent.addTab("Audio Playback",
                           Colours::grey, new AudioPlaybackDemo (audioFilePlayer), true);
    
//    File libraryFile (File::getSpecialLocation (File::currentApplicationFile)
//                                             .getChildFile ("dRowAudio Demo Library.xml"));
//    ValueTree libraryTree (readValueTreeFromFile (libraryFile));
//    ITunesLibrary::getInstance()->setLibraryTree (libraryTree);
    ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
    MusicLibraryTable* musicLibraryTable = new MusicLibraryTable;
    musicLibraryTable->setLibraryToUse (ITunesLibrary::getInstance());
    tabbedComponent.addTab ("iTunes Library",
                            Colours::grey, musicLibraryTable, true);
    
    audioSourcePlayer.setSource (&audioFilePlayer);
    audioDeviceManager.initialise (0, 2, nullptr, true);
//    audioDeviceManager.addAudioCallback (&audioSourcePlayer);
    audioDeviceManager.addAudioCallback (this);
}

MainComponent::~MainComponent()
{
    meterThread.removeTimeSliceClient (&meterL);
    meterThread.removeTimeSliceClient (&meterR);
    meterThread.stopThread (500);
    
    audioSourcePlayer.setSource (nullptr);
    audioDeviceManager.removeAudioCallback (this);
    
    File libraryFile (File::getSpecialLocation (File::currentApplicationFile).getChildFile ("dRowAudio Demo Library.xml"));
    ValueTree libraryTree (ITunesLibrary::getInstance()->getLibraryTree());
    writeValueTreeToFile (libraryTree, libraryFile);
}

void MainComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    const int m = 5;

    trackInfoComponent.setBounds (0, 0, w - 145, 100);
    transport.setBounds (w - 145, 0, 100, h);
    
    meterL.setBounds(transport.getRight() + 5, 0, 15, trackInfoComponent.getHeight());
    meterR.setBounds(meterL.getRight() + 5, 0, 15, trackInfoComponent.getHeight());
    
    tabbedComponent.setBounds (0, trackInfoComponent.getBottom(), w, h - trackInfoComponent.getBottom());
    searchBox.setBounds (transport.getX(), tabbedComponent.getY() + 5, w - transport.getX() - 5, tabbedComponent.getTabBarDepth() - 10);
//    cpuMeter.setBounds (transport.getRight(), tabbedComponent.getY(), w - transport.getRight(), tabbedComponent.getTabBarDepth());
}

void MainComponent::textEditorTextChanged (TextEditor& editor)
{
    if (&editor == &searchBox)
    {
        MusicLibraryTable* musicLibraryTable = static_cast<MusicLibraryTable*> (tabbedComponent.getTabContentComponent (1));
        musicLibraryTable->setFilterText (searchBox.getText());
        
        if (tabbedComponent.getCurrentTabName() != "iTunes Library")
        {
            tabbedComponent.setCurrentTabIndex (1, true);
        }
        searchBox.grabKeyboardFocus();
    }
}

void MainComponent::audioDeviceIOCallback (const float** inputChannelData,
                                           int numInputChannels,
                                           float** outputChannelData,
                                           int numOutputChannels,
                                           int numSamples)
{
    audioSourcePlayer.audioDeviceIOCallback (inputChannelData,
                                             numInputChannels,
                                             outputChannelData,
                                             numOutputChannels,
                                             numSamples);
    
    meterL.copySamples (outputChannelData[0], numSamples);
    
    if (numOutputChannels > 1)
        meterR.copySamples (outputChannelData[1], numSamples);
}

void MainComponent::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioSourcePlayer.audioDeviceAboutToStart (device);
}

void MainComponent::audioDeviceStopped()
{
    audioSourcePlayer.audioDeviceStopped();
}
