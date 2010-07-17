/*
 *  FilmstripSlider.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 19/07/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_FilmstripSlider.h"

FilmstripSlider::FilmstripSlider (String &componentName, File const& image, const int numFrames, const bool showTextBox, const bool stripIsHorizontal)
	:   Slider(componentName),
		filmStripImage(ImageFileFormat::loadFrom(image)),
		fileConstructorUsed(true),
		showTextBox_(showTextBox),
		isHorizontal_(stripIsHorizontal),
		numFrames_(numFrames)
{
	if (!showTextBox)
		setTextBoxStyle(NoTextBox, 0, 0, 0);
	else
		setTextBoxStyle(Slider::TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
		
	if(filmStripImage.isValid())
	{
		if(isHorizontal_)
		{
			frameHeight = filmStripImage.getHeight();
			frameWidth = filmStripImage.getWidth() / numFrames_;
		}
		else
		{
			frameHeight = filmStripImage.getHeight() / numFrames_;
			frameWidth = filmStripImage.getWidth();
		}
	}
}

FilmstripSlider::FilmstripSlider (String &componentName, Image image, const int numFrames, const bool showTextBox, const bool stripIsHorizontal)
:   Slider(componentName),
	filmStripImage(image),
	fileConstructorUsed(false),
	showTextBox_(showTextBox),
	isHorizontal_(stripIsHorizontal),
	numFrames_(numFrames)
{
	if (!showTextBox)
		setTextBoxStyle(NoTextBox, 0, 0, 0);
	else
		setTextBoxStyle(Slider::TextBoxBelow, false, getTextBoxWidth(), getTextBoxHeight());
		
	if(filmStripImage.isValid())
	{
		if(isHorizontal_)
		{
			frameHeight = filmStripImage.getHeight();
			frameWidth = filmStripImage.getWidth() / numFrames_;
		}
		else
		{
			frameHeight = filmStripImage.getHeight() / numFrames_;
			frameWidth = filmStripImage.getWidth();
		}
	}
}

FilmstripSlider::~FilmstripSlider()
{
}

void FilmstripSlider::paint(Graphics& g)
{
	if(filmStripImage.isValid())
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
	else
		Slider::paint(g);
}