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

#include "../../JuceLibraryCode/JuceHeader.h"
#include "DistortionComponent.h"

//==============================================================================
class DistortionDemo :  public Component,
                        public Button::Listener
{
public:
    //==============================================================================
    DistortionDemo (Buffer& bufferToControl);
    
    ~DistortionDemo();
    
    void resized();
    
    void paint (Graphics& g);
    
    void buttonClicked (Button* button);
    
private:
    //==============================================================================
    DistortionComponent distortionComponent;
    TextButton resetButton;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionDemo);
};