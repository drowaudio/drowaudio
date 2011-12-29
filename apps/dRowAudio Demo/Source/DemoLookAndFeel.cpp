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

void DemoLookAndFeel::drawBevel (Graphics& g, Rectangle<int> innerBevelBounds, float bevelThickness, Colour baseColour)
{
    Rectangle<float> innerBevelBoundsFloat (innerBevelBounds.getX(), innerBevelBounds.getY(), innerBevelBounds.getWidth(), innerBevelBounds.getHeight());
    drawBevel (g, innerBevelBoundsFloat, bevelThickness, baseColour);
}

void DemoLookAndFeel::drawBevel (Graphics& g, Rectangle<float> innerBevelBounds, float bevelThickness, Colour baseColour)
{
    Rectangle<float> outerBevelBounds (innerBevelBounds.expanded (bevelThickness, bevelThickness));
    Rectangle<float> centreBevelBounds (innerBevelBounds.expanded (bevelThickness * 0.5, bevelThickness * 0.5));
    
    Path pL, pR, pT, pB, pTL, pTR, pBL, pBR;
    pL.startNewSubPath (centreBevelBounds.getTopLeft());
    pL.lineTo (centreBevelBounds.getBottomLeft());
    
    pR.startNewSubPath (centreBevelBounds.getTopRight());
    pR.lineTo (centreBevelBounds.getBottomRight());
    
    pT.startNewSubPath (centreBevelBounds.getTopLeft());
    pT.lineTo (centreBevelBounds.getTopRight());
    
    pB.startNewSubPath (centreBevelBounds.getBottomLeft());
    pB.lineTo (centreBevelBounds.getBottomRight());
    
    pTL.addTriangle (outerBevelBounds.getX(), outerBevelBounds.getY(),
                     outerBevelBounds.getX(), innerBevelBounds.getY(),
                     innerBevelBounds.getX(), innerBevelBounds.getY());
    
    pTR.addTriangle (outerBevelBounds.getRight(), outerBevelBounds.getY(),
                     outerBevelBounds.getRight(), innerBevelBounds.getY(),
                     innerBevelBounds.getRight(), innerBevelBounds.getY());
    
    pBL.addTriangle (outerBevelBounds.getX(), outerBevelBounds.getBottom(),
                     outerBevelBounds.getX(), innerBevelBounds.getBottom(),
                     innerBevelBounds.getX(), innerBevelBounds.getBottom());
    
    pBR.addTriangle (outerBevelBounds.getRight(), innerBevelBounds.getBottom(),
                     outerBevelBounds.getRight(), outerBevelBounds.getBottom(),
                     innerBevelBounds.getRight(), innerBevelBounds.getBottom());
    
    g.saveState();
    
    g.setColour (baseColour);
    g.strokePath (pL, PathStrokeType (bevelThickness));
    g.strokePath (pR, PathStrokeType (bevelThickness));
    
    g.setColour (baseColour.darker (0.5f));
    g.strokePath (pT, PathStrokeType (bevelThickness, PathStrokeType::mitered, PathStrokeType::square));
    
    g.setColour (baseColour.brighter (0.5f));
    g.strokePath (pB, PathStrokeType (bevelThickness, PathStrokeType::mitered, PathStrokeType::square));
    
    g.setColour (baseColour);
    g.fillPath (pTL);
    g.fillPath (pTR);
    g.fillPath (pBL);
    g.fillPath (pBR);
    
    g.restoreState();
}
