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

#ifndef __DROWAUDIO_DISTORTIONCOMPONENT_H__
#define __DROWAUDIO_DISTORTIONCOMPONENT_H__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "CurvePoint.h"

//==============================================================================
class DistortionComponent : public Component,
                            public Buffer::Listener,
                            public ComponentListener
{
public:
    //==============================================================================
    DistortionComponent (Buffer& bufferToControl);

    ~DistortionComponent();
    
    void resized();
    
    void paint (Graphics& g);
    
    void bufferChanged (Buffer* buffer);
    
    void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized);
        
private:
    //==============================================================================
	enum CurvePoints
    {
		pointX1,
		pointY1,
		pointX2,
		pointY2,
		numPoints,
	};    
    
    //==============================================================================
    Buffer& buffer;
    Path path;
    
    OwnedArray<CurvePoint> curvePoints;
    OwnedArray<Value> values;
    
    void refreshPath();
    void refillBuffer (float x1, float y1, float x2, float y2);

    Image background;
    bool isInitialised;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionComponent);
};

#endif // __DROWAUDIO_DISTORTIONCOMPONENT_H__