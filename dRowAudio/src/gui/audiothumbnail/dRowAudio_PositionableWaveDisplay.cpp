/*
 *  dRowAudio_WaveDisplay.cpp
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "../../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_PositionableWaveDisplay.h"

PositionableWaveDisplay::PositionableWaveDisplay (FilteringAudioFilePlayer *sourceToBeUsed, AudioThumbnailCache *cacheToUse)
:	filePlayer(sourceToBeUsed),
	currentSampleRate(44100.0),
	thumbnailCache(cacheToUse),
	deleteCache(thumbnailCache ? false : true),
	zoomFactor(1.0f),
	firstLoad(true),
	isMouseDown(false),
	interestedInDrag(false)
{
	formatManager = filePlayer->getAudioFormatManager();
	
	// instansiate the cache and the thumbnail
	if (thumbnailCache == 0)
		thumbnailCache = new AudioThumbnailCache(2);
	thumbnailView = new AudioThumbnail(1024, *formatManager, *thumbnailCache);
	
	// register with the file player to recieve update messages
	filePlayer->addListener(this);
}

PositionableWaveDisplay::~PositionableWaveDisplay()
{
	stopTimer(waveformUpdated);
		
	if (!deleteCache)
		thumbnailCache.release();
}

//====================================================================================
void PositionableWaveDisplay::resized()
{
	int w = getWidth();
	int h = getHeight();
		
	if (firstLoad) {
		firstLoad = false;
		if (w == 0)
			w = 1;
		if (h == 0)
			h = 1;
		waveformImage = new Image(Image::RGB, w, h, true);
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
	
	if (interestedInDrag) {
		g.setColour(Colours::darkorange);
		g.drawRect(0, 0, w, h, 2);
	}
}
//====================================================================================
void PositionableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == waveformUpdated)
	{
		const int w = getWidth();
		const int h = getHeight();

		transportLineXCoord = w * oneOverFileLength * filePlayer->getAudioTransportSource()->getCurrentPosition();
		
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

void PositionableWaveDisplay::fileChanged (FilteringAudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
		fileLength = filePlayer->getAudioTransportSource()->getTotalLength() / currentSampleRate;
		oneOverFileLength = 1.0 / fileLength;
		
		File newFile(filePlayer->getFilePath());
		if (newFile.existsAsFile()) {
			FileInputSource* fileInputSource = new FileInputSource (newFile);
			thumbnailView->setSource(fileInputSource);
		}
		else {
			thumbnailView->setSource(0);
		}
		
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
	filePlayer->getAudioTransportSource()->setPosition(position);
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
	filePlayer->getAudioTransportSource()->setPosition(position);
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
	interestedInDrag = true;
	setMouseCursor(MouseCursor::CopyingCursor);
	
	repaint();
}
void PositionableWaveDisplay::fileDragExit (const StringArray &files)
{
	interestedInDrag = false;
	setMouseCursor(MouseCursor::NormalCursor);	

	repaint();
}
void PositionableWaveDisplay::filesDropped (const StringArray &files, int x, int y)
{
	interestedInDrag = false;
	filePlayer->setFile(files[0]);
	setMouseCursor(MouseCursor::NormalCursor);

	repaint();
}

//==============================================================================
bool PositionableWaveDisplay::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
	if (dragSourceDetails.description.toString().startsWith("<ITEMS>")) {
		return true;
	}
	
	return false;	
}

void PositionableWaveDisplay::itemDragEnter (const SourceDetails& dragSourceDetails)
{
	interestedInDrag = true;
	repaint();
}

void PositionableWaveDisplay::itemDragExit (const SourceDetails& dragSourceDetails)
{
	interestedInDrag = false;
	repaint();
}

void PositionableWaveDisplay::itemDropped (const SourceDetails& dragSourceDetails)
{
	ScopedPointer<XmlElement> newTracks (XmlDocument::parse(dragSourceDetails.description.toString()));
	
	if (newTracks->getNumChildElements() > 0) {
		File newFile(newTracks->getChildElement(0)->getStringAttribute("Location"));
		
		if (newFile.existsAsFile()) {
			filePlayer->setLibraryEntry(ValueTree::fromXml(*newTracks->getChildElement(0)));
			filePlayer->setFile(newFile.getFullPathName());
		}
	}
	
	interestedInDrag = false;
	repaint();
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
		
		if (filePlayer->getFileName().isNotEmpty())
		{
			g.setColour(Colours::lightgreen);
			thumbnailView->drawChannel(g, Rectangle<int> (0, 0, w, h),
									   0.0, fileLength,
									   0, 1.0f);
		}
		
		repaint();
	}	
}

//==============================================================================	

END_DROWAUDIO_NAMESPACE