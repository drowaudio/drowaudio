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
	playheadPos(0.5f),
	zoomFactor(1.0f),
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
		waveformImage.add(new WaveformSection());
		waveformImage[i]->img = new Image();
	}
	
	setZoomFactor(zoomFactor);
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
//	const int w = currentWidth = getWidth();
//	const int h = currentHeight = getHeight();
}

void DraggableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();
		
	g.setColour(Colours::darkgrey);
	g.fillAll();
	if (waveformImage[currentImage]->img->isValid())
	{
		int startXPos = (playheadPos * w) - timeToPixels(filePlayer->getCurrentPosition());
		startXPos += timeToPixels(waveformImage[currentImage]->startTime);
		int nextXPos = startXPos + waveformImage[currentImage]->img->getWidth();
		int prevXPos = startXPos - waveformImage[currentImage]->img->getWidth();
		
		// draw current image
		g.drawImage(*waveformImage[currentImage]->img,
					startXPos, 0, waveformImage[currentImage]->img->getWidth(), h,
					0, 0,
					waveformImage[currentImage]->img->getWidth(), waveformImage[currentImage]->img->getHeight());
		
		// draw next image
		if (startXPos < -w)
		{
			g.drawImage(*waveformImage[nextImage]->img,
						nextXPos, 0, waveformImage[nextImage]->img->getWidth(), h,
						0, 0,
						waveformImage[nextImage]->img->getWidth(), waveformImage[nextImage]->img->getHeight());
		}
		else if (startXPos > 0) // draw previous image
		{
			g.drawImage(*waveformImage[previousImage]->img,
						prevXPos, 0, waveformImage[previousImage]->img->getWidth(), h,
						0, 0,
						waveformImage[previousImage]->img->getWidth(), waveformImage[previousImage]->img->getHeight());			
		}

		// cycle images if necessary
		if (nextXPos < 0 ) {
			DBG("cycle images forward");
			cycleImages(true);
		}
		else if (startXPos > w) {
			DBG("cycle images back");
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
			Point<int> mousePoint = getMouseXYRelative();
			mouseX = mousePoint.getX();
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
				
		refreshWaveform(currentImage);
		refreshWaveform(nextImage);
	}
	else if (timerId == waveformZoomChanged)
	{
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
		
		waveformImage[currentImage]->startTime = filePlayer->getCurrentPosition();
		waveformImage[nextImage]->startTime = waveformImage[currentImage]->startTime + pixelsToTime(getWidth()*2);
		
		startTimer(waveformLoading, 25);
		startTimer(waveformUpdated, 60);
	}
}

//====================================================================================
void DraggableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	const int w = getWidth();
	DBG("new zoom factor: "<<(float)zoomFactor);
	zoomFactor = newZoomFactor;
	
	samplesPerPixel = sourceSamplesPerThumbSample / zoomFactor;
	timePerPixel = samplesPerPixel.getCurrent() / currentSampleRate;

	if (!samplesPerPixel.areEqual()) {
		startTimer(waveformZoomChanged, 500);
	}
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
	const int w = getWidth();
	
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

	if (w > 0 && h > 0 && filePlayer->getFileName() != String::empty)
	{
		waveformImage.set(waveformNumber, new WaveformSection);
		waveformImage[waveformNumber]->img = new Image(Image::RGB, (w * 2), h, true);
	}	
}

void DraggableWaveDisplay::refreshWaveform(int waveNum)
{	
	if(waveformImage[waveNum]->img->isValid())
	{
		const int w = getWidth();
		const int h = getHeight();
		
		Graphics g(*waveformImage[waveNum]->img);
		waveformImage[waveNum]->img->clear(waveformImage[waveNum]->img->getBounds());

		g.fillAll(Colours::black);
		g.setColour(Colours::lightgreen);
		thumbnailView->drawChannel(g, Rectangle<int> (0, 0, waveformImage[waveNum]->img->getWidth(), waveformImage[waveNum]->img->getHeight()),
								   waveformImage[waveNum]->startTime, waveformImage[waveNum]->startTime + pixelsToTime(waveformImage[waveNum]->img->getWidth())/*fileLengthSecs*/,
								   0, 1.0f);
		
//		if (waveNum == currentImage)
//			g.setColour(Colours::red);
//		else if (waveNum == nextImage)
//			g.setColour(Colours::purple);
//
//		g.drawRect(0, 0, 2*w, h, 1);
		repaint();
	}	
}

void DraggableWaveDisplay::cycleImages(bool cycleForwards)
{
	if (cycleForwards) 
	{
		WaveformSection *temp = waveformImage[previousImage];
		
		waveformImage.set(previousImage, waveformImage[currentImage], false);
		waveformImage.set(currentImage, waveformImage[nextImage], false);
		waveformImage.set(nextImage, temp, false);
		
		waveformImage[nextImage]->startTime = waveformImage[currentImage]->startTime + pixelsToTime(getWidth() * 2);
		refreshWaveform(nextImage);
	}
	else
	{
		WaveformSection *temp = waveformImage[nextImage];
		
		waveformImage.set(nextImage, waveformImage[currentImage], false);
		waveformImage.set(currentImage, waveformImage[previousImage], false);
		waveformImage.set(previousImage, temp, false);
		
		waveformImage[previousImage]->startTime = waveformImage[currentImage]->startTime - pixelsToTime(getWidth() * 2);
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

END_DROWAUDIO_NAMESPACE
