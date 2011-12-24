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

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_FilmstripSlider.h"

FilmstripSlider::FilmstripSlider (const String &componentName)
    : Slider(componentName),
      numFrames (0),
      isHorizontal (true),
      frameWidth (0),
      frameHeight (0)
{
    setTextBoxStyle (NoTextBox, false, getWidth(), getHeight());
}

void FilmstripSlider::setImage (Image image, int numFrames_, bool isHorizontal_)
{
    filmStripImage = image;
    
	if(filmStripImage.isValid())
	{
        numFrames = numFrames_;
        isHorizontal = isHorizontal_;
        
        if(isHorizontal)
        {
            frameHeight = filmStripImage.getHeight();
            frameWidth = filmStripImage.getWidth() / numFrames;
        }
        else
        {
            frameHeight = filmStripImage.getHeight() / numFrames;
            frameWidth = filmStripImage.getWidth();
        }
	}
    else
    {
        numFrames = 0;
    }
    
    repaint();
}

FilmstripSlider::~FilmstripSlider()
{
}

void FilmstripSlider::paint (Graphics& g)
{
	if (filmStripImage.isValid())
	{
		int value = (int) ((getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFrames - 1));
        
		int imageX = 0, imageY = 0, imageWidth = getWidth(), imageHeight = getHeight();
        
		if (getTextBoxPosition() == TextBoxBelow)
        {
			imageWidth = getWidth() - getTextBoxHeight();
			imageHeight = getHeight() - getTextBoxHeight();
            
            imageX = (getWidth() - imageWidth) * 0.5;
		}
		else if (getTextBoxPosition() == TextBoxLeft
                 || getTextBoxPosition() == TextBoxRight)
        {
			imageWidth = getWidth() - getTextBoxWidth();
			imageHeight = getHeight() - getTextBoxWidth();
            
            imageX = (getWidth() - imageWidth);
            imageY = (getHeight() - imageHeight) * 0.5;
		}
        else
        {
            imageX = (getWidth() - imageWidth) * 0.5;
        }
        
		
		if(isHorizontal)
        {
			g.drawImage(filmStripImage,
                        imageX, imageY,
                        imageWidth, imageHeight,
						value * frameWidth, 0, frameWidth, frameHeight);
        }
		else
        {
			g.drawImage(filmStripImage,
                        imageX, imageY, imageWidth, imageHeight,
						0, value * frameHeight, frameWidth, frameHeight);
        }
	}
}

END_DROWAUDIO_NAMESPACE