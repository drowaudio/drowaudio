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

#include "dRowAudio_SwitchableDraggableWaveDisplay.h"

SwitchableDraggableWaveDisplay::SwitchableDraggableWaveDisplay(FilteringAudioFilePlayer* sourceToBeUsed,
															   MultipleAudioThumbnailCache *cacheToUse,
                                                               ColouredAudioThumbnail *colouredThumbnailToUse)
:	showSeparate(false),
	togglePlay(true),
	filePlayer(sourceToBeUsed)
{
	addAndMakeVisible(colouredWave = new ColouredDraggableWaveDisplay(128, sourceToBeUsed, cacheToUse, colouredThumbnailToUse));
	addChildComponent(separateWave = new TriBandDraggableWaveDisplay(128, sourceToBeUsed, cacheToUse));
	
	colouredWave->setInterceptsMouseClicks(false, false);
	separateWave->setInterceptsMouseClicks(false, false);
}

SwitchableDraggableWaveDisplay::~SwitchableDraggableWaveDisplay()
{
}

void SwitchableDraggableWaveDisplay::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	
	if (showSeparate)
		separateWave->setBounds(0, 0, w, h);
	else
		colouredWave->setBounds(0, 0, w, h);
}

void SwitchableDraggableWaveDisplay::setShowSeparate(bool shouldShowSperate)
{
	if (shouldShowSperate)
	{
		separateWave->setVisible(true);
		colouredWave->setVisible(false);
		showSeparate = true;
		
		separateWave->markAsDirty();
	}
	else
	{
		separateWave->setVisible(false);
		colouredWave->setVisible(true);
		showSeparate = false;

		colouredWave->markAsDirty();
	}
	
	resized();
}

//====================================================================================
void SwitchableDraggableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	colouredWave->setZoomFactor(newZoomFactor);
	separateWave->setZoomFactor(newZoomFactor);
}

void SwitchableDraggableWaveDisplay::setPlayheadPosition(float newPlayheadPosition)
{
	colouredWave->setPlayheadPosition(newPlayheadPosition);
	separateWave->setPlayheadPosition(newPlayheadPosition);
}

void SwitchableDraggableWaveDisplay::setDraggable (bool isWaveformDraggable)
{
	colouredWave->setDraggable(isWaveformDraggable);
	separateWave->setDraggable(isWaveformDraggable);
}

//====================================================================================
void SwitchableDraggableWaveDisplay::mouseDown(const MouseEvent &e)
{
	if (e.mods.isCommandDown())
	{		
		setShowSeparate(!showSeparate);
		togglePlay = false;

		if (showSeparate) {
			separateWave->mouseShouldTogglePlay = togglePlay;
			separateWave->mouseDown(e);
		}
		else {
			colouredWave->mouseShouldTogglePlay = togglePlay;
			colouredWave->mouseDown(e);
		}
	}
	else
	{
		togglePlay = true;

		if (showSeparate) {
			separateWave->mouseShouldTogglePlay = togglePlay;
			separateWave->mouseDown(e);
		}
		else {
			colouredWave->mouseShouldTogglePlay = togglePlay;
			colouredWave->mouseDown(e);
		}
	}
}

void SwitchableDraggableWaveDisplay::mouseUp(const MouseEvent &e)
{
	if (showSeparate)
	{
		separateWave->mouseShouldTogglePlay = togglePlay;

		separateWave->mouseUp(e);
	}
	else
	{
		colouredWave->mouseShouldTogglePlay = togglePlay;

		colouredWave->mouseUp(e);
	}
}

//==============================================================================

END_DROWAUDIO_NAMESPACE
