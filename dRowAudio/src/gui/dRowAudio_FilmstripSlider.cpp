/*
 *  FilmstripSlider.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 19/07/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include <dRowAudio/dRowAudio.h>

FilmstripSlider::FilmstripSlider (String &componentName, File const& image, const int numFrames, const bool showTextBox, const bool stripIsHorizontal)
	:   Slider(componentName),
		filmStripImage(image.exists() ? ImageFileFormat::loadFrom(image) : 0),
		numFrames_(numFrames),
		showTextBox_(showTextBox),
		isHorizontal_(stripIsHorizontal)
{
	if(filmStripImage)
	{
		if (!showTextBox)
			setTextBoxStyle(NoTextBox, 0, 0, 0);
		else
			setTextBoxStyle(Slider::TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
		
		if(isHorizontal_)
		{
			frameHeight = filmStripImage->getHeight();
			frameWidth = filmStripImage->getWidth() / numFrames_;
		}
		else
		{
			frameHeight = filmStripImage->getHeight() / numFrames_;
			frameWidth = filmStripImage->getWidth();
		}
	}
}

FilmstripSlider::~FilmstripSlider()
{
	delete filmStripImage;
}

void FilmstripSlider::paint(Graphics& g)
{
	if(filmStripImage)
	{
		int value = (getValue() - getMinimum()) / (getMaximum() - getMinimum()) * (numFrames_ - 1);

		int imageHeight; 
		int imageWidth;
		if (!showTextBox_) {
			imageWidth = getWidth();
			imageHeight = getHeight();
		}
		else {
			imageWidth = getWidth() - getTextBoxHeight();
			imageHeight = getHeight() - getTextBoxHeight();
		}
		
		if(isHorizontal_)
			g.drawImage(filmStripImage, (getWidth() - imageWidth) * 0.5, 0, imageWidth, imageHeight,
						value * frameWidth, 0, frameWidth, frameHeight);
		else
			g.drawImage(filmStripImage, 0, 0, imageWidth, imageHeight,
						0, value * frameHeight, frameWidth, frameHeight);
	}
}