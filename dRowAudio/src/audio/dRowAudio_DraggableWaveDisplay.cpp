/*
 *  dRowAudio_DraggableWaveDisplay.cpp
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */
#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_DraggableWaveDisplay.h"

DraggableWaveDisplay::DraggableWaveDisplay(FilteringAudioFilePlayer* sourceToBeUsed, AudioThumbnailCache *cacheToUse)
:	filePlayer(sourceToBeUsed),
	currentSampleRate(44100),
	timePerPixel(1.0),
	zoomFactor(1.0f),
	playheadPos(0.5f),
	sourceSamplesPerThumbSample(2048),
	thumbnailCache(cacheToUse),
	deleteCache(thumbnailCache ? false : true),
	isDraggable(true)
{
	formatManager = filePlayer->getAudioFormatManager();
	
	// instansiate the cache and the thumbnail
	if (thumbnailCache == 0)
		thumbnailCache = new AudioThumbnailCache(2);
	thumbnailView = new AudioThumbnail(sourceSamplesPerThumbSample, *formatManager, *thumbnailCache);

	for (int i = 0; i < numWaveformImages; i++) {
		waveImgs.add(new WaveformSection());
		waveImgs[i]->img = new Image();
	}
	
	setZoomFactor(zoomFactor.getCurrent());
	timerCallback(waveformZoomChanged);

	// register with the file player to recieve update messages
	filePlayer->addListener(this);
	filePlayer->addChangeListener(this);
}

DraggableWaveDisplay::~DraggableWaveDisplay()
{
	filePlayer->removeListener(this);
	filePlayer->removeChangeListener(this);

	if (!deleteCache)
		thumbnailCache.release();
}

//====================================================================================
void DraggableWaveDisplay::resized()
{
	startTimer(waveformZoomChanged, 500);
}

void DraggableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();
		
	g.setColour(Colours::black);
	g.fillAll();
	
	if (waveImgs[currentImage]->img->isValid())
	{		
		float zoomLeftScale;
		float zoomScale = zoomLeftScale = 1.0f;
		if (!zoomFactor.areAlmostEqual(0.01))
		{
			zoomScale = zoomFactor.getCurrent() / zoomFactor.getPrevious();
			zoomLeftScale = (1.0f + zoomScale) * 0.5;
		}
	
		// calculate starting positions
		const int startPixelOffset = (playheadPos * w) - (timeToPixels(filePlayer->getCurrentPosition()) * zoomScale);
		int currXPos = startPixelOffset + (timeToPixels(waveImgs[currentImage]->startTime) * zoomScale);
		int nextXPos = currXPos + waveImgs[currentImage]->img->getWidth() * zoomScale;
		int prevXPos = currXPos - waveImgs[currentImage]->img->getWidth() * zoomScale;		
				
		// draw current image
		g.drawImage(*waveImgs[currentImage]->img,
					currXPos, 0, waveImgs[currentImage]->img->getWidth() * zoomScale, h,
					0, 0,
					waveImgs[currentImage]->img->getWidth(), waveImgs[currentImage]->img->getHeight());
		
		// draw next image
		if (currXPos < -w)
		{
			g.drawImage(*waveImgs[nextImage]->img,
						nextXPos, 0, waveImgs[nextImage]->img->getWidth() * zoomScale, h,
						0, 0,
						waveImgs[nextImage]->img->getWidth(), waveImgs[nextImage]->img->getHeight());
//			g.setColour(Colours::green);
//			g.drawRect(nextXPos, 0, int(waveImgs[nextImage]->img->getWidth() * zoomScale), h, 2);
		}
		else if (currXPos > 0 && waveImgs[currentImage]->startTime >= 0.00001) // draw previous image
		{
			g.drawImage(*waveImgs[previousImage]->img,
						prevXPos, 0, waveImgs[previousImage]->img->getWidth() * zoomScale, h,
						0, 0,
						waveImgs[previousImage]->img->getWidth(), waveImgs[previousImage]->img->getHeight());			
//			g.setColour(Colours::red);
//			g.drawRect(prevXPos, 0, int(waveImgs[previousImage]->img->getWidth() * zoomScale), h, 2);
		}

		// cycle images if necessary
		if (nextXPos < 0 ) {
			cycleImages(true);
		}
		else if (currXPos > w) {
			cycleImages(false);
		}
	}
	
	g.setColour (Colours::black);
	g.drawVerticalLine(w * playheadPos - 1, 0, h);
	g.drawVerticalLine(w * playheadPos + 1, 0, h);

	g.setColour(Colours::white);
	g.drawVerticalLine(w * playheadPos, 0, h);	
}
//====================================================================================
void DraggableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == waveformUpdated)
	{
		movedX = timeToPixels(filePlayer->getCurrentPosition());
		
		if (!movedX.areEqual())
			repaint();
	}
	else if (timerId == waveformMoved)
	{
		if (isMouseDown)
		{
			mouseX = getMouseXYRelative().getX();;
			const int currentXDrag = mouseX.getDifference();
			
			if (currentXDrag)
			{
				double position = (filePlayer->getCurrentPosition() - pixelsToTime(currentXDrag));
							
				filePlayer->setPosition(jlimit(0.0, fileLengthSecs, position));
				
//				if(!filePlayer->isPlaying())
//					filePlayer->start();
				
				repaint();
			}
//			else {
//				if(filePlayer->isPlaying())
//					filePlayer->stop();
//			}			

		}
	}
	else if (timerId == waveformLoading)
	{
		if(thumbnailView->isFullyLoaded())
			stopTimer(waveformLoading);
				
		numSamplesFinished = thumbnailView->getNumSamplesFinished();
		
		refreshWaveform(currentImage);
		refreshWaveform(nextImage);
	}
	else if (timerId == waveformZoomChanged)
	{
		zoomFactor = zoomFactor.getCurrent();
		samplesPerPixel = sourceSamplesPerThumbSample / zoomFactor.getCurrent();
		timePerPixel = samplesPerPixel.getCurrent() / currentSampleRate;

		createNewImageForWaveform(previousImage);
		createNewImageForWaveform(currentImage);
		createNewImageForWaveform(nextImage);

		refreshWaveform(previousImage);
		refreshWaveform(currentImage);
		refreshWaveform(nextImage);

		stopTimer(waveformZoomChanged);
	}
}

void DraggableWaveDisplay::changeListenerCallback(ChangeBroadcaster* changedObject)
{
}

void DraggableWaveDisplay::fileChanged (FilteringAudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
		fileLengthSecs = filePlayer->getTotalLength() / currentSampleRate;
		
		DBG(filePlayer->getFileName()<<" - "<<currentSampleRate<<" - "<<fileLengthSecs);
		
		File newFile(filePlayer->getFilePath());
		FileInputSource* fileInputSource = new FileInputSource (newFile);
		thumbnailView->setSource(fileInputSource);
		
		createNewImageForWaveform(currentImage);
		createNewImageForWaveform(previousImage);
		createNewImageForWaveform(nextImage);
		
		waveImgs[currentImage]->startTime = filePlayer->getCurrentPosition();
		waveImgs[nextImage]->startTime = waveImgs[currentImage]->startTime + pixelsToTime(getWidth()*2);
		
		startTimer(waveformLoading, 40);
		startTimer(waveformUpdated, 60);
	}
}

//====================================================================================
void DraggableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	zoomFactor.setOnlyCurrent(newZoomFactor);
	
	int tempSamplesPerPixel = sourceSamplesPerThumbSample / zoomFactor.getCurrent();
//	timePerPixel = samplesPerPixel.getCurrent() / currentSampleRate;

	if (samplesPerPixel.getCurrent() != tempSamplesPerPixel) {
		startTimer(waveformZoomChanged, 500);
	}

	repaint();
}

void DraggableWaveDisplay::setPlayheadPosition(float newPlayheadPosition)
{
	playheadPos = jlimit(0.0f, 1.0f, newPlayheadPosition);
	
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
	mouseX.setBoth(e.x);
	isMouseDown = true;
	
	if (isDraggable)
	{
		if (filePlayer->isPlaying())
		{
			shouldBePlaying = true;
			filePlayer->stop();
		}
		else
			shouldBePlaying = false;
		
		setMouseCursor(MouseCursor::DraggingHandCursor);
		
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
}

//==============================================================================	
double DraggableWaveDisplay::pixelsToTime(double numPixels)
{
	return numPixels * timePerPixel;
}

double DraggableWaveDisplay::timeToPixels(double timeInSecs)
{
	return timeInSecs / timePerPixel;
}

void DraggableWaveDisplay::createNewImageForWaveform(int waveformNumber)
{
	const int w = getWidth();
	const int h = getHeight();

	if (/*w > 0 && h > 0 &&*/ filePlayer->getFileName() != String::empty)
	{
		waveImgs.set(waveformNumber, new WaveformSection);
		int imageWidth = w * 2;
		waveImgs[waveformNumber]->img = new Image(Image::RGB, imageWidth, h, true);
	}	
}

void DraggableWaveDisplay::refreshWaveform(int waveNum)
{	
	if(waveImgs[waveNum]->img->isValid())
	{
		Graphics g(*waveImgs[waveNum]->img);
		waveImgs[waveNum]->img->clear(waveImgs[waveNum]->img->getBounds());

		const double startTime = waveImgs[waveNum]->startTime;
		const double endTime = startTime + pixelsToTime(waveImgs[waveNum]->img->getWidth());

		if (thumbnailView->isFullyLoaded() || (numSamplesFinished > (startTime * currentSampleRate)))
		{
			g.fillAll(Colours::black);
			g.setColour(Colours::lightgreen);
			thumbnailView->drawChannel(g, Rectangle<int> (0, 0, waveImgs[waveNum]->img->getWidth(), waveImgs[waveNum]->img->getHeight()),
									   startTime, endTime,
									   0, 1.0f);
		}

		repaint();
	}	
}

void DraggableWaveDisplay::cycleImages(bool cycleForwards)
{
	if (cycleForwards) 
	{
		WaveformSection *temp = waveImgs[previousImage];
		
		waveImgs.set(previousImage, waveImgs[currentImage], false);
		waveImgs.set(currentImage, waveImgs[nextImage], false);
		waveImgs.set(nextImage, temp, false);
		
		waveImgs[nextImage]->startTime = waveImgs[currentImage]->startTime + pixelsToTime(getWidth() * 2);
		refreshWaveform(nextImage);
	}
	else
	{
		WaveformSection *temp = waveImgs[nextImage];
		
		waveImgs.set(nextImage, waveImgs[currentImage], false);
		waveImgs.set(currentImage, waveImgs[previousImage], false);
		waveImgs.set(previousImage, temp, false);
		
		waveImgs[previousImage]->startTime = waveImgs[currentImage]->startTime - pixelsToTime(getWidth() * 2);
		refreshWaveform(previousImage);
	}

}

//==============================================================================
bool DraggableWaveDisplay::isInterestedInFileDrag (const StringArray &files)
{
	File droppedFile(files[0]);
	if (matchesAudioWildcard(droppedFile.getFileExtension(), formatManager->getWildcardForAllFormats(), true))
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
bool DraggableWaveDisplay::isInterestedInDragSource (const String &sourceDescription, Component *sourceComponent)
{
	if (sourceDescription.startsWith("<ITEMS>")) {
		return true;
	}
	
	return false;	
}

void DraggableWaveDisplay::itemDragExit (const String &sourceDescription, Component *sourceComponent)
{
}

void DraggableWaveDisplay::itemDropped (const String &sourceDescription, Component *sourceComponent, int x, int y)
{
	ScopedPointer<XmlElement> newTracks (XmlDocument::parse(sourceDescription));
	
	if (newTracks->getNumChildElements() > 0) {
		File newFile(newTracks->getChildElement(0)->getStringAttribute("Location"));

		if (newFile.existsAsFile()) {
			filePlayer->setLibraryEntry(ValueTree::fromXml(*newTracks->getChildElement(0)));
			filePlayer->setFile(newFile.getFullPathName());
		}
	}
}

//==============================================================================

END_DROWAUDIO_NAMESPACE
