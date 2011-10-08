/*
  ==============================================================================

    dRowAudio_AbstractDraggableWaveDisplay.cpp
    Created: 12 Mar 2011 8:58:38pm
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

AbstractDraggableWaveDisplay::AbstractDraggableWaveDisplay(int _sourceSamplesPerThumbSample,
                                                           AudioFilePlayer* sourceToBeUsed,
                                                           MultipleAudioThumbnailCache* cacheToUse)
    : Thread                        ("DraggableWaveDisplayThread"),
	  filePlayer                    (sourceToBeUsed),
	  currentSampleRate             (44100),
      oneOverSampleRate             (1.0 / currentSampleRate),
	  timePerPixel                  (1.0),
	  zoomRatio                     (1.0f),
      oneOverZoomRatio              (1.0f),
      playheadPos                   (0.5f),
	  waveformIsFullyLoaded         (false),
	  sourceSamplesPerThumbSample   (_sourceSamplesPerThumbSample),
	  thumbnailCache                (cacheToUse, (cacheToUse == nullptr) ? true : false),
	  isDraggable                   (true),
	  mouseShouldTogglePlay         (true)
{
    setOpaque(true);

	formatManager = filePlayer->getAudioFormatManager();
	
	// instansiate the cache and the thumbnail
	if (thumbnailCache == nullptr)
    {
        OptionalScopedPointer<MultipleAudioThumbnailCache> newCache (new MultipleAudioThumbnailCache (3), true);
		thumbnailCache = newCache;
	}
    
	timePerPixel = sourceSamplesPerThumbSample * oneOverSampleRate;

    waveformImage.lastTimeDrawn = 0.0;
    waveformImage.needsRepaint = false;
    waveformImage.img = Image (Image::RGB, 1, 1, true);
	
	// register with the file player to recieve update messages
	filePlayer->addListener(this);
}

AbstractDraggableWaveDisplay::~AbstractDraggableWaveDisplay()
{
	filePlayer->removeListener (this);

	stopThread (1000);
}

//====================================================================================
void AbstractDraggableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	jassert (newZoomFactor > 0.0f);
	
	zoomRatio = newZoomFactor;
    oneOverZoomRatio = 1.0 / zoomRatio;
    
    triggerAsyncUpdate();
}

void AbstractDraggableWaveDisplay::setPlayheadPosition (float newPlayheadPosition)
{
	playheadPos = jlimit (0.0f, 1.0f, newPlayheadPosition);
    
    triggerAsyncUpdate();
}

void AbstractDraggableWaveDisplay::setDraggable (bool isWaveformDraggable)
{
	isDraggable = isWaveformDraggable;
}

//====================================================================================
void AbstractDraggableWaveDisplay::resized()
{
    // redraw playhead image
    playheadImage = Image (Image::RGB, 3, getHeight(), false);

    Graphics g (playheadImage);
    g.fillAll (Colours::black);
    g.setColour (Colours::white);
    g.drawVerticalLine (1, 0, playheadImage.getHeight());
}

void AbstractDraggableWaveDisplay::paint (Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();
	
	g.fillAll (Colours::darkgrey);
	
	if (filePlayer->getAudioTransportSource()->getTotalLength() > 0)
	{		
        const int startXPosInImage = (filePlayer->getAudioTransportSource()->getNextReadPosition()) / sourceSamplesPerThumbSample;
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

            g.drawImage (clippedImage,
                         destRect.getX(), destRect.getY(), destRect.getWidth(), destRect.getHeight(),
                         0, 0, srcRect.getWidth(), srcRect.getHeight(),
                         false);
		}
	}
    
    g.drawImageAt (playheadImage, (w * playheadPos) - 1, 0);
}

void AbstractDraggableWaveDisplay::mouseDown (const MouseEvent &e)
{
	mouseX.setBoth (e.x);
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
        
		setMouseCursor (MouseCursor::DraggingHandCursor);
		
		startTimer (waveformMoved, 40);
	}
}

void AbstractDraggableWaveDisplay::mouseUp (const MouseEvent &e)
{
	isMouseDown = false;
	
	if (isDraggable)
	{
		if (mouseShouldTogglePlay)
		{
			if (shouldBePlaying && ! filePlayer->getAudioTransportSource()->isPlaying())
				filePlayer->getAudioTransportSource()->start();
			else if (! shouldBePlaying && filePlayer->getAudioTransportSource()->isPlaying())
				filePlayer->getAudioTransportSource()->stop();
		}
		
		setMouseCursor (MouseCursor::NormalCursor);
		
		stopTimer (waveformMoved);
	}
}

//====================================================================================
void AbstractDraggableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == waveformUpdated) //moved due to file position changing
	{
		movedX = timeToPixels(filePlayer->getAudioTransportSource()->getCurrentPosition());

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
				double position = (filePlayer->getAudioTransportSource()->getCurrentPosition() - pixelsToTime (currentXDrag));

				filePlayer->getAudioTransportSource()->setPosition (position);

				triggerAsyncUpdate();
			}
		}
	}
}

void AbstractDraggableWaveDisplay::fileChanged (AudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
        
        if (currentSampleRate > 0.0)
        {
            oneOverSampleRate = 1.0 / currentSampleRate;
            oneOverFileLength = 1.0 / filePlayer->getAudioTransportSource()->getLengthInSeconds();
            timePerPixel = sourceSamplesPerThumbSample * oneOverSampleRate;
            
            // reset counters so waveform gets properly refreshed
            waveformIsFullyLoaded = false;
            numSamplesFinished = 0;

            // refresh wavefrom image
            const int sizeNeeded = jmax (1, (int) filePlayer->getAudioTransportSource()->getTotalLength() / sourceSamplesPerThumbSample);

            waveformImage.img = Image (Image::RGB,
                                       sizeNeeded,
                                       getHeight(),
                                       true);
            waveformImage.lastTimeDrawn = 0.0;
            waveformImage.needsRepaint = true;
            
            newFileLoaded();
            
            startTimer (waveformUpdated, 15);
        }
        else 
        {
            stopTimer (waveformUpdated);
        }
	}
}

void AbstractDraggableWaveDisplay::run()
{
    while (! threadShouldExit())
    {
        refreshWaveform();
        wait(-1);           //wait until notify is called again
    }
}

void AbstractDraggableWaveDisplay::handleAsyncUpdate()
{
	repaint();
}

void AbstractDraggableWaveDisplay::changeListenerCallback (ChangeBroadcaster* source)
{
    thumbnailLoading (waveformIsFullyLoaded, numSamplesFinished);
    
    const double secondsFinishedRendering = samplesToSeconds (numSamplesFinished, currentSampleRate);
    
    if (secondsFinishedRendering > waveformImage.lastTimeDrawn)
        refreshWaveformsOnBackgroundThread();
}

//==============================================================================	
void AbstractDraggableWaveDisplay::refreshWaveformsOnBackgroundThread()
{
    startThread (2);
    notify();
}

double AbstractDraggableWaveDisplay::pixelsToTime (double numPixels)
{
	return numPixels * timePerPixel * oneOverZoomRatio;
}

double AbstractDraggableWaveDisplay::timeToPixels (double timeInSecs)
{
	return timeInSecs / timePerPixel;
}

END_JUCE_NAMESPACE
