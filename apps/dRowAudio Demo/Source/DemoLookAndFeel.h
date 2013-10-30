/*
 *  DemoLookAndFeel.h
 *  dRowAudio Demo
 *
 *  Created by David Rowland on 29/12/2011.
 *  Copyright 2011 dRowAudio. All rights reserved.
 *
 */

#include "DemoHeader.h"

#ifndef __DROWAUDIO_DEMOLOOKANDFEEL_H__
#define __DROWAUDIO_DEMOLOOKANDFEEL_H__

class DemoLookAndFeel : public LookAndFeel_V3
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

#endif  // __DROWAUDIO_DEMOLOOKANDFEEL_H__
