/*
  ==============================================================================

    TransportComponent.h
    Created: 8 Oct 2011 7:36:32pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __TRANSPORTCOMPONENT_H_E0AC5162__
#define __TRANSPORTCOMPONENT_H_E0AC5162__

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class TransportComponent :  public Component,
                            public Button::Listener
{
public:    
    //==============================================================================
    TransportComponent (AudioFilePlayerExt& audioFilePlayer);
    
    ~TransportComponent();
    
    void resized();
        
    void buttonClicked (Button* button);
    
    enum Buttons
    {
        play,
        stop,
        loop,
        reverse,
        numButtons
    };
    
private:
    //==============================================================================
    AudioFilePlayerExt& audioFilePlayer;
    OwnedArray<TextButton> buttons;    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransportComponent);
};

#endif  // __TRANSPORTCOMPONENT_H_E0AC5162__
