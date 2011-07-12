/*
  ==============================================================================

    dRowAudio_AbstractDraggableWaveDisplay.cpp
    Created: 12 Mar 2011 8:58:38pm
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_AbstractDraggableWaveDisplay.h"

AbstractDraggableWaveDisplay::AbstractDraggableWaveDisplay(int sourceSamplesPerThumbSample,
                                                           FilteringAudioFilePlayer* sourceToBeUsed,
                                                           MultipleAudioThumbnailCache *cacheToUse)
:	Thread("DraggableWaveDisplayThread"),
	filePlayer(sourceToBeUsed),
	currentSampleRate(44100),
	timePerPixel(1.0),
	zoomFactor(0.5f),
	playheadPos(0.5f),
	shouldRefreshWaveforms(false),
	waveformIsFullyLoaded(false),
	sourceSamplesPerThumbSample(sourceSamplesPerThumbSample),
	thumbnailCache(cacheToUse, (cacheToUse == nullptr) ? true : false),
	isDraggable(true),
	mouseShouldTogglePlay(true)
{
    setOpaque(true);
    startThread(2);

	formatManager = filePlayer->getAudioFormatManager();
	
	// instansiate the cache and the thumbnail
	if (thumbnailCache == nullptr)
    {
        OptionalScopedPointer<MultipleAudioThumbnailCache> newCache(new MultipleAudioThumbnailCache(3), true);
		thumbnailCache = newCache;
	}
    
	setZoomFactor(zoomFactor.getCurrent());

	zoomFactor = zoomFactor.getCurrent();
	samplesPerPixel = sourceSamplesPerThumbSample / zoomFactor.getCurrent();
	timePerPixel = samplesPerPixel.getCurrent() / currentSampleRate;
		
	createNewImagesForAllWaveforms();
	
	// register with the file player to recieve update messages
	filePlayer->addListener(this);
}

AbstractDraggableWaveDisplay::~AbstractDraggableWaveDisplay()
{
	stopThread(5000);

	filePlayer->removeListener(this);
}

//====================================================================================
void AbstractDraggableWaveDisplay::resized()
{
    // redraw playhead image
    playheadImage = Image(Image::RGB, 3, getHeight(), false);
    Graphics g (playheadImage);
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.drawVerticalLine(1, 0, playheadImage.getHeight());

	startTimer(waveformZoomChanged, 500);
}

void AbstractDraggableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();
	
	g.fillAll(Colours::darkgrey);
	
	if (filePlayer->getFileName().isNotEmpty())
	{		
		float zoomScale = 1.0f;
		bool needToScale = false;
		if (! zoomFactor.areAlmostEqual(0.01))
        {
			zoomScale = zoomFactor.getCurrent() / zoomFactor.getPrevious();
			needToScale = true;
		}
		
		// calculate starting positions
		const int startPixelOffset = (playheadPos * w) - (timeToPixels(filePlayer->getAudioTransportSource()->getCurrentPosition()) * zoomScale);
		const int currXPos = startPixelOffset + (timeToPixels(waveImgs[currentImage]->startTime) * zoomScale);
		const int nextXPos = currXPos + (waveImgs[currentImage]->img.getWidth() * zoomScale);
		const int prevXPos = currXPos - (waveImgs[currentImage]->img.getWidth() * zoomScale);		
				
		// draw current image
		if (waveImgs[currentImage]->img.isValid())
		{
			if (needToScale)
			{
				g.drawImage(waveImgs[currentImage]->img,
							currXPos, 0, waveImgs[currentImage]->img.getWidth() * zoomScale, h,
							0, 0,
							waveImgs[currentImage]->img.getWidth(), waveImgs[currentImage]->img.getHeight());
			}
			else
			{
				g.drawImageAt(waveImgs[currentImage]->img,
							  currXPos, 0);
			}
		}
		
		// draw next image
		if (currXPos < -w)
		{
			if (waveImgs[nextImage]->img.isValid())
			{
				if (needToScale)
				{
					g.drawImage(waveImgs[nextImage]->img,
								nextXPos, 0, waveImgs[nextImage]->img.getWidth() * zoomScale, h,
								0, 0,
								waveImgs[nextImage]->img.getWidth(), waveImgs[nextImage]->img.getHeight());
				}
				else
				{
					g.drawImageAt(waveImgs[nextImage]->img,
								  nextXPos, 0);
				}
			}
		}
		else if (currXPos > 0 && waveImgs[currentImage]->startTime >= 0.00001) // draw previous image
		{
			if (waveImgs[previousImage]->img.isValid())
			{
				if (needToScale)
				{
					g.drawImage(waveImgs[previousImage]->img,
								prevXPos, 0, waveImgs[previousImage]->img.getWidth() * zoomScale, h,
								0, 0,
								waveImgs[previousImage]->img.getWidth(), waveImgs[previousImage]->img.getHeight());			
				}
				else
				{
					g.drawImageAt(waveImgs[previousImage]->img,
								  prevXPos, 0);
				}
			}
		}
		
		// cycle images if necessary
		if (nextXPos < 0 ) {
			cycleImages(true);
		}
		else if (currXPos > w) {
			cycleImages(false);
		}
	}
    
    g.drawImageAt(playheadImage, (w * playheadPos) - 1, 0);
}

//====================================================================================
void AbstractDraggableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == waveformUpdated) //moved due to file position changing
	{
		movedX = timeToPixels(filePlayer->getAudioTransportSource()->getCurrentPosition());

		if (!movedX.areEqual() && getWidth() != 0)
		{
			if (abs(movedX.getDifference()) > getWidth()) // need to refresh sections as they have moved a lot
			{
				markAsDirty();
			}

			triggerAsyncUpdate();
		}
	}
	else if (timerId == waveformMoved) // being dragged
	{
		if (isMouseDown)
		{
			mouseX = getMouseXYRelative().getX();;
			const int currentXDrag = mouseX.getDifference();
			
			if (currentXDrag != 0)
			{
				double position = (filePlayer->getAudioTransportSource()->getCurrentPosition() - pixelsToTime(currentXDrag));

				filePlayer->getAudioTransportSource()->setPosition(position);

				triggerAsyncUpdate();
			}
		}
	}
//	else if (timerId == waveformLoading)
//	{
//		thumbnailLoading(waveformIsFullyLoaded, numSamplesFinished);
//
//		if(waveformIsFullyLoaded)
//			stopTimer(waveformLoading);
//
//		if (numSamplesFinished > 0)
//		{
//			const double secondsFinishedRendering = samplesToSeconds(numSamplesFinished, currentSampleRate);
//
//			bool needToNotify = false;
//
//			if (secondsFinishedRendering > waveImgs[previousImage]->startTime
//				&& waveImgs[previousImage]->needToRepaint == true)
//				needToNotify = true;
//			
//			if (secondsFinishedRendering > waveImgs[currentImage]->startTime
//				&& waveImgs[currentImage]->needToRepaint == true)
//				needToNotify = true;
//			
//			if (secondsFinishedRendering > waveImgs[nextImage]->startTime
//				&& waveImgs[nextImage]->needToRepaint == true)
//				needToNotify = true;
//			
//			if (needToNotify)
//				refreshWaveformsOnBackgroundThread();
//		}
//	}
	else if (timerId == waveformZoomChanged)
	{
		zoomFactor = zoomFactor.getCurrent();
		samplesPerPixel = sourceSamplesPerThumbSample / zoomFactor.getCurrent();
		timePerPixel = samplesPerPixel.getCurrent() / currentSampleRate;
		
		createNewImagesForAllWaveforms();
		markAsDirty();
		
		stopTimer(waveformZoomChanged);
	}
}

void AbstractDraggableWaveDisplay::changeListenerCallback (ChangeBroadcaster* source)
{
    thumbnailLoading(waveformIsFullyLoaded, numSamplesFinished);
            
    if (numSamplesFinished > 0)
    {
        const double secondsFinishedRendering = samplesToSeconds(numSamplesFinished, currentSampleRate);

        bool needToNotify = false;

        if (secondsFinishedRendering > waveImgs[previousImage]->startTime
            && waveImgs[previousImage]->needToRepaint == true)
            needToNotify = true;
        
        if (secondsFinishedRendering > waveImgs[currentImage]->startTime
            && waveImgs[currentImage]->needToRepaint == true)
            needToNotify = true;
        
        if (secondsFinishedRendering > waveImgs[nextImage]->startTime
            && waveImgs[nextImage]->needToRepaint == true)
            needToNotify = true;
        
        if (needToNotify)
        {
            refreshWaveformsOnBackgroundThread();
        }
    }
}

void AbstractDraggableWaveDisplay::fileChanged (FilteringAudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
		fileLengthSecs = filePlayer->getAudioTransportSource()->getTotalLength() / currentSampleRate;

        // reset counters so waveform gets properly refreshed
        waveformIsFullyLoaded = false;
        numSamplesFinished = 0;

		newFileLoaded();
		markAsDirty();
		
		//startTimer(waveformLoading, 40);
		startTimer(waveformUpdated, 15);
	}
}

void AbstractDraggableWaveDisplay::markAsDirty()
{
    // clear images quickly first
    Rectangle<int> imageRect(0, 0, waveImgs[currentImage]->img.getWidth(), waveImgs[currentImage]->img.getHeight());
    waveImgs[currentImage]->img.clear(imageRect, Colours::pink);
    waveImgs[previousImage]->img.clear(imageRect, Colours::red);
    waveImgs[nextImage]->img.clear(imageRect, Colours::blue);
    
	waveImgs[currentImage]->needToRepaint = true;
	waveImgs[previousImage]->needToRepaint = true;
	waveImgs[nextImage]->needToRepaint = true;

	const double timePerSection = pixelsToTime(waveImgs[currentImage]->img.getWidth());
	waveImgs[currentImage]->startTime = filePlayer->getAudioTransportSource()->getCurrentPosition();
	waveImgs[previousImage]->startTime = waveImgs[currentImage]->startTime - timePerSection;
	waveImgs[nextImage]->startTime = waveImgs[currentImage]->startTime + timePerSection;

	refreshWaveformsOnBackgroundThread();
}

void AbstractDraggableWaveDisplay::run()
{
    while (! threadShouldExit())
    {
		if (shouldRefreshWaveforms)
		{
			shouldRefreshWaveforms = false;

			//ScopedLock sl(lock);
			// paint current image first
			refreshWaveform(currentImage);
            if (threadShouldExit())
                break;
			
            refreshWaveform(nextImage);
            if (threadShouldExit())
                break;

			refreshWaveform(previousImage);
            if (threadShouldExit())
                break;
		}
        
        if (! shouldRefreshWaveforms)
        {
            wait(-1); //wait until notify is called again
        }
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
		
    triggerAsyncUpdate();
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
			if (filePlayer->getAudioTransportSource()->isPlaying())
			{
				shouldBePlaying = true;
				filePlayer->getAudioTransportSource()->stop();
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
			if (shouldBePlaying && !filePlayer->getAudioTransportSource()->isPlaying())
				filePlayer->getAudioTransportSource()->start();
			else if ( !shouldBePlaying && filePlayer->getAudioTransportSource()->isPlaying() )
				filePlayer->getAudioTransportSource()->stop();
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

void AbstractDraggableWaveDisplay::createNewImagesForAllWaveforms()
{
	createNewImageForWaveform(previousImage);
	createNewImageForWaveform(currentImage);
	createNewImageForWaveform(nextImage);	
}

void AbstractDraggableWaveDisplay::createNewImageForWaveform(int waveformNumber)
{
	const int w = jmax(1, getWidth());
	const int h = jmax(1, getHeight());
	
	ScopedLock sl(lock);
	waveImgs.set(waveformNumber, new WaveformSection);
	const int imageWidth = w * 2;
	waveImgs[waveformNumber]->img = Image(Image::RGB, imageWidth, h, true);
	waveImgs[waveformNumber]->needToRepaint = true;
}

void AbstractDraggableWaveDisplay::cycleImages(bool cycleForwards)
{
				  
	if (cycleForwards) 
	{
		ScopedLock sl(lock);

		WaveformSection *temp = waveImgs[previousImage];
		
		waveImgs.set(previousImage, waveImgs[currentImage], false);
		waveImgs.set(currentImage, waveImgs[nextImage], false);
		waveImgs.set(nextImage, temp, false);
		
		waveImgs[nextImage]->startTime = waveImgs[currentImage]->startTime + pixelsToTime(getWidth() * 2);
		waveImgs[nextImage]->needToRepaint = true;
	}
	else
	{
		ScopedLock sl(lock);
		
		WaveformSection *temp = waveImgs[nextImage];
		
		waveImgs.set(nextImage, waveImgs[currentImage], false);
		waveImgs.set(currentImage, waveImgs[previousImage], false);
		waveImgs.set(previousImage, temp, false);
		
		waveImgs[previousImage]->startTime = waveImgs[currentImage]->startTime - pixelsToTime(getWidth() * 2);
		waveImgs[previousImage]->needToRepaint = true;
	}
	
	refreshWaveformsOnBackgroundThread();
}

void AbstractDraggableWaveDisplay::refreshWaveformsOnBackgroundThread()
{
	shouldRefreshWaveforms = true;
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
bool AbstractDraggableWaveDisplay::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
	if (dragSourceDetails.description.toString().startsWith("<ITEMS>")) {
		return true;
	}
	
	return false;	
}

void AbstractDraggableWaveDisplay::itemDragExit (const SourceDetails& dragSourceDetails)
{
}

void AbstractDraggableWaveDisplay::itemDropped (const SourceDetails& dragSourceDetails)
{
	ScopedPointer<XmlElement> newTracks (XmlDocument::parse(dragSourceDetails.description.toString()));
	
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
