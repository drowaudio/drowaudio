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

#ifndef __DROWAUDIO_DEFAULTCOLOURS_H__
#define __DROWAUDIO_DEFAULTCOLOURS_H__

//==============================================================================
/*
    This class is used internally by the module to handle it's own colourIds and
    maintain compatibility with the normal JUCE setColour etc. methods. You
    should never have to use this yourself.
 */
class DefaultColours
{
public:
    //==============================================================================
    DefaultColours()
    {
        fillDefaultColours();
    }

    Colour findColour (const Component& source, const int colourId) const noexcept
    {
        if (source.isColourSpecified (colourId))
            return source.findColour (colourId);
        else if (source.getLookAndFeel().isColourSpecified (colourId))
            return source.getLookAndFeel().findColour (colourId);
        else
            return colourIds.contains (colourId) ? colours[colourIds.indexOf (colourId)]
                                                    : Colours::black;
    }
    
private:
    //==============================================================================
    Array <int> colourIds;
    Array <Colour> colours;
    
    //==============================================================================
    void fillDefaultColours() noexcept
    {
        static const uint32 standardColours[] =
        {
            MusicLibraryTable::backgroundColourId,                      Colour::greyLevel (0.2f).getARGB(),
            MusicLibraryTable::unfocusedBackgroundColourId,             Colour::greyLevel (0.2f).getARGB(),
            MusicLibraryTable::selectedBackgroundColourId,              0xffff8c00,//Colour (Colours::darkorange).getARGB(),
            MusicLibraryTable::selectedUnfocusedBackgroundColourId,     Colour::greyLevel (0.6f).getARGB(),
            MusicLibraryTable::textColourId,                            Colour::greyLevel (0.9f).getARGB(),
            MusicLibraryTable::selectedTextColourId,                    Colour::greyLevel (0.2f).getARGB(),
            MusicLibraryTable::unfocusedTextColourId,                   Colour::greyLevel (0.9f).getARGB(),
            MusicLibraryTable::selectedUnfocusedTextColourId,           Colour::greyLevel (0.9f).getARGB(),
            MusicLibraryTable::outlineColourId,                         Colour::greyLevel (0.9f).withAlpha (0.2f).getARGB(),
            MusicLibraryTable::selectedOutlineColourId,                 Colour::greyLevel (0.9f).withAlpha (0.2f).getARGB(),
            MusicLibraryTable::unfocusedOutlineColourId,                Colour::greyLevel (0.9f).withAlpha (0.2f).getARGB(),
            MusicLibraryTable::selectedUnfocusedOutlineColourId,        Colour::greyLevel (0.9f).withAlpha (0.2f).getARGB()
        };
        
        for (int i = 0; i < numElementsInArray (standardColours); i += 2)
        {
            colourIds.add ((int) standardColours [i]);
            colours.add (Colour ((uint32) standardColours [i + 1]));
        }
    }
};

static const DefaultColours defaultColours;


#endif // __DROWAUDIO_DEFAULTCOLOURS_H__
