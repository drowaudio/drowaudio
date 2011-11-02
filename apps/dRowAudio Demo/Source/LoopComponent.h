/*
  ==============================================================================

    LoopComponent.h
    Created: 9 Oct 2011 3:04:02am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __LOOPCOMPONENT_H_AEA5B15F__
#define __LOOPCOMPONENT_H_AEA5B15F__

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class LoopMarker :  public Component
{
public:    
    //==============================================================================
    LoopMarker()
    {
    }

    ~LoopMarker() {}
    
    void resized ()
    {
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(), getHeight(), getWidth());
    }
    
    void paint (Graphics& g)
    {
        g.fillAll (isMouseOver ? Colours::red.withAlpha (0.9f) : Colours::red.withAlpha (0.7f));
    }
    
    void mouseEnter (const MouseEvent& e)
    {
        setMouseCursor (MouseCursor::PointingHandCursor);
        isMouseOver = true;
        repaint();
    }

    void mouseExit (const MouseEvent& e)
    {
        setMouseCursor (MouseCursor::NormalCursor);
        isMouseOver = false;
        repaint();
    }
    
    void mouseDown (const MouseEvent& e)
    {
        setMouseCursor (MouseCursor::DraggingHandCursor);
        dragger.startDraggingComponent (this, e);
    } 

    void mouseUp (const MouseEvent& e)
    {
        setMouseCursor (MouseCursor::PointingHandCursor);
    }

    void mouseDrag (const MouseEvent& e)
    {
        dragger.dragComponent (this, e, &constrainer);
    }
    
private:
    //==============================================================================
    ComponentDragger dragger;
    ComponentBoundsConstrainer constrainer;
    bool isMouseOver;
};

//==============================================================================
class LoopComponent :   public Component,
                        public ComponentListener,
                        public AudioFilePlayer::Listener
{
public:    
    //==============================================================================
    LoopComponent (AudioFilePlayer& audioFilePlayer);
    
    ~LoopComponent();
    
    void resized();
    
    void paint (Graphics& g);
    
    void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);

    void fileChanged (AudioFilePlayer* player);
    
    void loopBetweenTimesChanged (AudioFilePlayer* player);
        
private:
    //==============================================================================
    AudioFilePlayer& audioFilePlayer;
    
    LoopMarker marker1, marker2;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopComponent);
};

#endif  // __LOOPCOMPONENT_H_AEA5B15F__
