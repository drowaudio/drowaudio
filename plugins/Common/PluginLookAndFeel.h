/*
 *  dRowAudioLookAndFeel.h
 *  LookAndFeel_Test
 *
 *  Created by David Rowland on 27/12/2008.
 *  Copyright 2008 UWE. All rights reserved.
 *
 */

#ifndef _dRowAudioLookAndFeel_H_
#define _dRowAudioLookAndFeel_H_

//#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class PluginLookAndFeel :   public LookAndFeel
{
public:
    //==============================================================================
    static void drawInsetLine (Graphics& g,
							   const float startX,
							   const float startY,
							   const float endX,
							   const float endY,
							   const float lineThickness);
    
    static void drawPluginBackgroundBase (Graphics& g, Component& editor)
    {
        Colour backgroundColour (0xFF455769);
        
        backgroundColour = backgroundColour.withBrightness(0.4f);
        g.setColour (backgroundColour);
        g.fillRoundedRectangle (0, 0, editor.getWidth(), editor.getHeight(), 10);
    }
    
    static void drawPluginBackgroundHighlights (Graphics& g, Component& editor)
    {
        const Colour backgroundColour (0xFF455769);

        ColourGradient topHighlight (Colours::white.withAlpha(0.3f),
                                     0, 0,
                                     Colours::white.withAlpha(0.0f),
                                     0, 0 + 15,
                                     false);
        
        
        g.setGradientFill (topHighlight);
        g.fillRoundedRectangle (0, 0, editor.getWidth(), 30, 10);	
        
        ColourGradient outlineGradient (Colours::white,
                                        0, 0,
                                        backgroundColour.withBrightness(0.5f),
                                        0, 20,
                                        false);
        g.setGradientFill (outlineGradient);
        g.drawRoundedRectangle (0, 0, editor.getWidth(), editor.getHeight(), 10, 1.0f);
    }
    //==============================================================================
	virtual void drawRotarySlider (Graphics& g,
                                   int x, int y,
                                   int width, int height,
                                   float sliderPosProportional,
                                   const float rotaryStartAngle,
                                   const float rotaryEndAngle,
                                   Slider& slider);
		
	virtual void drawFaderKnob(Graphics& g,
							   const float x,
							   const float y,
							   const float width,
							   const float height);
    
	/**
     Draws a label.
     If the label's background is not transparent then it will draw a 3D label.
	 */
	virtual void drawLabel (Graphics& g, Label& label);
};

#endif