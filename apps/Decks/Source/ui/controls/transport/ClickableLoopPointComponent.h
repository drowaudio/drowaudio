/*
  ==============================================================================

    ClickableLoopPointComponent.h
    Created: 6 Aug 2011 6:39:41pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __CLICKABLELOOPPOINTCOMPONENT_H_737B0322__
#define __CLICKABLELOOPPOINTCOMPONENT_H_737B0322__

#include "../JuceLibraryCode/JuceHeader.h"


class LoopPointComponent;

class ClickableLoopPointComponent :  public Component,
                                     public AudioFilePlayer::Listener,
                                     public ValueTree::Listener
{
public:
    
    ClickableLoopPointComponent (AudioFilePlayer* playerToRespondTo);
    
    ~ClickableLoopPointComponent();
    
    void resized();
        
    //==============================================================================
    void fileChanged (AudioFilePlayer* player);

    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {}
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {}
    void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved) {}
    void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) {}
    
    //==============================================================================

private:
    
    AudioFilePlayer *filePlayer;
    ValueTree loopPointTree;
    OwnedArray<LoopPointComponent> loopPoints;

    void updateLoopPoints();
    
    JUCE_LEAK_DETECTOR (ClickableLoopPointComponent);
};

#endif  // __CLICKABLELOOPPOINTCOMPONENT_H_737B0322__
