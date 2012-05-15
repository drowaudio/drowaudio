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

#ifndef __TRACKINFOCOMPONENT_H_75E955F2__
#define __TRACKINFOCOMPONENT_H_75E955F2__

#include "DemoHeader.h"

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
