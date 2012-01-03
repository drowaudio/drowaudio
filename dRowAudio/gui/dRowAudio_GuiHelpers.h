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

#ifndef __DROWAUDIO_GUIHELPERS_H__
#define __DROWAUDIO_GUIHELPERS_H__

namespace GuiHelpers
{
    /** Creates a base colour for a component based on the current keyboard
        and mouse interactivity.
     */
    static Colour createBaseColour (const Colour& colour,
                                    const bool hasKeyboardFocus,
                                    const bool isMouseOver,
                                    const bool isButtonDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const Colour baseColour (colour.withMultipliedSaturation (sat));
        
        if (isButtonDown)
            return baseColour.contrasting (0.2f);
        else if (isMouseOver)
            return baseColour.contrasting (0.1f);
        
        return baseColour;
    }
    
    /** Draws a square bevel around a given rectange.
        This is useful for insetting components and givin them a border.
     */
    static void drawBevel (Graphics& g, juce::Rectangle<float> innerBevelBounds,
                           float bevelThickness, const Colour& baseColour)
    {
       juce::Rectangle<float> outerBevelBounds (innerBevelBounds.expanded (bevelThickness, bevelThickness));
       juce::Rectangle<float> centreBevelBounds (innerBevelBounds.expanded (bevelThickness * 0.5, bevelThickness * 0.5));
        
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
}

#endif // __DROWAUDIO_GUIHELPERS_H__