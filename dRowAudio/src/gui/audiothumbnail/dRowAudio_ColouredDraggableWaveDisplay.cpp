/*
  ==============================================================================

    dRowAudio_ColouredDraggableWaveDisplay.cpp
    Created: 13 Mar 2011 3:02:12am
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_ColouredDraggableWaveDisplay.h"

ColouredDraggableWaveDisplay::ColouredDraggableWaveDisplay (int sourceSamplesPerThumbnailSample, FilteringAudioFilePlayer* sourceToBeUsed, MultipleAudioThumbnailCache *cacheToUse)
:	AbstractDraggableWaveDisplay(sourceSamplesPerThumbnailSample, sourceToBeUsed, cacheToUse)
{
	tempLargeImage = new Image(Image::RGB, 1, 1, true);

	thumbnailView = new ColouredAudioThumbnail(sourceSamplesPerThumbSample, *filePlayer->getAudioFormatManager(), *thumbnailCache);
}

ColouredDraggableWaveDisplay::~ColouredDraggableWaveDisplay ()
{
	// need to stop the thread running here as this will destruct before the superclass
	//signalThreadShouldExit();
}

void ColouredDraggableWaveDisplay::resized()
{
	AbstractDraggableWaveDisplay::resized();
	
	const int w = jmax(1, getWidth());
	const int h = jmax(1, getHeight());

	tempLargeImage = new Image(Image::RGB, w*4, h, true);
}

//====================================================================================
void ColouredDraggableWaveDisplay::newFileLoaded()
{
	File newFile(filePlayer->getFilePath());
	if (newFile.existsAsFile())
	{
		thumbnailView->setSource(new FileInputSource (newFile));
	}
}

void ColouredDraggableWaveDisplay::thumbnailLoading(bool &isFullyLoaded, int64 &numFinished)
{
		if (thumbnailView->isFullyLoaded())
			isFullyLoaded = true;
		
		numFinished = thumbnailView->getNumSamplesFinished();
}

void ColouredDraggableWaveDisplay::refreshWaveform(int waveNum)
{
	if(waveImgs[waveNum]->img->isValid() && waveImgs[waveNum]->needToRepaint)
	{		
		Graphics g(*tempLargeImage);
		g.fillAll(Colours::black);
		
		const double startTime = waveImgs[waveNum]->startTime;
		const double endTime = startTime + pixelsToTime(waveImgs[waveNum]->img->getWidth());
		
		if (waveformIsFullyLoaded || (numSamplesFinished > (startTime * currentSampleRate)))
		{
			thumbnailView->drawColouredChannel(g, Rectangle<int> (0, 0, tempLargeImage->getWidth(), tempLargeImage->getHeight()),
											   startTime, endTime,
											   0, 1.0f);
		}
					
		Graphics g2(*waveImgs[waveNum]->img);

		g2.drawImage(*tempLargeImage,
					 0, 0, waveImgs[waveNum]->img->getWidth(), waveImgs[waveNum]->img->getHeight(),
					 0, 0, tempLargeImage->getWidth(), tempLargeImage->getHeight(),
					 false);
		
//		g2.setColour(Colours::red);
//		g2.drawText(String(waveImgs[waveNum]->startTime), 0, waveImgs[waveNum]->img->getHeight()*0.75, 60, 20, Justification(Justification::left), false);

		waveImgs[waveNum]->needToRepaint = false;

		triggerAsyncUpdate();
	}	
}

END_DROWAUDIO_NAMESPACE
