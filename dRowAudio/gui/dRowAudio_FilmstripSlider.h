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

#ifndef _FILMSTRIPSLIDER_H_
#define _FILMSTRIPSLIDER_H_

#include "../core/dRowAudio_StandardHeader.h"

/**
	A slider class that takes a film strip image to display as its graphics.
 */
class FilmstripSlider : public Slider
{
public:
	/**
     Creates an empty slider.
     Use setImage to let the slider know what to use as its display.
	 */
	FilmstripSlider(const String& componentName = String::empty);
    
	/**	Destructor.
     */
	~FilmstripSlider();
    
    /** Sets the image to use, you need to supply the number of frames within the image
     and if the image is horizontal or vertical.
     */
    void setImage (Image image, int numFrames, bool isHorizontal = true);
    
	/** Returns the width of one frame of the filmstrip in pixels.
     */
	int getFrameWidth() const       { return frameWidth;    }
	
	/** Returns the height of one frame of the filmstrip in pixels.
     */
	int getFrameHeight() const      { return frameHeight;   }
    
	/** @interal
     */
	void paint (Graphics& g);
    
private:
    
	Image filmStripImage;
	int numFrames;
	bool isHorizontal;
	int frameWidth, frameHeight;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilmstripSlider);
};


#endif //_FILMSTRIPSLIDER_H_