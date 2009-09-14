/*
 *  FilmstripSlider.h
 *  dRowAudio
 *
 *  Created by David Rowland on 19/07/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _FILMSTRIPSLIDER_H_
#define _FILMSTRIPSLIDER_H_

#include <juce/juce.h>

/**
	A slider class that takes a film strip image to display as its graphics.
 */
class FilmstripSlider : public Slider
{
public:
	/**
		Constructor.
		You need to supply the image file and the number of frames it represents for correct image display.
		Optional arguments show or hide the default number box and if the filmstrip image is horizontal or vertical.
	 */
	FilmstripSlider(String &componentName, File const& image, const int numFrames, const bool showTextBox =false, const bool stripIsHorizontal =true);
	
	/**
		Constructor.
		Use this if you want to use the same image for many sliders in order to save memory.
		Remember to delete the Image passed to the slider yourself.
	 */
	FilmstripSlider (String &componentName, Image *image, const int numFrames, const bool showTextBox =false, const bool stripIsHorizontal =true);

	///	Destructor.
	~FilmstripSlider();

	/// Returns the width of one frame of the filmstrip in pixels.
	int getFrameWidth() const  { return filmStripImage ? frameWidth  : 100; }
	
	/// Returns the height of one frame of the filmstrip in pixels.
	int getFrameHeight() const { return filmStripImage ? frameHeight : 24;  }
		
	/// Returns ture is the text box is visible.
	bool isTextBoxVisible() const { return showTextBox_; }

	/// @interal
	void paint(Graphics& g);

private:
	Image* filmStripImage;
	const bool fileConstructorUsed;
	bool showTextBox_;
	const bool isHorizontal_;
	const int numFrames_;
	int frameWidth, frameHeight;
};

#endif //_FILMSTRIPSLIDER_H_