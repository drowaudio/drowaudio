/*
  ==============================================================================

    dRowAudio_TriBandDraggableWaveDisplay.h
    Created: 12 Mar 2011 8:52:20pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_TRIBANDDRAGGABLEWAVEDISPLAY_H_756CD495__
#define __DROWAUDIO_TRIBANDDRAGGABLEWAVEDISPLAY_H_756CD495__

#include "../../core/dRowAudio_StandardHeader.h"

#include "../../audio/dRowAudio_FilteringAudioFilePlayer.h"
#include "dRowAudio_MultipleAudioThumbnailCache.h"
#include "dRowAudio_FilteredAudioThumbnail.h"
#include "dRowAudio_AbstractDraggableWaveDisplay.h"

/**
	A class to display the waveform of an audio file.

	This will load an audio file and display its waveform. The waveform can then
	be dragged to reposition the transport source. The horizontal zoom can be
	adjusted and you can change the file loaded by dragging a new file onto the display.

	@todo	Render images on a background thread, possibly using a GraphicalComponentManager?
 */
class TriBandDraggableWaveDisplay :	public AbstractDraggableWaveDisplay
{
public:
	
	/**
	 Creates the display.
	 The file player associated with the display must be passed in along with
	 the current sample rate. This can later be changed with setSampleRate.
	 */
	explicit TriBandDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
										  FilteringAudioFilePlayer* sourceToBeUsed,
										  MultipleAudioThumbnailCache *cacheToUse =0);
	
	/// Destructor
	~TriBandDraggableWaveDisplay ();
	
	//====================================================================================
	
protected:
	
	/**	This is called when a new file is loaded and should be used to pass
		the information on to any thumbnails you may be holding.
	 */
	void newFileLoaded();
	
	/**	This will be called repeatedly as your waveform is loading.
		This must update the numSamplesFinished and the waveformIsFullyLoaded
		members or the timers will run forever.
	 */
	void thumbnailLoading(bool &isFullyLoaded, int64 &numFinished);
	
	/**	This is called when a waveform image needs to refresh its contents.
		eg. the section of the waveform it is displaying.
	 */
	void refreshWaveform(int waveformNumber);
	
private:
	
	// thumbnail classes
	//ScopedPointer<AudioThumbnail> thumbnailView;

	OwnedArray <BiquadFilterSetup> filterSetups;
	OwnedArray <FilteredAudioThumbnail> waveforms;

	JUCE_LEAK_DETECTOR (TriBandDraggableWaveDisplay);
};


#endif  // __DROWAUDIO_TRIBANDDRAGGABLEWAVEDISPLAY_H_756CD495__
