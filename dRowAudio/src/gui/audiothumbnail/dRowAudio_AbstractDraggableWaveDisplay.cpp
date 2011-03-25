/*
  ==============================================================================

    dRowAudio_AbstractDraggableWaveDisplay.cpp
    Created: 12 Mar 2011 8:58:38pm
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_AbstractDraggableWaveDisplay.h"

AbstractDraggableWaveDisplay::AbstractDraggableWaveDisplay(int sourceSamplesPerThumbSample, FilteringAudioFilePlayer* sourceToBeUsed, MultipleAudioThumbnailCache *cacheToUse)
:	Thread("DraggableWaveDisplayThread"),
	filePlayer(sourceToBeUsed),
	currentSampleRate(44100),
	timePerPixel(1.0),
	zoomFactor(0.5f),
	playheadPos(0.5f),
	waveformIsFullyLoaded(false),
	sourceSamplesPerThumbSample(sourceSamplesPerThumbSample),
	thumbnailCache(cacheToUse),
	deleteCache(thumbnailCache ? false : true),
	isDraggable(true),
	mouseShouldTogglePlay(true)
{
	formatManager = filePlayer->getAudioFormatManager();
	
	// instansiate the cache and the thumbnail
	if (thumbnailCache == 0)
		thumbnailCache = new MultipleAudioThumbnailCache(3);
	
	
	setZoomFactor(zoomFactor.getCurrent());

	zoomFactor = zoomFactor.getCurrent();
	samplesPerPixel = sourceSamplesPerThumbSample / zoomFactor.getCurrent();
	timePerPixel = samplesPerPixel.getCurrent() / currentSampleRate;
		
	createNewImageForWaveform(previousImage);
	createNewImageForWaveform(currentImage);
	createNewImageForWaveform(nextImage);
	
	// register with the file player to recieve update messages
	filePlayer->addListener(this);
	filePlayer->addChangeListener(this);
    
	startTimer(init, 100);
}

AbstractDraggableWaveDisplay::~AbstractDraggableWaveDisplay()
{
	stopThread(500);

	filePlayer->removeListener(this);
	filePlayer->removeChangeListener(this);
	
	if (!deleteCache)
		thumbnailCache.release();
}

//====================================================================================
void AbstractDraggableWaveDisplay::resized()
{
	startTimer(waveformZoomChanged, 500);
}

void AbstractDraggableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();
	
	g.setColour(Colours::black);
	g.fillAll();
	
	if (waveImgs[currentImage]->img->isValid() && (filePlayer->getFileName().isNotEmpty()))
	{		
		float zoomScale = 1.0f;
		if (!zoomFactor.areAlmostEqual(0.01))
			zoomScale = zoomFactor.getCurrent() / zoomFactor.getPrevious();
		
		// calculate starting positions
		const int startPixelOffset = (playheadPos * w) - (timeToPixels(filePlayer->getCurrentPosition()) * zoomScale);
		const int currXPos = startPixelOffset + (timeToPixels(waveImgs[currentImage]->startTime) * zoomScale);
		const int nextXPos = currXPos + (waveImgs[currentImage]->img->getWidth() * zoomScale);
		const int prevXPos = currXPos - (waveImgs[currentImage]->img->getWidth() * zoomScale);		
				
		// draw current image
		g.drawImage(*waveImgs[currentImage]->img,
					currXPos, 0, waveImgs[currentImage]->img->getWidth() * zoomScale, h,
					0, 0,
					waveImgs[currentImage]->img->getWidth(), waveImgs[currentImage]->img->getHeight());

		// draw next image
		if (currXPos < -w)
		{
			if ((waveImgs[currentImage]->startTime + pixelsToTime(waveImgs[currentImage]->img->getWidth()))
				> fileLengthSecs) {
				return;
			}
			
			g.drawImage(*waveImgs[nextImage]->img,
						nextXPos, 0, waveImgs[nextImage]->img->getWidth() * zoomScale, h,
						0, 0,
						waveImgs[nextImage]->img->getWidth(), waveImgs[nextImage]->img->getHeight());
//			g.setColour(Colours::blue);
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
void AbstractDraggableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == init) {
		startThread(1);
		stopTimer(init);
	}
	else if (timerId == waveformUpdated)
	{
		movedX = timeToPixels(filePlayer->getCurrentPosition());
		
		if (!movedX.areEqual())
		{
			if (abs(movedX.getDifference()) > getWidth()) {
				waveImgs[previousImage]->needToRepaint = true;
				waveImgs[currentImage]->needToRepaint = true;
				waveImgs[nextImage]->needToRepaint = true;
				notify();
			}
			triggerAsyncUpdate();
		}
	}
	else if (timerId == waveformMoved)
	{
		if (isMouseDown)
		{
			mouseX = getMouseXYRelative().getX();;
			const int currentXDrag = mouseX.getDifference();
			
			if (currentXDrag != 0)
			{
				double position = (filePlayer->getCurrentPosition() - pixelsToTime(currentXDrag));
				
				filePlayer->setPosition(jlimit(0.0, fileLengthSecs, position));
				
				//				if(!filePlayer->isPlaying())
				//					filePlayer->start();
				
				triggerAsyncUpdate();
			}
			//			else {
			//				if(filePlayer->isPlaying())
			//					filePlayer->stop();
			//			}			
			
		}
	}
	else if (timerId == waveformLoading)
	{
		thumbnailLoading(waveformIsFullyLoaded, numSamplesFinished);
		
		if(waveformIsFullyLoaded)
			stopTimer(waveformLoading);
				
		waveImgs[currentImage]->needToRepaint = true;
		waveImgs[nextImage]->needToRepaint = true;
		notify();
	}
	else if (timerId == waveformZoomChanged)
	{
		zoomFactor = zoomFactor.getCurrent();
		samplesPerPixel = sourceSamplesPerThumbSample / zoomFactor.getCurrent();
		timePerPixel = samplesPerPixel.getCurrent() / currentSampleRate;
		
		createNewImageForWaveform(previousImage);
		createNewImageForWaveform(currentImage);
		createNewImageForWaveform(nextImage);
		
		waveImgs[previousImage]->needToRepaint = true;
		waveImgs[currentImage]->needToRepaint = true;
		waveImgs[nextImage]->needToRepaint = true;
		notify();
		
		stopTimer(waveformZoomChanged);
	}
}

void AbstractDraggableWaveDisplay::changeListenerCallback(ChangeBroadcaster* changedObject)
{
}

void AbstractDraggableWaveDisplay::fileChanged (FilteringAudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
		fileLengthSecs = filePlayer->getTotalLength() / currentSampleRate;
		movedX = -1; //force waveform to repaint
		
		DBG(filePlayer->getFileName()<<" - "<<currentSampleRate<<" - "<<fileLengthSecs);
				
		newFileLoaded();
		
		timerCallback(waveformZoomChanged);

		const double timePerSection = pixelsToTime(waveImgs[currentImage]->img->getWidth());
		waveImgs[currentImage]->startTime = filePlayer->getCurrentPosition();
		waveImgs[previousImage]->startTime = waveImgs[currentImage]->startTime - timePerSection;
		waveImgs[nextImage]->startTime = waveImgs[currentImage]->startTime + timePerSection;
		
		startTimer(waveformLoading, 40);
		startTimer(waveformUpdated, 60);
	}
}

void AbstractDraggableWaveDisplay::run()
{
    while (!threadShouldExit())
    {
		refreshWaveform(previousImage);
		refreshWaveform(currentImage);
		refreshWaveform(nextImage);

        wait(-1);
    }
}

void AbstractDraggableWaveDisplay::handleAsyncUpdate()
{
	repaint();
}

//====================================================================================
void AbstractDraggableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	jassert(newZoomFactor > 0.0f);
	
	zoomFactor.setOnlyCurrent(newZoomFactor);

	int tempSamplesPerPixel = sourceSamplesPerThumbSample / zoomFactor.getCurrent();
	
	if (samplesPerPixel.getCurrent() != tempSamplesPerPixel) {
		startTimer(waveformZoomChanged, 500);
	}
	
	triggerAsyncUpdate();
}

void AbstractDraggableWaveDisplay::setPlayheadPosition(float newPlayheadPosition)
{
	playheadPos = jlimit(0.0f, 1.0f, newPlayheadPosition);
		
	waveImgs[previousImage]->needToRepaint = true;
	waveImgs[currentImage]->needToRepaint = true;
	waveImgs[nextImage]->needToRepaint = true;
	
	notify();
}

void AbstractDraggableWaveDisplay::setDraggable (bool isWaveformDraggable)
{
	isDraggable = isWaveformDraggable;
}
bool AbstractDraggableWaveDisplay::getDraggable ()
{
	return isDraggable;
}

//==============================================================================
void AbstractDraggableWaveDisplay::mouseDown(const MouseEvent &e)
{
	mouseX.setBoth(e.x);
	isMouseDown = true;
	
	if (isDraggable)
	{
		if (mouseShouldTogglePlay)
		{
			if (filePlayer->isPlaying())
			{
				shouldBePlaying = true;
				filePlayer->stop();
			}
			else
				shouldBePlaying = false;
		}

		setMouseCursor(MouseCursor::DraggingHandCursor);
		
		startTimer(waveformMoved, 40);
	}
}

void AbstractDraggableWaveDisplay::mouseUp(const MouseEvent &e)
{
	isMouseDown = false;
	
	if (isDraggable)
	{
		if (mouseShouldTogglePlay)
		{
			if (shouldBePlaying && !filePlayer->isPlaying())
				filePlayer->start();
			else if ( !shouldBePlaying && filePlayer->isPlaying() )
				filePlayer->stop();
		}
		
		setMouseCursor(MouseCursor::NormalCursor);
		
		stopTimer(waveformMoved);
	}
}

//==============================================================================	
double AbstractDraggableWaveDisplay::pixelsToTime(double numPixels)
{
	return numPixels * timePerPixel;
}

double AbstractDraggableWaveDisplay::timeToPixels(double timeInSecs)
{
	return timeInSecs / timePerPixel;
}

void AbstractDraggableWaveDisplay::createNewImageForWaveform(int waveformNumber)
{
	const int w = jmax(1, getWidth());
	const int h = jmax(1, getHeight());
	
	waveImgs.set(waveformNumber, new WaveformSection);
	const int imageWidth = w * 2;
	waveImgs[waveformNumber]->img = new Image(Image::RGB, imageWidth, h, true);
	waveImgs[waveformNumber]->needToRepaint = true;
	
	notify();
}

void AbstractDraggableWaveDisplay::cycleImages(bool cycleForwards)
{
	ScopedLock sl(lock);
				  
	if (cycleForwards) 
	{
		WaveformSection *temp = waveImgs[previousImage];
		
		waveImgs.set(previousImage, waveImgs[currentImage], false);
		waveImgs.set(currentImage, waveImgs[nextImage], false);
		waveImgs.set(nextImage, temp, false);
		
		waveImgs[nextImage]->startTime = waveImgs[currentImage]->startTime + pixelsToTime(getWidth() * 2);
		waveImgs[nextImage]->needToRepaint = true;
	}
	else
	{
		WaveformSection *temp = waveImgs[nextImage];
		
		waveImgs.set(nextImage, waveImgs[currentImage], false);
		waveImgs.set(currentImage, waveImgs[previousImage], false);
		waveImgs.set(previousImage, temp, false);
		
		waveImgs[previousImage]->startTime = waveImgs[currentImage]->startTime - pixelsToTime(getWidth() * 2);
		waveImgs[previousImage]->needToRepaint = true;
	}
	
	notify();
}

//==============================================================================
bool AbstractDraggableWaveDisplay::isInterestedInFileDrag (const StringArray &files)
{
	File droppedFile(files[0]);
	if (matchesAudioWildcard(droppedFile.getFileExtension(), formatManager->getWildcardForAllFormats(), true))
		return true;
	else
		return false;
}

void AbstractDraggableWaveDisplay::fileDragEnter (const StringArray &files, int x, int y)
{
	setMouseCursor(MouseCursor::CopyingCursor);
}

void AbstractDraggableWaveDisplay::fileDragExit (const StringArray &files)
{
	setMouseCursor(MouseCursor::NormalCursor);	
}

void AbstractDraggableWaveDisplay::filesDropped (const StringArray &files, int x, int y)
{
	filePlayer->setFile(files[0]);
	setMouseCursor(MouseCursor::NormalCursor);
}

//==============================================================================

END_DROWAUDIO_NAMESPACE
