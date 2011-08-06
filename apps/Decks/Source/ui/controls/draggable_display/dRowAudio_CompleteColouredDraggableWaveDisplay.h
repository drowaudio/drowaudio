/*
  ==============================================================================

    dRowAudio_CompleteColouredDraggableWaveDisplay.h
    Created: 13 Mar 2011 3:02:12am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_COMPLETECOLOUREDDRAGGABLEWAVEDISPLAY_H_2C4DA4A2__
#define __DROWAUDIO_COMPLETECOLOUREDDRAGGABLEWAVEDISPLAY_H_2C4DA4A2__

#include <dRowAudio/dRowAudio.h>
#include "dRowAudio_CompleteDraggableWaveDisplay.h"

/**
	A class to display the waveform of an audio file in a coloured, frequency dependant form.
 
	This will load an audio file and display its waveform. The waveform can then
	be dragged to reposition the transport source. The horizontal zoom can be
	adjusted and you can change the file loaded by dragging a new file onto the display.

	@todo	Render images on a background thread, possibly using a GraphicalComponentManager?
 */
class CompleteColouredDraggableWaveDisplay :	public CompleteDraggableWaveDisplay
{
public:
	
	/**
	 Creates the display.
	 The file player associated with the display must be passed in along with
	 the current sample rate. This can later be changed with setSampleRate.
	 */
	explicit CompleteColouredDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
                                           FilteringAudioFilePlayer* sourceToBeUsed,
                                           MultipleAudioThumbnailCache *cacheToUse =nullptr,
                                           ColouredAudioThumbnail *thumbnailToUse =nullptr);
	
	/// Destructor
	~CompleteColouredDraggableWaveDisplay ();
	
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
	void refreshWaveform();
	
private:
	
	// thumbnail classes
	OptionalScopedPointer<ColouredAudioThumbnail> thumbnailView;
	Image tempLargeImage;
	
	JUCE_LEAK_DETECTOR (CompleteColouredDraggableWaveDisplay);
};

#endif  // __DROWAUDIO_COLOUREDDRAGGABLEWAVEDISPLAY_H_2C4DA4A2__
