/*
  ==============================================================================

    dRowAudio_ColouredDraggableWaveDisplay.h
    Created: 13 Mar 2011 3:02:12am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_COLOUREDDRAGGABLEWAVEDISPLAY_H_2C4DA4A2__
#define __DROWAUDIO_COLOUREDDRAGGABLEWAVEDISPLAY_H_2C4DA4A2__

#include "../../core/dRowAudio_StandardHeader.h"

#include "../../audio/dRowAudio_FilteringAudioFilePlayer.h"
#include "dRowAudio_MultipleAudioThumbnailCache.h"
#include "dRowAudio_ColouredAudioThumbnail.h"
#include "dRowAudio_AbstractDraggableWaveDisplay.h"

/**
	A class to display the waveform of an audio file in a coloured, frequency dependant form.
 
	This will load an audio file and display its waveform. The waveform can then
	be dragged to reposition the transport source. The horizontal zoom can be
	adjusted and you can change the file loaded by dragging a new file onto the display.

	@todo	Render images on a background thread, possibly using a GraphicalComponentManager?
 */
class ColouredDraggableWaveDisplay :	public AbstractDraggableWaveDisplay
{
public:
	
	/**
	 Creates the display.
	 The file player associated with the display must be passed in along with
	 the current sample rate. This can later be changed with setSampleRate.
	 */
	explicit ColouredDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
										  FilteringAudioFilePlayer* sourceToBeUsed,
										  MultipleAudioThumbnailCache *cacheToUse =0);
	
	/// Destructor
	~ColouredDraggableWaveDisplay ();
	
	void resized();
	
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
	ScopedPointer<ColouredAudioThumbnail> thumbnailView;
	ScopedPointer<Image> tempLargeImage;
	
	JUCE_LEAK_DETECTOR (ColouredDraggableWaveDisplay);
};

#endif  // __DROWAUDIO_COLOUREDDRAGGABLEWAVEDISPLAY_H_2C4DA4A2__
