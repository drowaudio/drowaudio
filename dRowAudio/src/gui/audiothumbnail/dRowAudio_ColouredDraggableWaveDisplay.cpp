/*
  ==============================================================================

    dRowAudio_ColouredDraggableWaveDisplay.cpp
    Created: 13 Mar 2011 3:02:12am
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_ColouredDraggableWaveDisplay.h"

ColouredDraggableWaveDisplay::ColouredDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
															FilteringAudioFilePlayer* sourceToBeUsed,
															MultipleAudioThumbnailCache *cacheToUse,
                                                            ColouredAudioThumbnail *thumbnailToUse)
:	AbstractDraggableWaveDisplay(sourceSamplesPerThumbnailSample,
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

ColouredDraggableWaveDisplay::~ColouredDraggableWaveDisplay ()
{
    thumbnailView->removeChangeListener(this);
	// need to stop the thread running here as this will destruct before the superclass
	signalThreadShouldExit();
}

void ColouredDraggableWaveDisplay::resized()
{
	const int w = jmax(2, getWidth());
	const int h = jmax(2, getHeight());

	{
		ScopedLock sl(lock);
		tempLargeImage = Image(Image::RGB, w*4, h, true);
	}
	
	AbstractDraggableWaveDisplay::resized();
}

//====================================================================================
void ColouredDraggableWaveDisplay::newFileLoaded()
{
	File newFile(filePlayer->getFilePath());
	if (newFile.existsAsFile())
		thumbnailView->setSource(new FileInputSource (newFile));
}

void ColouredDraggableWaveDisplay::thumbnailLoading(bool &isFullyLoaded, int64 &numFinished)
{
    isFullyLoaded = thumbnailView->isFullyLoaded();
    numFinished = thumbnailView->getNumSamplesFinished();
}

void ColouredDraggableWaveDisplay::refreshWaveform(int waveNum)
{
	WaveformSection* sectionToRefresh = waveImgs[waveNum];

	if(sectionToRefresh->img.isValid() && sectionToRefresh->needToRepaint)
	{		
		const double sectionStartTime = sectionToRefresh->lastTimeDrawn;
        const double imageEndTime = sectionToRefresh->startTime + pixelsToTime(sectionToRefresh->img.getWidth());
		const double sectionEndTime = jlimit(sectionStartTime, imageEndTime, numSamplesFinished * oneOverSampleRate);
        const int startPixelX = roundToInt(timeToPixels(sectionStartTime - sectionToRefresh->startTime));
        const int numPixels = roundToInt(timeToPixels(sectionEndTime - sectionStartTime));
        
        const int tempStartPixel = (roundToInt(timeToPixels(sectionStartTime - sectionToRefresh->startTime))) * 2;
        const int tempNumPixels = numPixels * 2;
        
        Rectangle<int> tempRectangleToDraw (tempStartPixel, 0, tempNumPixels, tempLargeImage.getHeight());
        Rectangle<int> destRectangleToDraw (startPixelX, 0, numPixels, sectionToRefresh->img.getHeight());

		Graphics g(tempLargeImage);
        tempLargeImage.clear(tempRectangleToDraw, Colours::black);
        
		thumbnailView->drawColouredChannel(g, tempRectangleToDraw,
										   sectionStartTime, sectionEndTime,
										   0, 1.0f);

        if (sectionStartTime < 0.0) 
        {
            g.setColour(Colours::darkgrey);
            g.fillRect(tempStartPixel, 0, roundToInt(timeToPixels(-sectionStartTime * 2.0)), tempLargeImage.getHeight());
        }
        
        // create temp image to draw onto to avoid locking
        Image tempImage(sectionToRefresh->img);
        tempImage.duplicateIfShared();
        
        Graphics g2(tempImage);
        g2.drawImage(tempLargeImage,
                     destRectangleToDraw.getX(), destRectangleToDraw.getY(), destRectangleToDraw.getWidth(), destRectangleToDraw.getHeight(),
                     tempRectangleToDraw.getX(), tempRectangleToDraw.getY(), tempRectangleToDraw.getWidth(), tempRectangleToDraw.getHeight());
        
        // lock whilst copying
        {
            const MessageManagerLock mmLock;
            sectionToRefresh->img = tempImage;
		}
//		g2.setColour(Colours::red);
//		g2.drawText(String(sectionToRefresh->startTime),
//					0, sectionToRefresh->img.getHeight()*0.75,
//					60, 20,
//					Justification(Justification::left), false);

        sectionToRefresh->lastTimeDrawn = sectionEndTime;
		sectionToRefresh->needToRepaint = sectionEndTime <= imageEndTime;

		triggerAsyncUpdate();
	}	
}

END_DROWAUDIO_NAMESPACE
