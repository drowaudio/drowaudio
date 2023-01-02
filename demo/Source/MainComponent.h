/*
    ==============================================================================

    This file is part of the dRowAudio JUCE module
    Copyright 2004-13 by dRowAudio.

    ------------------------------------------------------------------------------

    dRowAudio is provided under the terms of The MIT License (MIT):

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    ==============================================================================
*/

#ifndef MAIN_COMPONENT_H
#define MAIN_COMPONENT_H

#include "TrackInfoComponent.h"
#include "TransportComponent.h"
#include "playback/BufferTransformAudioSource.h"

class FFTDemo;

class MainComponent : public Component,
                      public TextEditor::Listener,
                      public DragAndDropContainer,
                      public AudioIODeviceCallback
{
public:
    MainComponent();
    ~MainComponent();

    //==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void textEditorTextChanged (TextEditor& editor) override;
    /** @internal */
    void audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                           int numInputChannels,
                                           float* const* outputChannelData,
                                           int numOutputChannels,
                                           int numSamples,
                                           const AudioIODeviceCallbackContext& context) override;
    /** @internal */
    void audioDeviceAboutToStart (AudioIODevice* device) override;
    /** @internal */
    void audioDeviceStopped() override;

private:
    //==============================================================================
    TextEditor searchBox;
    
    AudioDeviceManager audioDeviceManager;
    AudioSourcePlayer audioSourcePlayer;
    AudioFilePlayerExt audioFilePlayer;
    BufferTransformAudioSource bufferTransformAudioSource;

    TrackInfoComponent trackInfoComponent;
    AudioFileDropTarget dropTarget;
    TransportComponent transport;

    TimeSliceThread meterThread;
    SegmentedMeter meterL, meterR;

    Clock clock;
    CpuMeter cpuMeter;
    TabbedComponent tabbedComponent;

    FFTDemo* fftDemo;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

#endif //MAIN_COMPONENT_H
