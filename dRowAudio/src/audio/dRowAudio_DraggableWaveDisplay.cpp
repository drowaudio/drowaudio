/*
 *  dRowAudio_DraggableWaveDisplay.cpp
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_DraggableWaveDisplay.h"

DraggableWaveDisplay::DraggableWaveDisplay(AudioFilePlayer* sourceToBeUsed, double sampleRate)
	:	filePlayer(sourceToBeUsed),
		currentSampleRate(sampleRate),
		zoomFactor(1.0f)
{
	// set up the format manager to read basic formats
	formatManager = new AudioFormatManager();
	formatManager->registerBasicFormats();
	
	// instansiate the cache and the thumbnail
	thumbnailCache = new AudioThumbnailCache(2);
	thumbnailViewLow = new AudioThumbnail(512, *formatManager, *thumbnailCache);
	
	// register with the file player to recieve update messages
	filePlayer->addChangeListener(this);
}

DraggableWaveDisplay::~DraggableWaveDisplay()
{
	deleteAndZero(formatManager);
	deleteAndZero(thumbnailCache);
	deleteAndZero(thumbnailViewLow);
	
	filePlayer->removeChangeListener(this);
}

//====================================================================================
void DraggableWaveDisplay::resized()
{
	currentWidth = getWidth();
	currentHeight = getHeight();
}

void DraggableWaveDisplay::paint(Graphics &g)
{
	double centreTime = 0.5 * zoomFactor * fileLength;
	
	g.fillAll(Colours::black);
	
	g.setColour(Colours::lightgreen);
	thumbnailViewLow->drawChannel(g, 0, 0, currentWidth, currentHeight*0.5f,
								  0.0+currentPos-centreTime, (zoomFactor*fileLength+currentPos)-centreTime,
								  0, 1.0f);
	thumbnailViewLow->drawChannel(g, 0, currentHeight*0.5f, currentWidth, currentHeight*0.5f,
								  0.0+currentPos-centreTime, (zoomFactor*fileLength+currentPos)-centreTime,
								  1, 1.0f);
	
	g.setColour(Colours::white);
	g.drawLine(currentWidth * 0.5 - 1, 0, currentWidth * 0.5 - 1, currentHeight, 2);	
}
//====================================================================================
void DraggableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == waveformUpdated)
	{
		currentPos = filePlayer->getCurrentPosition();
		repaint();
	}
	else if (timerId == waveformMoved)
	{
		if (isMouseDown)
		{
			int y;
			lastMouseX = currentMouseX;
			getMouseXYRelative(currentMouseX, y);
			
			currentXDrag = currentMouseX - lastMouseX;
			
			double position = currentXDrag * currentXScale;
						
			filePlayer->setPosition(currentPos-position);
			
			repaint();
		}
	}
	else if (timerId == waveformLoading)
	{
		repaint();
		
		if(thumbnailViewLow->isFullyLoaded())
			stopTimer(waveformLoading);
	}
}

void DraggableWaveDisplay::changeListenerCallback(void* changedObject)
{
	if (changedObject == filePlayer)
	{
		fileLength = filePlayer->getTotalLength() / currentSampleRate;
	
		File newFile(((AudioFilePlayer*)changedObject)->getFile());
		FileInputSource* fileInputSource = new FileInputSource (newFile);
		thumbnailViewLow->setSource(fileInputSource);
		
		startTimer(waveformLoading, 25);
		startTimer(waveformUpdated, 40);
	}
}
//====================================================================================
void DraggableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	zoomFactor = newZoomFactor;
	currentXScale = ( (zoomFactor)*fileLength ) / currentWidth;
	
	repaint();
}

//==============================================================================
void DraggableWaveDisplay::mouseDown(const MouseEvent &e)
{
	// update scale
	currentXScale = ( (zoomFactor)*fileLength ) / currentWidth;
	
	lastMouseX = e.x;
	currentMouseX = e.x;
	isMouseDown = true;
	
	setMouseCursor(MouseCursor::DraggingHandCursor);
	
	startTimer(waveformMoved, 40);
}

void DraggableWaveDisplay::mouseUp(const MouseEvent &e)
{
	isMouseDown = false;
	
	setMouseCursor(MouseCursor::NormalCursor);
	
	stopTimer(waveformMoved);
}

void DraggableWaveDisplay::mouseDrag(const MouseEvent &e)
{
//	currentMouseX = e.x;
}
//==============================================================================
bool DraggableWaveDisplay::isInterestedInFileDrag (const StringArray &files)
{
	if (files[0].containsIgnoreCase(T(".wav"))
		|| files[0].containsIgnoreCase(T(".aif")))
		return true;
	else
		return false;
}
void DraggableWaveDisplay::fileDragEnter (const StringArray &files, int x, int y)
{
		setMouseCursor(MouseCursor::CopyingCursor);
}
void DraggableWaveDisplay::fileDragExit (const StringArray &files)
{
	setMouseCursor(MouseCursor::NormalCursor);	
}
void DraggableWaveDisplay::filesDropped (const StringArray &files, int x, int y)
{
	filePlayer->setFile(files[0]);
	setMouseCursor(MouseCursor::NormalCursor);
}
//==============================================================================	


