/*
  ==============================================================================

    dRowAudio_ColouredDraggableWaveDisplay.cpp
    Created: 13 Mar 2011 3:02:12am
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

ColouredDraggableWaveDisplay::ColouredDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
															AudioFilePlayer* sourceToBeUsed,
															MultipleAudioThumbnailCache *cacheToUse,
                                                            ColouredAudioThumbnail *thumbnailToUse)
    : AbstractDraggableWaveDisplay (thumbnailToUse == nullptr
                                    ? sourceSamplesPerThumbnailSample
                                    : thumbnailToUse->getNumSamplesPerThumbnailSample(),
                                    sourceToBeUsed,
                                    cacheToUse),
      thumbnailView (thumbnailToUse, (thumbnailToUse == nullptr) ? true : false),
	  tempLargeImage (Image::RGB, 1, 1, true)
{
    if (thumbnailToUse == nullptr)
    {
        OptionalScopedPointer<ColouredAudioThumbnail> newThumbnail (new ColouredAudioThumbnail (sourceSamplesPerThumbSample, 
                                                                                                *filePlayer->getAudioFormatManager(),
                                                                                                *thumbnailCache),
                                                                    true);
        thumbnailView = newThumbnail;
    }

    thumbnailView->addChangeListener(this);
}

ColouredDraggableWaveDisplay::~ColouredDraggableWaveDisplay ()
{
    thumbnailView->removeChangeListener(this);
	// need to stop the thread running here as this will destruct before the superclass
	signalThreadShouldExit();
}

void ColouredDraggableWaveDisplay::resized()
{
	AbstractDraggableWaveDisplay::resized();

	const int w = jmax (2, getWidth());
	const int h = jmax (2, getHeight());

	{
		ScopedLock sl (lock);
		tempLargeImage = Image (Image::RGB, w * 4, h, true);
	}
}

//====================================================================================
void ColouredDraggableWaveDisplay::newFileLoaded()
{
	File newFile (filePlayer->getFile());
    
	if (newFile.existsAsFile())
		thumbnailView->setSource (new FileInputSource (newFile));
}

void ColouredDraggableWaveDisplay::thumbnailLoading (bool &isFullyLoaded, int64 &numFinished)
{
    isFullyLoaded = thumbnailView->isFullyLoaded();
    numFinished = thumbnailView->getNumSamplesFinished();
}

void ColouredDraggableWaveDisplay::refreshWaveform()
{
	if(waveformImage.img.isValid())
	{		
		const double sectionStartTime = waveformImage.lastTimeDrawn;
		const double sectionEndTime = numSamplesFinished * oneOverSampleRate;

        const int startPixelX = roundToInt (timeToPixels (sectionStartTime));
        const int numPixels = roundToInt (timeToPixels (sectionEndTime - sectionStartTime));

        const int tempNumPixels = numPixels * 2;
        if (tempLargeImage.getWidth() < tempNumPixels) 
        {
            tempLargeImage = Image (Image::RGB,
                                    tempNumPixels,
                                    waveformImage.img.getHeight(),
                                    false);
        }
        
        
        Rectangle<int> tempRectangleToDraw (0, 0, tempNumPixels, tempLargeImage.getHeight());
        Rectangle<int> destRectangleToDraw (startPixelX, 0, numPixels, waveformImage.img.getHeight());

		Graphics g (tempLargeImage);
        tempLargeImage.clear (tempRectangleToDraw, Colours::black);

		thumbnailView->drawColouredChannel (g, tempRectangleToDraw,
                                            sectionStartTime, sectionEndTime,
                                            0, 1.0f);
        
        const ScopedLock sl (lock);
        Graphics g2 (waveformImage.img);
        g2.drawImage (tempLargeImage,
                      destRectangleToDraw.getX(), destRectangleToDraw.getY(), destRectangleToDraw.getWidth(), destRectangleToDraw.getHeight(),
                      tempRectangleToDraw.getX(), tempRectangleToDraw.getY(), tempRectangleToDraw.getWidth(), tempRectangleToDraw.getHeight());

        waveformImage.lastTimeDrawn = sectionEndTime;

		triggerAsyncUpdate();
	}	
}

END_JUCE_NAMESPACE