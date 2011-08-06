/*
  ==============================================================================

    ClickableCuePointComponent.h
    Created: 6 Aug 2011 6:39:41pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __CLICKABLECUEPOINTCOMPONENT_H_737B0322__
#define __CLICKABLECUEPOINTCOMPONENT_H_737B0322__

#include <dRowAudio/dRowAudio.h>

class CuePointComponent;

class ClickableCuePointComponent :  public Component,
                                    public FilteringAudioFilePlayer::Listener,
                                    public ValueTree::Listener
{
public:
    
    ClickableCuePointComponent(FilteringAudioFilePlayer* playerToRespondTo);
    
    ~ClickableCuePointComponent();
    
    void resized();
        
    //==============================================================================
    void fileChanged (FilteringAudioFilePlayer* player);

    void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {}
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {}
    void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved) {}
    void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) {}
    
    //==============================================================================

private:
    
    FilteringAudioFilePlayer *filePlayer;
    ValueTree cuePointTree;
    OwnedArray<CuePointComponent> cuePoints;

    void updateCuePoints();
    
    JUCE_LEAK_DETECTOR (ClickableCuePointComponent);
};

#endif  // __CLICKABLECUEPOINTCOMPONENT_H_737B0322__
