/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#include "MainComponent.h"
#include "playback/AudioPlaybackDemo.h"
#include "network/NetworkDemo.h"
#include "fft/FFTDemo.h"

MainComponent::MainComponent()
    : bufferTransformAudioSource    (&audioFilePlayer),
      trackInfoComponent            (audioFilePlayer),
      dropTarget                    (&audioFilePlayer, &trackInfoComponent),
      transport                     (audioDeviceManager, audioFilePlayer),
      meterThread                   ("Meter Thread"),
      cpuMeter                      (&audioDeviceManager),
      tabbedComponent               (TabbedButtonBar::TabsAtTop)
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
    
    addAndMakeVisible (&clock);
    clock.setColour (Label::textColourId, Colours::white);
    clock.setJustificationType (Justification::centred);

    cpuMeter.setTextColour (Colours::red);
    cpuMeter.setJustificationType (Justification::centred);
    cpuMeter.setBorderSize (0, 0);
    
    addAndMakeVisible (&searchBox);
    searchBox.addListener (this);
    searchBox.setTextToShowWhenEmpty ("search...", Colours::grey);
    
    tabbedComponent.addTab("Audio Playback",
                           Colours::grey, 
                           new AudioPlaybackDemo (audioFilePlayer, bufferTransformAudioSource.getBuffer()), 
                           true);
    
//    File libraryFile (File::getSpecialLocation (File::currentApplicationFile)
//                                             .getChildFile ("dRowAudio Demo Library.xml"));
//    ValueTree libraryTree (readValueTreeFromFile (libraryFile));
//    ITunesLibrary::getInstance()->setLibraryTree (libraryTree);
    ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
    MusicLibraryTable* musicLibraryTable = new MusicLibraryTable;
    musicLibraryTable->setLibraryToUse (ITunesLibrary::getInstance());
    tabbedComponent.addTab ("iTunes Library",
                            Colours::grey, musicLibraryTable, true);

    tabbedComponent.addTab ("Column File Browser",
                            Colours::grey, 
                            new ColumnFileBrowser (new WildcardFileFilter (audioFilePlayer.getAudioFormatManager()->getWildcardForAllFormats(),
                                                                                          "*", 
                                                                                          "Audio Files")), 
                            true);

    fftDemo = new FFTDemo();
    tabbedComponent.addTab ("FFT Demo",
                            Colours::grey, 
                            fftDemo, 
                            true);
    
#if DROWAUDIO_USE_CURL
    tabbedComponent.addTab ("CURL Demo",
                            Colours::grey, 
                            new NetworkDemo(), 
                            true);
#endif
        
    audioSourcePlayer.setSource (&bufferTransformAudioSource);
    audioDeviceManager.initialise (0, 2, nullptr, true);
//    audioDeviceManager.addAudioCallback (&audioSourcePlayer);
    audioDeviceManager.addAudioCallback (this);
//    audioDeviceManager.addAudioCallback (fftDemo);
}

MainComponent::~MainComponent()
{
    meterThread.removeTimeSliceClient (&meterL);
    meterThread.removeTimeSliceClient (&meterR);
    meterThread.stopThread (500);
    
    audioSourcePlayer.setSource (nullptr);
    audioDeviceManager.removeAudioCallback (this);
//    audioDeviceManager.removeAudioCallback (fftDemo);
    
    File libraryFile (File::getSpecialLocation (File::currentApplicationFile).getChildFile ("dRowAudio Demo Library.xml"));
    ValueTree libraryTree (ITunesLibrary::getInstance()->getLibraryTree());
    writeValueTreeToFile (libraryTree, libraryFile);
}

void MainComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();
//    const int m = 5;

    trackInfoComponent.setBounds (0, 0, w - 145, 100);
    transport.setBounds (w - 145, 20, 100, h - 20);
    clock.setBounds (transport.getX(), 0, transport.getWidth(), 20);

    cpuMeter.setBounds (clock.getRight(), 0, w - clock.getRight(), clock.getHeight());
    meterL.setBounds(transport.getRight() + 5, cpuMeter.getBottom(), 15, trackInfoComponent.getHeight() - cpuMeter.getHeight());
    meterR.setBounds(meterL.getRight() + 5, cpuMeter.getBottom(), 15, trackInfoComponent.getHeight() - cpuMeter.getHeight());
    
    tabbedComponent.setBounds (0, trackInfoComponent.getBottom(), w, h - trackInfoComponent.getBottom());
    searchBox.setBounds (transport.getX(), tabbedComponent.getY() + 5, w - transport.getX() - 5, tabbedComponent.getTabBarDepth() - 10);
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
    
    if (fftDemo->isShowing())
        fftDemo->processBlock (outputChannelData[0], numSamples);
    
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
