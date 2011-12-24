/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_TribandDraggableWaveDisplay.h"

TriBandDraggableWaveDisplay::TriBandDraggableWaveDisplay (int sourceSamplesPerThumbnailSample, FilteringAudioFilePlayer* sourceToBeUsed, MultipleAudioThumbnailCache *cacheToUse)
:	AbstractDraggableWaveDisplay(sourceSamplesPerThumbnailSample, sourceToBeUsed, cacheToUse)
{
	filterSetups.add(new BiquadFilterSetup(BiquadFilterSetup::Bandpass, 130.0, 1.0));
	filterSetups.add(new BiquadFilterSetup(BiquadFilterSetup::Bandpass, 1200.0, 1.0));
	filterSetups.add(new BiquadFilterSetup(BiquadFilterSetup::Highpass, 2700.0));
	
	for (int i = 0; i < filterSetups.size(); i++) {
		waveforms.add(new FilteredAudioThumbnail(sourceSamplesPerThumbSample, *filePlayer->getAudioFormatManager(), *thumbnailCache, *filterSetups[i]));
	}
}

TriBandDraggableWaveDisplay::~TriBandDraggableWaveDisplay ()
{
	// need to stop the thread running here as this will destruct before the superclass
	signalThreadShouldExit();
}

void TriBandDraggableWaveDisplay::newFileLoaded()
{
	File newFile(filePlayer->getFilePath());
	if (newFile.existsAsFile())
	{
//		FileInputSource* fileInputSource = new FileInputSource (newFile);
//		seems unnessesarry to create three separate streams but they will be deleted by the thumbnail
		
		for (int i = 0; i < waveforms.size(); i++) {
			waveforms[i]->setSource(new FileInputSource (newFile));
		}
	}
}

void TriBandDraggableWaveDisplay::thumbnailLoading(bool &isFullyLoaded, int64 &numFinished)
{
	int numFinishedCount = 0;
	
	for (int i = 0; i < waveforms.size(); i++)
	{
		if (waveforms[i]->isFullyLoaded())
			numFinishedCount++;

		numFinished = jmax(numFinished, waveforms[i]->getNumSamplesFinished());
	}

	if (numFinishedCount == waveforms.size()) {
		isFullyLoaded = true;
	}
	else {
		isFullyLoaded = false;
	}
}

void TriBandDraggableWaveDisplay::refreshWaveform(int waveNum)
{
	ScopedLock sl(lock);
	WaveformSection *sectionToRefresh = waveImgs[waveNum];

	if(sectionToRefresh->img.isValid() && sectionToRefresh->needToRepaint)
	{				
		Graphics g(sectionToRefresh->img);
		sectionToRefresh->img.clear(sectionToRefresh->img.getBounds());
		
		const double startTime = sectionToRefresh->startTime;
		const double endTime = startTime + pixelsToTime(sectionToRefresh->img.getWidth());
		
		if (waveformIsFullyLoaded || (numSamplesFinished > (startTime * currentSampleRate)))
		{
			g.fillAll(Colours::black);
			g.setColour(Colours::red);
			
			int wavHeight = sectionToRefresh->img.getHeight() / 3;
			for (int i = 0; i < waveforms.size(); i++)
			{
				waveforms[i]->drawChannel(g, Rectangle<int> (0, i * wavHeight, sectionToRefresh->img.getWidth(), wavHeight),
										  startTime, endTime,
										  0, 1.0f);
			}
		}
		
		sectionToRefresh->needToRepaint = false;
		triggerAsyncUpdate();
	}	
}

END_DROWAUDIO_NAMESPACE