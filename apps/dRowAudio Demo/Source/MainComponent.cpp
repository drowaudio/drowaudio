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
      cpuMeter (&audioDeviceManager),
      tabbedComponent (TabbedButtonBar::TabsAtTop)
{
    addAndMakeVisible (&trackInfoComponent);
    addAndMakeVisible (&dropTarget);
    addAndMakeVisible (&transport);
    addAndMakeVisible (&tabbedComponent);
    addAndMakeVisible (&cpuMeter);
                       
    cpuMeter.setTextColour (Colours::red);
    cpuMeter.setJustificationType (Justification::centred);
    
    addAndMakeVisible (&searchBox);
    searchBox.addListener (this);
    
    tabbedComponent.addTab("Audio Playback",
                           Colours::grey, new AudioPlaybackDemo (audioFilePlayer), true);
    
    ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
    MusicLibraryTable* musicLibraryTable = new MusicLibraryTable;
    musicLibraryTable->setLibraryToUse (ITunesLibrary::getInstance());
    tabbedComponent.addTab ("iTunes Library",
                            Colours::grey, musicLibraryTable, true);
    
    audioSourcePlayer.setSource (&audioFilePlayer);
    audioDeviceManager.initialise (0, 2, nullptr, true);
    audioDeviceManager.addAudioCallback (&audioSourcePlayer);
}

MainComponent::~MainComponent()
{
    audioSourcePlayer.setSource (nullptr);
    audioDeviceManager.removeAudioCallback (&audioSourcePlayer);
}

void MainComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();

    trackInfoComponent.setBounds (0, 0, w, 100);
    transport.setBounds (w - 180, 0, 80, h);
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