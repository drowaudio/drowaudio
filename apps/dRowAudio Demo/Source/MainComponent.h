/*
  ==============================================================================

    MainComponent.h
    Created: 8 Oct 2011 4:25:42pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_7EAA32A__
#define __MAINCOMPONENT_H_7EAA32A__

#include "../JuceLibraryCode/JuceHeader.h"
#include "TrackInfoComponent.h"
#include "TransportComponent.h"

class FFTDemo;

//==============================================================================
class MainComponent :   public Component,
                        public TextEditor::Listener,
                        public DragAndDropContainer,
                        public AudioIODeviceCallback
{
public:    
    //==============================================================================
    MainComponent();
    
    ~MainComponent();
    
    void resized();
    
    void textEditorTextChanged (TextEditor& editor);
    
    void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
    void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    
private:
    //==============================================================================
    AudioDeviceManager audioDeviceManager;
    AudioSourcePlayer audioSourcePlayer;
    AudioFilePlayerExt audioFilePlayer;

    TrackInfoComponent trackInfoComponent;
    AudioFileDropTarget dropTarget;
    TransportComponent transport;
    
    TimeSliceThread meterThread;
    SegmentedMeter meterL, meterR;
    
    Clock clock;
    CpuMeter cpuMeter;
    TabbedComponent tabbedComponent;
    TextEditor searchBox;
    
    FFTDemo* fftDemo;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent);
};

#endif  // __MAINCOMPONENT_H_7EAA32A__
