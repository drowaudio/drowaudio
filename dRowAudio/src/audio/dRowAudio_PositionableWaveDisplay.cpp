/*
 *  dRowAudio_WaveDisplay.cpp
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_PositionableWaveDisplay.h"

PositionableWaveDisplay::PositionableWaveDisplay (FilteringAudioFilePlayer *sourceToBeUsed, AudioThumbnailCache *cacheToUse)
:	filePlayer(sourceToBeUsed),
	currentSampleRate(44100.0),
	thumbnailCache(cacheToUse),
	deleteCache(thumbnailCache ? false : true),
	zoomFactor(1.0f),
	firstLoad(true)
{
	formatManager = filePlayer->getAudioFormatManager();
	
	// instansiate the cache and the thumbnail
	if (thumbnailCache == 0)
		thumbnailCache = new AudioThumbnailCache(2);
	thumbnailView = new AudioThumbnail(5120, *formatManager, *thumbnailCache);
	
	// register with the file player to recieve update messages
	filePlayer->addChangeListener(this);
}

PositionableWaveDisplay::~PositionableWaveDisplay()
{
	stopTimer(waveformUpdated);
	
	filePlayer->removeChangeListener(this);
	
	if (!deleteCache)
		thumbnailCache.release();
}

//====================================================================================
void PositionableWaveDisplay::resized()
{
	const int w = getWidth();
	const int h = getHeight();
		
	if (firstLoad) {
		firstLoad = false;
		waveformImage = new Image(Image::RGB, getWidth(), getHeight(), true);
		refreshWaveform();
	}
	
	startTimer(waveformResizing, 500);
}

void PositionableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();

	g.drawImage(*waveformImage, 0, 0, w, h,
				0, 0, waveformImage->getWidth(), waveformImage->getHeight());
	
	g.setColour(Colours::black);	
	g.drawVerticalLine(transportLineXCoord.getCurrent() - 1, 0, h);
	g.drawVerticalLine(transportLineXCoord.getCurrent() + 1, 0, h);

	g.setColour(Colours::white);
	g.drawVerticalLine(transportLineXCoord.getCurrent(), 0, h);		
}
//====================================================================================
void PositionableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == waveformUpdated)
	{
		const int w = getWidth();
		const int h = getHeight();

		transportLineXCoord = w * oneOverFileLength * filePlayer->getCurrentPosition();
		
		// if the line has moved repaint the old and new positions of it
		if (!transportLineXCoord.areEqual())
		{
			repaint(transportLineXCoord.getPrevious() - 2, 0, 5, h);
			repaint(transportLineXCoord.getCurrent() - 2, 0, 5, h);
		}
	}
	else if (timerId == waveformLoading)
	{		
		if(thumbnailView->isFullyLoaded())
			stopTimer(waveformLoading);
		
		refreshWaveform();	
	}
	else if (timerId == waveformResizing)
	{
		waveformImage = new Image(Image::RGB, getWidth(), getHeight(), true);
		refreshWaveform();
		stopTimer(waveformResizing);
	}

}

void PositionableWaveDisplay::changeListenerCallback(ChangeBroadcaster* changedObject)
{
	if (changedObject == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
		fileLength = filePlayer->getTotalLength() / currentSampleRate;
		oneOverFileLength = 1.0 / fileLength;
	
		File newFile(((FilteringAudioFilePlayer*)changedObject)->getFilePath());
		FileInputSource* fileInputSource = new FileInputSource (newFile);
		thumbnailView->setSource(fileInputSource);
		
		startTimer(waveformLoading, 25);
		startTimer(waveformUpdated, 40);
	}
}
//====================================================================================
void PositionableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	const int w = getWidth();

	zoomFactor = newZoomFactor;
	currentXScale = ( (zoomFactor)*fileLength ) / w;
	
	repaint();
}

//==============================================================================
void PositionableWaveDisplay::mouseDown(const MouseEvent &e)
{
	// update scale
	const int w = getWidth();

	currentXScale = ( (zoomFactor)*fileLength ) / w;
	
	currentMouseX = e.x;
	isMouseDown = true;
	
	setMouseCursor(MouseCursor::IBeamCursor);
	
	double position = currentXScale * currentMouseX;
	filePlayer->setPosition(position);
	repaint();		
}

void PositionableWaveDisplay::mouseUp(const MouseEvent &e)
{
	isMouseDown = false;
	
	setMouseCursor(MouseCursor::NormalCursor);
}

void PositionableWaveDisplay::mouseDrag(const MouseEvent &e)
{
	currentMouseX = e.x;
	
	double position = currentXScale * currentMouseX;
	filePlayer->setPosition(position);
}
//==============================================================================
bool PositionableWaveDisplay::isInterestedInFileDrag (const StringArray &files)
{
	File droppedFile(files[0]);
	if (matchesAudioWildcard(droppedFile.getFileExtension(), formatManager->getWildcardForAllFormats(), true))
		return true;
	else
		return false;
}
void PositionableWaveDisplay::fileDragEnter (const StringArray &files, int x, int y)
{
	setMouseCursor(MouseCursor::CopyingCursor);
}
void PositionableWaveDisplay::fileDragExit (const StringArray &files)
{
	setMouseCursor(MouseCursor::NormalCursor);	
}
void PositionableWaveDisplay::filesDropped (const StringArray &files, int x, int y)
{
	filePlayer->setFile(files[0]);
	setMouseCursor(MouseCursor::NormalCursor);
}
//==============================================================================
bool PositionableWaveDisplay::isInterestedInDragSource (const String &sourceDescription, Component *sourceComponent)
{
	if (sourceDescription.startsWith("<ITEMS>")) {
		return true;
	}
	
	return false;	
}

void PositionableWaveDisplay::itemDragExit (const String &sourceDescription, Component *sourceComponent)
{
}

void PositionableWaveDisplay::itemDropped (const String &sourceDescription, Component *sourceComponent, int x, int y)
{
	ScopedPointer<XmlElement> newTracks (XmlDocument::parse(sourceDescription));
	
	if (newTracks->getNumChildElements() > 0) {
		File newFile(newTracks->getChildElement(0)->getStringAttribute("Location"));
		
		if (newFile.existsAsFile()) {
			filePlayer->setFile(newFile.getFullPathName());
		}
	}
}


//==============================================================================	
void PositionableWaveDisplay::refreshWaveform()
{
	if(waveformImage != 0)
	{
		const int w = getWidth();
		const int h = getHeight();

		Graphics g(*waveformImage);
		waveformImage->clear(waveformImage->getBounds());
		
		g.fillAll(Colours::black);
		g.setColour(Colours::lightgreen);
		thumbnailView->drawChannel(g, Rectangle<int> (0, 0, w, h),
								   0.0, fileLength,
								   0, 1.0f);
		
		repaint();
	}	
}

//==============================================================================	

END_DROWAUDIO_NAMESPACE