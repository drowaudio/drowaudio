/*
  ==============================================================================

    dRowAudio_CompleteColouredDraggableWaveDisplay.cpp
    Created: 13 Mar 2011 3:02:12am
    Author:  David Rowland

  ==============================================================================
*/

//BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_CompleteColouredDraggableWaveDisplay.h"

CompleteColouredDraggableWaveDisplay::CompleteColouredDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
															FilteringAudioFilePlayer* sourceToBeUsed,
															MultipleAudioThumbnailCache *cacheToUse,
                                                            ColouredAudioThumbnail *thumbnailToUse)
:	CompleteDraggableWaveDisplay(thumbnailToUse == nullptr ? sourceSamplesPerThumbnailSample : thumbnailToUse->getNumSamplesPerThumbnailSample(),
                                 sourceToBeUsed,
                                 cacheToUse),
    thumbnailView (thumbnailToUse, (thumbnailToUse == nullptr) ? true : false),
	tempLargeImage(Image::RGB, 1, 1, true)
{
    if (thumbnailToUse == nullptr)
    {
        OptionalScopedPointer<ColouredAudioThumbnail> newThumbnail(new ColouredAudioThumbnail(sourceSamplesPerThumbSample, *filePlayer->getAudioFormatManager(), *thumbnailCache),
                                                                   true);
        thumbnailView = newThumbnail;
    }

    thumbnailView->addChangeListener(this);
}

CompleteColouredDraggableWaveDisplay::~CompleteColouredDraggableWaveDisplay ()
{
    thumbnailView->removeChangeListener(this);
	// need to stop the thread running here as this will destruct before the superclass
	signalThreadShouldExit();
}

void CompleteColouredDraggableWaveDisplay::resized()
{
	const int w = jmax(2, getWidth());
	const int h = jmax(2, getHeight());

	{
		ScopedLock sl(lock);
		tempLargeImage = Image(Image::RGB, w*4, h, true);
	}
	
	CompleteDraggableWaveDisplay::resized();
}

//====================================================================================
void CompleteColouredDraggableWaveDisplay::newFileLoaded()
{
	File newFile(filePlayer->getFilePath());
	if (newFile.existsAsFile())
		thumbnailView->setSource(new FileInputSource (newFile));
}

void CompleteColouredDraggableWaveDisplay::thumbnailLoading(bool &isFullyLoaded, int64 &numFinished)
{
    isFullyLoaded = thumbnailView->isFullyLoaded();
    numFinished = thumbnailView->getNumSamplesFinished();
}

void CompleteColouredDraggableWaveDisplay::refreshWaveform()
{
	if(waveformImage.img.isValid())
	{		
		const double sectionStartTime = waveformImage.lastTimeDrawn;
		const double sectionEndTime = numSamplesFinished * oneOverSampleRate;

        const int startPixelX = roundToInt(timeToPixels(sectionStartTime));
        const int numPixels = roundToInt(timeToPixels(sectionEndTime - sectionStartTime));

        const int tempNumPixels = numPixels * 2;
        if (tempLargeImage.getWidth() < tempNumPixels) 
        {
            tempLargeImage = Image(Image::RGB,
                                   tempNumPixels,
                                   waveformImage.img.getHeight(),
                                   false);
        }
        
        
        Rectangle<int> tempRectangleToDraw (0, 0, tempNumPixels, tempLargeImage.getHeight());
        Rectangle<int> destRectangleToDraw (startPixelX, 0, numPixels, waveformImage.img.getHeight());

		Graphics g(tempLargeImage);
        tempLargeImage.clear(tempRectangleToDraw, Colours::black);

		thumbnailView->drawColouredChannel(g, tempRectangleToDraw,
										   sectionStartTime, sectionEndTime,
										   0, 1.0f);
        
        Graphics g2(waveformImage.img);
        //const MessageManagerLock mmLock;
        g2.drawImage(tempLargeImage,
                     destRectangleToDraw.getX(), destRectangleToDraw.getY(), destRectangleToDraw.getWidth(), destRectangleToDraw.getHeight(),
                     tempRectangleToDraw.getX(), tempRectangleToDraw.getY(), tempRectangleToDraw.getWidth(), tempRectangleToDraw.getHeight());

        waveformImage.lastTimeDrawn = sectionEndTime;

		triggerAsyncUpdate();
	}	
}

//END_DROWAUDIO_NAMESPACE
