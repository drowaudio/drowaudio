/*
 *  FilmstripSlider.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 19/07/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
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
    setTextBoxStyle(NoTextBox, false, getWidth(), getHeight());
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

void FilmstripSlider::paint(Graphics& g)
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