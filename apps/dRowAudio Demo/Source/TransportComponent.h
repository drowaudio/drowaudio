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
