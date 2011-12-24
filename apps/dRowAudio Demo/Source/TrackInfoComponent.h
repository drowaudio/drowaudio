/*
  ==============================================================================

    TrackInfoComponent.h
    Created: 8 Oct 2011 4:26:24pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __TRACKINFOCOMPONENT_H_75E955F2__
#define __TRACKINFOCOMPONENT_H_75E955F2__

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class TrackInfoComponent :  public Component,
                            public AudioFilePlayer::Listener,
                            public Timer
{
public:    
    //==============================================================================
    TrackInfoComponent (AudioFilePlayerExt& audioFilePlayer);
    
    ~TrackInfoComponent();
    
    void resized();
    
    void paint (Graphics& g);
    
    void fileChanged (AudioFilePlayer* player);
    
    void audioFilePlayerSettingChanged (AudioFilePlayer* player, int settingCode);
    
    void timerCallback();
    
private:
    //==============================================================================
    AudioFilePlayerExt& audioFilePlayer;
    Label bpmLabel, remainLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackInfoComponent);
};

#endif  // __TRACKINFOCOMPONENT_H_75E955F2__
