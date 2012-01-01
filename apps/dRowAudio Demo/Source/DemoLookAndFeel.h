/*
 *  DemoLookAndFeel.h
 *  dRowAudio Demo
 *
 *  Created by David Rowland on 29/12/2011.
 *  Copyright 2011 dRowAudio. All rights reserved.
 *
 */

#include "../JuceLibraryCode/JuceHeader.h"

class DemoLookAndFeel : public LookAndFeel
{
public:
    DemoLookAndFeel();
    
    ~DemoLookAndFeel();

//    static void drawBevel (Graphics& g, Rectangle<int> innerBevelBounds, float bevelThickness, Colour baseColour);
//
//    static void drawBevel (Graphics& g, Rectangle<float> innerBevelBounds, float bevelThickness, Colour baseColour);
    
    void drawButtonBackground (Graphics& g,
                               Button& button,
                               const Colour& backgroundColour,
                               bool isMouseOverButton,
                               bool isButtonDown);
    
private:
};