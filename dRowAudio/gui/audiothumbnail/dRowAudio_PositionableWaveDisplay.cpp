/*
 *  dRowAudio_PositionableWaveDisplay.cpp
 *
 *  Created by David Rowland on 02/11/2011.
 *  Copyright 2011 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

#include "dRowAudio_PositionableWaveDisplay.h"

PositionableWaveDisplay::PositionableWaveDisplay (AudioThumbnailImage& sourceToBeUsed)
    : currentSampleRate     (44100.0),
      audioThumbnailImage   (sourceToBeUsed),
      audioFilePlayer       (audioThumbnailImage.getAudioFilePlayer())
{
    audioThumbnailImage.addListener (this);
}

PositionableWaveDisplay::~PositionableWaveDisplay()
{
    audioThumbnailImage.removeListener (this);
	stopTimer (waveformUpdated);
}

//====================================================================================
void PositionableWaveDisplay::resized()
{
    cursorImage = Image (Image::RGB, 3, getHeight(), true);
    Graphics g (cursorImage);
    g.setColour (Colours::white);
	g.drawVerticalLine (1, 0, cursorImage.getHeight());
    
    if (audioThumbnailImage.hasFinishedLoading()) 
    {
        cachedImage = audioThumbnailImage.getImage();
        cachedImage.duplicateIfShared();
        cachedImage = cachedImage.rescaled (getWidth(), getHeight());
    }
}

void PositionableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();

    g.setColour (Colours::black);	
    g.fillAll();

    Image thumbnail (cachedImage);
	g.drawImage (thumbnail,
                 0, 0, w, h,
                 0, 0, thumbnail.getWidth(), thumbnail.getHeight(), 
                 false);

    g.drawImageAt (cursorImage, transportLineXCoord.getCurrent() - 1, 0);
	
//	g.drawVerticalLine (transportLineXCoord.getCurrent() - 1, 0, h);
//	g.drawVerticalLine (transportLineXCoord.getCurrent() + 1, 0, h);
//
//	g.setColour (Colours::white);
//	g.drawVerticalLine (transportLineXCoord.getCurrent(), 0, h);
}

//====================================================================================
void PositionableWaveDisplay::timerCallback (const int timerId)
{
	if (timerId == waveformUpdated)
	{
		const int w = getWidth();
		const int h = getHeight();

		transportLineXCoord = w * oneOverFileLength * audioFilePlayer->getCurrentPosition();
		
		// if the line has moved repaint the old and new positions of it
		if (! transportLineXCoord.areEqual())
		{
			repaint (transportLineXCoord.getPrevious() - 2, 0, 5, h);
			repaint (transportLineXCoord.getCurrent() - 2, 0, 5, h);
		}
	}
}

//====================================================================================
void PositionableWaveDisplay::imageChanged (AudioThumbnailImage* changedAudioThumbnailImage)
{
	if (changedAudioThumbnailImage == &audioThumbnailImage)
	{
        cachedImage = Image::null;

		currentSampleRate = audioFilePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
		fileLength = audioFilePlayer->getTotalLength() / currentSampleRate;
		oneOverFileLength = 1.0 / fileLength;
		
		startTimer (waveformUpdated, 40);		
	}
}

void PositionableWaveDisplay::imageUpdated (AudioThumbnailImage* changedAudioThumbnailImage)
{
    cachedImage = audioThumbnailImage.getImage();
    repaint();
}

void PositionableWaveDisplay::imageFinished (AudioThumbnailImage* changedAudioThumbnailImage)
{
    resized();
}

//==============================================================================
void PositionableWaveDisplay::mouseDown (const MouseEvent &e)
{
	currentXScale = fileLength / getWidth();
	currentMouseX = e.x;
	
	setMouseCursor (MouseCursor::IBeamCursor);
	
	double position = currentXScale * currentMouseX;
	audioFilePlayer->setPositionIgnoringLoop (position);

    repaint();
}

void PositionableWaveDisplay::mouseUp (const MouseEvent &e)
{
	setMouseCursor (MouseCursor::NormalCursor);
}

void PositionableWaveDisplay::mouseDrag (const MouseEvent &e)
{
	currentMouseX = e.x;
	
	double position = currentXScale * currentMouseX;
	audioFilePlayer->setPositionIgnoringLoop (position);
}

END_JUCE_NAMESPACE