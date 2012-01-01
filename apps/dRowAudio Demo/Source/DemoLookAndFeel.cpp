/*
 *  DemoLookAndFeel.cpp
 *  dRowAudio Demo
 *
 *  Created by David Rowland on 29/12/2011.
 *  Copyright 2011 dRowAudio. All rights reserved.
 *
 */

#include "DemoLookAndFeel.h"

DemoLookAndFeel::DemoLookAndFeel()
{
}

DemoLookAndFeel::~DemoLookAndFeel()
{
}

void DemoLookAndFeel::drawButtonBackground (Graphics& g,
                                            Button& button,
                                            const Colour& backgroundColour,
                                            bool isMouseOverButton,
                                            bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();
    
//    const float outlineThickness = button.isEnabled() ? ((isButtonDown || isMouseOverButton) ? 1.2f : 0.7f) : 0.4f;
//    const float halfThickness = outlineThickness * 0.5f;
    
//    const float indentL = button.isConnectedOnLeft()   ? 0.1f : halfThickness;
//    const float indentR = button.isConnectedOnRight()  ? 0.1f : halfThickness;
//    const float indentT = button.isConnectedOnTop()    ? 0.1f : halfThickness;
//    const float indentB = button.isConnectedOnBottom() ? 0.1f : halfThickness;
    
    const Colour baseColour (GuiHelpers::createBaseColour (backgroundColour,
                                                                   button.hasKeyboardFocus (true),
                                                                   isMouseOverButton, isButtonDown)
                             .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));

    ColourGradient cg (baseColour.brighter (0.5f), 0.0f, 0.0f,
                       baseColour.darker (0.5f), 0.0f, (float)height,
                       false);
    
    Rectangle<float> bounds (g.getClipBounds().toFloat().reduced (0.0f, 1.0f));
    g.setGradientFill (cg);
    g.fillRoundedRectangle (bounds, 4.0f);
    
    bounds.setX (bounds.getX() + 0.5f);
    bounds.setWidth (bounds.getWidth() - 1.0f);
    bounds.setY (bounds.getY() + 0.5f);
    bounds.setHeight (bounds.getHeight() - 1.0f);
    
    g.setColour (Colours::black);
    g.drawRoundedRectangle (bounds, 4.0f, 1.0f);
    
    ColourGradient highlight (Colours::white.withAlpha (0.1f), 2.0f, (float)height,
                              Colours::white.withAlpha (0.1f), width - 2.0f, (float)height,
                              false);
    highlight.addColour (2.0f / (width - 4.0f),
                         Colours::white.withAlpha (0.5f));
    highlight.addColour (1.0f - (2.0f / (width - 4.0f)),
                         Colours::white.withAlpha (0.5f));
    g.setGradientFill (highlight);
    g.drawLine (2.0f, height, width - 2.0f, height - 0.5f, 0.5f);
//    drawGlassLozenge (g,
//                      indentL,
//                      indentT,
//                      width - indentL - indentR,
//                      height - indentT - indentB,
//                      baseColour, outlineThickness, -1.0f,
//                      button.isConnectedOnLeft(),
//                      button.isConnectedOnRight(),
//                      button.isConnectedOnTop(),
//                      button.isConnectedOnBottom());
}

//void DemoLookAndFeel::drawBevel (Graphics& g, Rectangle<int> innerBevelBounds, float bevelThickness, Colour baseColour)
//{
//    Rectangle<float> innerBevelBoundsFloat (innerBevelBounds.getX(), innerBevelBounds.getY(), innerBevelBounds.getWidth(), innerBevelBounds.getHeight());
//    drawBevel (g, innerBevelBoundsFloat, bevelThickness, baseColour);
//}
//
//void DemoLookAndFeel::drawBevel (Graphics& g, Rectangle<float> innerBevelBounds, float bevelThickness, Colour baseColour)
//{
//    Rectangle<float> outerBevelBounds (innerBevelBounds.expanded (bevelThickness, bevelThickness));
//    Rectangle<float> centreBevelBounds (innerBevelBounds.expanded (bevelThickness * 0.5, bevelThickness * 0.5));
//    
//    Path pL, pR, pT, pB, pTL, pTR, pBL, pBR;
//    pL.startNewSubPath (centreBevelBounds.getTopLeft());
//    pL.lineTo (centreBevelBounds.getBottomLeft());
//    
//    pR.startNewSubPath (centreBevelBounds.getTopRight());
//    pR.lineTo (centreBevelBounds.getBottomRight());
//    
//    pT.startNewSubPath (centreBevelBounds.getTopLeft());
//    pT.lineTo (centreBevelBounds.getTopRight());
//    
//    pB.startNewSubPath (centreBevelBounds.getBottomLeft());
//    pB.lineTo (centreBevelBounds.getBottomRight());
//    
//    pTL.addTriangle (outerBevelBounds.getX(), outerBevelBounds.getY(),
//                     outerBevelBounds.getX(), innerBevelBounds.getY(),
//                     innerBevelBounds.getX(), innerBevelBounds.getY());
//    
//    pTR.addTriangle (outerBevelBounds.getRight(), outerBevelBounds.getY(),
//                     outerBevelBounds.getRight(), innerBevelBounds.getY(),
//                     innerBevelBounds.getRight(), innerBevelBounds.getY());
//    
//    pBL.addTriangle (outerBevelBounds.getX(), outerBevelBounds.getBottom(),
//                     outerBevelBounds.getX(), innerBevelBounds.getBottom(),
//                     innerBevelBounds.getX(), innerBevelBounds.getBottom());
//    
//    pBR.addTriangle (outerBevelBounds.getRight(), innerBevelBounds.getBottom(),
//                     outerBevelBounds.getRight(), outerBevelBounds.getBottom(),
//                     innerBevelBounds.getRight(), innerBevelBounds.getBottom());
//    
//    g.saveState();
//    
//    g.setColour (baseColour);
//    g.strokePath (pL, PathStrokeType (bevelThickness));
//    g.strokePath (pR, PathStrokeType (bevelThickness));
//    
//    g.setColour (baseColour.darker (0.5f));
//    g.strokePath (pT, PathStrokeType (bevelThickness, PathStrokeType::mitered, PathStrokeType::square));
//    
//    g.setColour (baseColour.brighter (0.5f));
//    g.strokePath (pB, PathStrokeType (bevelThickness, PathStrokeType::mitered, PathStrokeType::square));
//    
//    g.setColour (baseColour);
//    g.fillPath (pTL);
//    g.fillPath (pTR);
//    g.fillPath (pBL);
//    g.fillPath (pBR);
//    
//    g.restoreState();
//}
