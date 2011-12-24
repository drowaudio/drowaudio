/*
  ==============================================================================

    AudioPlaybackDemo.h
    Created: 8 Oct 2011 4:27:04pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __AUDIOPLAYBACKDEMO_H_4F99037A__
#define __AUDIOPLAYBACKDEMO_H_4F99037A__

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoopComponent.h"

//==============================================================================
class AudioPlaybackDemo :   public Component,
                            public AudioFilePlayer::Listener,
                            public Slider::Listener
{
public:    
    //==============================================================================
    AudioPlaybackDemo (AudioFilePlayerExt& audioFilePlayer);
    
    ~AudioPlaybackDemo();
    
    void resized();
    
    void paint (Graphics& g);
    
    void fileChanged (AudioFilePlayer* player);
    
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
    
    MultipleAudioThumbnailCache thumbnailCache;
    ColouredAudioThumbnail audioThumbnail;
//    ScopedPointer<ColouredPositionableWaveDisplay> positionalDisplay;
//    ScopedPointer<ColouredDraggableWaveDisplay> draggableDisplay;
    
    LoopComponent loopComponent;
    
    GroupComponent filterGroup, rateGroup;
    Slider resolutionSlider, zoomSlider;
    OwnedArray<Slider> playerControls;
    OwnedArray<Label> playerControlLabels;
    
    TimeSliceThread backgroundThread;
    ScopedPointer<AudioThumbnailImage> audioThumbnailImage;
    ScopedPointer<PositionableWaveDisplay> positionableWaveDisplay;
    //ImageComponent thumbnailImage;
    
    friend class LoopComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlaybackDemo);
};

#endif  // __AUDIOPLAYBACKDEMO_H_4F99037A__