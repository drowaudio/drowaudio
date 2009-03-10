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
		playheadPos(0.25f),
		zoomFactor(1.0f),
		isDraggable(true)
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
	double centreTime = playheadPos * zoomFactor * fileLength;
	
	g.fillAll(Colours::black);
	
	g.setColour(Colours::lightgreen);
	thumbnailViewLow->drawChannel(g, 0, 0, currentWidth, currentHeight*0.5f,
								  0.0+currentPos-centreTime, (zoomFactor*fileLength+currentPos)-centreTime,
								  0, 1.0f);
	thumbnailViewLow->drawChannel(g, 0, currentHeight*0.5f, currentWidth, currentHeight*0.5f,
								  0.0+currentPos-centreTime, (zoomFactor*fileLength+currentPos)-centreTime,
								  1, 1.0f);
	
	g.setColour (Colours::black);
	g.drawVerticalLine(currentWidth * playheadPos - 1, 0, currentHeight);
	g.drawVerticalLine(currentWidth * playheadPos + 1, 0, currentHeight);

	g.setColour(Colours::white);
	g.drawVerticalLine(currentWidth * playheadPos, 0, currentHeight);
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
			
			double position = currentPos - (currentXDrag * currentXScale);
			// limit position to the bounds of the file
			if (position < 0.0)
				position = 0.0;
			else if (position > fileLength)
				position = fileLength;
						
			filePlayer->setPosition(position);
			
			if (currentXDrag == 0) {
				if(filePlayer->isPlaying())
					filePlayer->stop();
			}
			else
				if(!filePlayer->isPlaying())
					filePlayer->start();
			
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
void DraggableWaveDisplay::setSampleRate (double newSampleRate)
{
	currentSampleRate = newSampleRate;
	
	fileLength = filePlayer->getTotalLength() / currentSampleRate;
}

void DraggableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	zoomFactor = newZoomFactor;
	currentXScale = (zoomFactor * fileLength) / currentWidth;
	
	repaint();
}

void DraggableWaveDisplay::setPlayheadPosition(float newPlayheadPosition)
{
	jlimit(0.0f, 1.0f, newPlayheadPosition);
	playheadPos = newPlayheadPosition;
	
	repaint();
}

void DraggableWaveDisplay::setDraggable (bool isWaveformDraggable)
{
	isDraggable = isWaveformDraggable;
}
bool DraggableWaveDisplay::getDraggable ()
{
	return isDraggable;
}
//==============================================================================
void DraggableWaveDisplay::mouseDown(const MouseEvent &e)
{
	// update scale
	currentXScale = (zoomFactor * fileLength) / currentWidth;
	
	lastMouseX = e.x;
	currentMouseX = e.x;
	isMouseDown = true;
	
	if (isDraggable)
	{
		if (filePlayer->isPlaying())
			shouldBePlaying = true;
		else
			shouldBePlaying = false;
		
		setMouseCursor(MouseCursor::DraggingHandCursor);
		enableUnboundedMouseMovement(true, true);
		
		startTimer(waveformMoved, 40);
	}
}

void DraggableWaveDisplay::mouseUp(const MouseEvent &e)
{
	isMouseDown = false;
	
	if (isDraggable)
	{
		if (shouldBePlaying && !filePlayer->isPlaying())
			filePlayer->start();
		else if ( !shouldBePlaying && filePlayer->isPlaying() )
			filePlayer->stop();
		
		setMouseCursor(MouseCursor::NormalCursor);
		
		stopTimer(waveformMoved);
	}
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


