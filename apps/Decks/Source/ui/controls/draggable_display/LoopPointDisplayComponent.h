/*
  ==============================================================================

    LoopPointDisplayComponent.h
    Created: 7 Aug 2011 4:57:11am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __LOOPPOINTDISPLAYCOMPONENT_H_F9D889D5__
#define __LOOPPOINTDISPLAYCOMPONENT_H_F9D889D5__

#include "../JuceLibraryCode/JuceHeader.h"


class LoopPointMarker;

class LoopPointDisplayComponent :   public Component,
                                    public AudioFilePlayer::Listener,
                                    public ValueTree::Listener,
                                    public Timer
{
public:
    LoopPointDisplayComponent(AudioFilePlayer* playerToRespondTo, int numSamplesPerThumbnailSample);
    
    ~LoopPointDisplayComponent();
    
    void resized();

    void paint (Graphics& g);
    
    void fileChanged (AudioFilePlayer *player);
    void loopStateChanged (AudioFilePlayer *player, int loopIndex, bool isEnabled);
    
    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {}
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {}
    void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved) {}
    void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) {}
    
    void timerCallback();
    
    void setZoomFactor (float newZoomFactor);
    
    void setPlayheadPosition (float newPlayHeadPosition);
    
    inline double timeToPixels (double time)    {   return time * oneOverTimePerPixel;  }
    inline double pixelsToTime (double pixels)  {   return pixels * timePerPixel;  }
    
private:
    
    AudioFilePlayer* filePlayer;
    ValueTree loopPointTree;
    int scale;
    float zoomRatio, playheadPosition;
    double currentSampleRate;
    double timePerPixel, oneOverTimePerPixel;
    OwnedArray<LoopPointMarker> markerArray;

    void updateLoopPoints();

    JUCE_LEAK_DETECTOR (LoopPointDisplayComponent);
};

#endif  // __LOOPPOINTDISPLAYCOMPONENT_H_F9D889D5__
