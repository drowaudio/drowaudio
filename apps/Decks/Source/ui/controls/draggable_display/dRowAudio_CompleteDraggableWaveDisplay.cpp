/*
  ==============================================================================

    dRowAudio_CompleteDraggableWaveDisplay.cpp
    Created: 12 Mar 2011 8:58:38pm
    Author:  David Rowland

  ==============================================================================
*/

//BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_CompleteDraggableWaveDisplay.h"

CompleteDraggableWaveDisplay::CompleteDraggableWaveDisplay(int _sourceSamplesPerThumbSample,
                                                           AudioFilePlayer* sourceToBeUsed,
                                                           MultipleAudioThumbnailCache *cacheToUse)
:	Thread("DraggableWaveDisplayThread"),
	filePlayer(sourceToBeUsed),
	currentSampleRate(44100),
    oneOverSampleRate(1.0 / currentSampleRate),
	timePerPixel(1.0),
	zoomRatio(1.0f),
    oneOverZoomRatio (1.0f),
	playheadPos(0.5f),
	shouldRefreshWaveforms(false),
	waveformIsFullyLoaded(false),
	sourceSamplesPerThumbSample(_sourceSamplesPerThumbSample),
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
    
	timePerPixel = sourceSamplesPerThumbSample * oneOverSampleRate;

    waveformImage.lastTimeDrawn = 0.0;
    waveformImage.needsRepaint = false;
    waveformImage.img = Image(Image::RGB,
                          1,
                          1,
                          true);
	
	// register with the file player to recieve update messages
	filePlayer->addListener(this);
}

CompleteDraggableWaveDisplay::~CompleteDraggableWaveDisplay()
{
	filePlayer->removeListener(this);

	stopThread(10000);
}

//====================================================================================
void CompleteDraggableWaveDisplay::resized()
{
    // redraw playhead image
    playheadImage = Image(Image::RGB, 3, getHeight(), false);
    Graphics g (playheadImage);
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.drawVerticalLine(1, 0, playheadImage.getHeight());
}

void CompleteDraggableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();
	
	g.fillAll(Colours::darkgrey);
	
	if (filePlayer->getPath().isNotEmpty())
	{		
        const int startXPosInImage = (filePlayer->getNextReadPosition()) / sourceSamplesPerThumbSample;
        const int playHeadXPos = playheadPos * w;
        
        int srcX = startXPosInImage - roundToInt (playHeadXPos * oneOverZoomRatio);
        int srcW = roundToInt (w * oneOverZoomRatio);

        if (srcX < 0)
        {
            srcW += srcX;
            srcX = 0;
        }

        int destX = 0;
        int destW = roundToInt (srcW * zoomRatio);
        if (destW < w)
        {
            destX = w - destW;
        }

		if (waveformImage.img.isValid())
		{
            const Rectangle<int> srcRect    (srcX, 0, srcW, waveformImage.img.getHeight());
            const Rectangle<int> destRect   (destX, 0, destW, h);
            Image clippedImage (waveformImage.img.getClippedImage (srcRect));

            g.drawImage(clippedImage,
                        destRect.getX(), destRect.getY(), destRect.getWidth(), destRect.getHeight(),
                        0, 0, srcRect.getWidth(), srcRect.getHeight(),
                        false);
		}
        
//        // draw cue points
//        const double time = 60.0;
//        const double timeAtOrigin = pixelsToTime (startXPosInImage - playHeadXPos) * zoomRatio;
//        const double timeAtEnd = timeAtOrigin + pixelsToTime (w);
//        DBG("origin: "<<timeAtOrigin<<" - end: "<<timeAtEnd<<" - "<<time);
//
//        if (time > timeAtOrigin && time < timeAtEnd) 
//        {
//            const float pixelForTime = timeToPixels (time - timeAtOrigin) * zoomRatio;
//            g.setColour(Colours::red);
//            g.drawLine(pixelForTime, 0.0f, pixelForTime, h, 2.0f);
//        }
	}
    
    g.drawImageAt(playheadImage, (w * playheadPos) - 1, 0);
}

//====================================================================================
void CompleteDraggableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == waveformUpdated) //moved due to file position changing
	{
		movedX = timeToPixels(filePlayer->getCurrentPosition());

		if (! movedX.areEqual())
		{
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
				double position = (filePlayer->getCurrentPosition() - pixelsToTime(currentXDrag));

				filePlayer->setPosition(position);

				triggerAsyncUpdate();
			}
		}
	}
}

void CompleteDraggableWaveDisplay::changeListenerCallback (ChangeBroadcaster* source)
{
    thumbnailLoading(waveformIsFullyLoaded, numSamplesFinished);
            
    const double secondsFinishedRendering = samplesToSeconds(numSamplesFinished, currentSampleRate);

    if (secondsFinishedRendering > waveformImage.lastTimeDrawn)
        refreshWaveformsOnBackgroundThread();
}

void CompleteDraggableWaveDisplay::fileChanged (AudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
        
        if (currentSampleRate > 0.0)
        {
            oneOverSampleRate = (1.0 / currentSampleRate);
            fileLengthSecs = filePlayer->getTotalLength() * oneOverSampleRate;
            oneOverFileLength = 1.0 / fileLengthSecs;
            timePerPixel = sourceSamplesPerThumbSample * oneOverSampleRate;
            
            // reset counters so waveform gets properly refreshed
            waveformIsFullyLoaded = false;
            numSamplesFinished = 0;

            // refresh wavefrom image
            const int sizeNeeded = jmax(1, (int)filePlayer->getTotalLength() / sourceSamplesPerThumbSample);

            waveformImage.img = Image(Image::RGB,
                                      sizeNeeded,//timeToPixels(fileLengthSecs),
                                      getHeight(),
                                      true);
            waveformImage.lastTimeDrawn = 0.0;
            waveformImage.needsRepaint = true;
            
            newFileLoaded();
            
            //startTimer(waveformLoading, 40);
            startTimer(waveformUpdated, 15);
        }
	}
}

void CompleteDraggableWaveDisplay::run()
{
    while (! threadShouldExit())
    {
		if (shouldRefreshWaveforms)
		{
			shouldRefreshWaveforms = false;

			refreshWaveform();
		}
        
        if (! shouldRefreshWaveforms)
        {
            wait(-1); //wait until notify is called again
        }
    }
}

void CompleteDraggableWaveDisplay::handleAsyncUpdate()
{
	repaint();
}

//====================================================================================
void CompleteDraggableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	jassert(newZoomFactor > 0.0f);
	
	zoomRatio = newZoomFactor;
    oneOverZoomRatio = 1.0 / zoomRatio;
    
    triggerAsyncUpdate();
}

void CompleteDraggableWaveDisplay::setPlayheadPosition(float newPlayheadPosition)
{
	playheadPos = jlimit (0.0f, 1.0f, newPlayheadPosition);
		
    triggerAsyncUpdate();
}

void CompleteDraggableWaveDisplay::setDraggable (bool isWaveformDraggable)
{
	isDraggable = isWaveformDraggable;
}

bool CompleteDraggableWaveDisplay::getDraggable ()
{
	return isDraggable;
}

//==============================================================================
void CompleteDraggableWaveDisplay::mouseDown(const MouseEvent &e)
{
	mouseX.setBoth (e.x);
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

		setMouseCursor (MouseCursor::DraggingHandCursor);
		
		startTimer (waveformMoved, 40);
	}
}

void CompleteDraggableWaveDisplay::mouseUp (const MouseEvent &e)
{
	isMouseDown = false;
	
	if (isDraggable)
	{
		if (mouseShouldTogglePlay)
		{
			if (shouldBePlaying && ! filePlayer->isPlaying())
				filePlayer->start();
			else if (! shouldBePlaying && filePlayer->isPlaying())
				filePlayer->stop();
		}
		
		setMouseCursor(MouseCursor::NormalCursor);
		
		stopTimer(waveformMoved);
	}
}

//==============================================================================	
double CompleteDraggableWaveDisplay::pixelsToTime(double numPixels)
{
	return numPixels * timePerPixel * oneOverZoomRatio;
}

double CompleteDraggableWaveDisplay::timeToPixels(double timeInSecs)
{
	return timeInSecs / timePerPixel;
}

void CompleteDraggableWaveDisplay::refreshWaveformsOnBackgroundThread()
{
	shouldRefreshWaveforms = true;
	notify();
}

//==============================================================================
bool CompleteDraggableWaveDisplay::isInterestedInFileDrag (const StringArray &files)
{
	File droppedFile(files[0]);
	if (matchesAudioWildcard(droppedFile.getFileExtension(), formatManager->getWildcardForAllFormats(), true))
		return true;
	else
		return false;
}

void CompleteDraggableWaveDisplay::fileDragEnter (const StringArray &files, int x, int y)
{
	setMouseCursor(MouseCursor::CopyingCursor);
}

void CompleteDraggableWaveDisplay::fileDragExit (const StringArray &files)
{
	setMouseCursor(MouseCursor::NormalCursor);	
}

void CompleteDraggableWaveDisplay::filesDropped (const StringArray &files, int x, int y)
{
	filePlayer->setFile(files[0]);
	setMouseCursor(MouseCursor::NormalCursor);
}

//==============================================================================
bool CompleteDraggableWaveDisplay::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
	if (dragSourceDetails.description.toString().startsWith("<ITEMS>")) {
		return true;
	}
	
	return false;	
}

void CompleteDraggableWaveDisplay::itemDragExit (const SourceDetails& dragSourceDetails)
{
}

void CompleteDraggableWaveDisplay::itemDropped (const SourceDetails& dragSourceDetails)
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

//END_DROWAUDIO_NAMESPACE
