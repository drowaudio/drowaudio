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
		Graphics g(tempLargeImage);
		g.fillAll(Colours::black);
		
		const double startTime = sectionToRefresh->startTime;
		const double endTime = startTime + pixelsToTime(sectionToRefresh->img.getWidth());
		bool sectionHasFullyRendered = samplesToSeconds(numSamplesFinished, currentSampleRate) >= endTime;

		thumbnailView->drawColouredChannel(g, Rectangle<int> (0, 0, tempLargeImage.getWidth(), tempLargeImage.getHeight()),
										   startTime, endTime,
										   0, 1.0f);

		if (startTime < 0.0) {
			g.setColour(Colours::darkgrey);
			g.fillRect(0, 0, 2 * timeToPixels(-startTime), tempLargeImage.getHeight());
		}
		
		ScopedLock sl(lock);
		Graphics g2(sectionToRefresh->img);

		g2.drawImage(tempLargeImage,
					 0, 0, sectionToRefresh->img.getWidth(), sectionToRefresh->img.getHeight(),
					 0, 0, tempLargeImage.getWidth(), tempLargeImage.getHeight(),
					 false);
		
//		g2.setColour(Colours::red);
//		g2.drawText(String(sectionToRefresh->startTime),
//					0, sectionToRefresh->img.getHeight()*0.75,
//					60, 20,
//					Justification(Justification::left), false);

		sectionToRefresh->needToRepaint = ! sectionHasFullyRendered;

		triggerAsyncUpdate();
	}	
}

END_DROWAUDIO_NAMESPACE
