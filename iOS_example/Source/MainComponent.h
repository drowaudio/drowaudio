/*
  ==============================================================================

    MainComponent.h
    Created: 8 Jun 2012 7:39:57am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_55E6F46C__
#define __MAINCOMPONENT_H_55E6F46C__

#include "StandardHeader.h"
#include "AudioManager.h"

//==============================================================================
class MainComponent :   public Component,
                        public Timer,
                        public Button::Listener,
                        public Slider::Listener,
                        public AudioPicker::Listener
{
public:
    //==============================================================================
    MainComponent();
    
    ~MainComponent();
    
    void resized();
    
    void paint (Graphics& g);
    
    //==============================================================================
    void timerCallback();
    
    void buttonClicked (Button* button);
    
    void sliderValueChanged (Slider* slider);
    
    //==============================================================================
    void audioPickerFinished (const Array<void*> mpMediaItems);
    
    void audioPickerCancelled();
    
private:
    //==============================================================================
    AudioManager audioManager;
    
    TextButton pickButton, playButton;
    Slider positionSlider;

    AudioPicker audioPicker;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent);
};

#endif  // __MAINCOMPONENT_H_55E6F46C__
