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

#ifndef __AUDIOPLAYBACKDEMO_H_4F99037A__
#define __AUDIOPLAYBACKDEMO_H_4F99037A__

#include "../DemoHeader.h"
#include "LoopComponent.h"
#include "DistortionDemo.h"

//==============================================================================
class AudioPlaybackDemo :   public Component,
                            public Slider::Listener
{
public:    
    //==============================================================================
    AudioPlaybackDemo (AudioFilePlayerExt& audioFilePlayer,
                       Buffer& distortionBuffer);
    
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

    //==============================================================================
//    void imageChanged (AudioThumbnailImage* audioThumbnailImage);
//    
//    void imageUpdated (AudioThumbnailImage* audioThumbnailImage);
//    
//    void imageFinished (AudioThumbnailImage* audioThumbnailImage);

private:
    //==============================================================================
    AudioFilePlayerExt& audioFilePlayer;
    
//    MultipleAudioThumbnailCache thumbnailCache;
//    ColouredAudioThumbnail audioThumbnail;
//    ScopedPointer<ColouredPositionableWaveDisplay> positionalDisplay;
//    ScopedPointer<ColouredDraggableWaveDisplay> draggableDisplay;
    
    LoopComponent loopComponent;
    
    GroupComponent filterGroup, rateGroup;
    Slider resolutionSlider, zoomSlider;
    Label zoomLabel;

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