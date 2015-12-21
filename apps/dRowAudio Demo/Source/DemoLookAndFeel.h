/*
 *  DemoLookAndFeel.h
 *  dRowAudio Demo
 *
 *  Created by David Rowland on 29/12/2011.
 *  Copyright 2011 dRowAudio. All rights reserved.
 *
 */

#include "DemoHeader.h"

#ifndef DROWAUDIO_DEMOLOOKANDFEEL_H
#define DROWAUDIO_DEMOLOOKANDFEEL_H

class DemoLookAndFeel : public LookAndFeel_V3
{
public:
    DemoLookAndFeel();

    void drawButtonBackground (Graphics& g,
                               Button& button,
                               const Colour& backgroundColour,
                               bool isMouseOverButton,
                               bool isButtonDown) override;
};

#endif  // DROWAUDIO_DEMOLOOKANDFEEL_H
