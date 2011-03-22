/*
  ==============================================================================

    dRowAudio_SwitchableDraggableWaveDisplay.h
    Created: 23 Feb 2011 2:26:51pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_SWITCHABLEDRAGGABLEWAVEDISPLAY_H_AD989767__
#define __DROWAUDIO_SWITCHABLEDRAGGABLEWAVEDISPLAY_H_AD989767__

#include "../../core/dRowAudio_StandardHeader.h"

#include "../../audio/dRowAudio_FilteringAudioFilePlayer.h"
#include "dRowAudio_MultipleAudioThumbnailCache.h"
#include "dRowAudio_ColouredDraggableWaveDisplay.h"
#include "dRowAudio_TriBandDraggableWaveDisplay.h"

class SwitchableDraggableWaveDisplay : public Component
{
public:
	SwitchableDraggableWaveDisplay(FilteringAudioFilePlayer* sourceToBeUsed, MultipleAudioThumbnailCache *cacheToUse =0);
	
	~SwitchableDraggableWaveDisplay();

	void resized();
	
//	void mouseDoubleClick(const MouseEvent &e);
	void setShowSeparate(bool shouldShowSperate);
	
	//====================================================================================
	/// Sets the current horizontal zoom
	void setZoomFactor (float newZoomFactor);
	
	/**
	 Sets the offset of the white line that marks the current position.
	 This is as a fraction of the width of the display.
	 */
	void setPlayheadPosition(float newPlayheadPosition);
	
	/// Turns dragging to reposition the transport on or off
	void setDraggable (bool isWaveformDraggable);
	
	/// Returns true if dragging the waveform will reposition the audio source 
//	bool getDraggable ()	{	return waveforms[0]->getDraggable();	}
	
	//==============================================================================
	void mouseDown(const MouseEvent &e);
	
	void mouseUp(const MouseEvent &e);
			
	//====================================================================================

private:
	
	bool showSeparate;
	bool togglePlay;
	
	FilteringAudioFilePlayer *filePlayer;
	ScopedPointer<ColouredDraggableWaveDisplay> colouredWave;
	ScopedPointer<TriBandDraggableWaveDisplay> separateWave;
};



#endif  // __DROWAUDIO_SWITCHABLEDRAGGABLEWAVEDISPLAY_H_AD989767__
