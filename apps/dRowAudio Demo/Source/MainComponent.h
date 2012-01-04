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

#ifndef __MAINCOMPONENT_H_7EAA32A__
#define __MAINCOMPONENT_H_7EAA32A__

#include "../JuceLibraryCode/JuceHeader.h"
#include "TrackInfoComponent.h"
#include "TransportComponent.h"
#include "playback/BufferTransformAudioSource.h"

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
    BufferTransformAudioSource bufferTransformAudioSource;
    FilteringAudioSource filteringAudioSource;
        
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
