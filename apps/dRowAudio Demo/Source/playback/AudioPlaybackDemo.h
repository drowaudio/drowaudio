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

#ifndef __AUDIOPLAYBACKDEMO_H_4F99037A__
#define __AUDIOPLAYBACKDEMO_H_4F99037A__

#include "../DemoHeader.h"
#include "LoopComponent.h"
#include "DistortionDemo.h"
#include "BufferTransformAudioSource.h"

//==============================================================================
class AudioPlaybackDemo :   public Component,
                            public Slider::Listener
{
public:    
    //==============================================================================
    AudioPlaybackDemo (AudioFilePlayerExt& audioFilePlayer,
                       BufferTransformAudioSource& bufferTransformAudioSource);
    
    ~AudioPlaybackDemo();
    
    void resized();
    
    void paint (Graphics& g);
        
    void sliderValueChanged (Slider* slider);
    
    enum PlayerControls 
    {
        lowEQ,
        midEQ,
        highEQ,
        rate,
        tempo,
        pitch,
        numControls
    };

private:
    //==============================================================================
    AudioFilePlayerExt& audioFilePlayer;

    LoopComponent loopComponent;
    
    GroupComponent filterGroup, rateGroup;
    Slider resolutionSlider, zoomSlider;
    Label resolutionLabel, zoomLabel;

    OwnedArray<Slider> playerControls;
    OwnedArray<Label> playerControlLabels;
    
    TimeSliceThread backgroundThread;
    AudioThumbnailCache audioThumbnailCache;
#if JUCE_MAC
    ColouredAudioThumbnail audioThumbnail;
#else
    AudioThumbnail audioThumbnail;
#endif
    ScopedPointer<AudioThumbnailImage> audioThumbnailImage;
    ScopedPointer<PositionableWaveDisplay> positionableWaveDisplay;
    ScopedPointer<DraggableWaveDisplay> draggableWaveDisplay;
    
    DistortionDemo distortionDemo;
    
    friend class LoopComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlaybackDemo);
};

#endif  // __AUDIOPLAYBACKDEMO_H_4F99037A__