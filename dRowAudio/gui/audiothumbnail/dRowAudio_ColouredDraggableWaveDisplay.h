/*
  ==============================================================================

    dRowAudio_ColouredDraggableWaveDisplay.h
    Created: 13 Mar 2011 3:02:12am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_COLOUREDDRAGGABLEWAVEDISPLAY_H__
#define __DROWAUDIO_COLOUREDDRAGGABLEWAVEDISPLAY_H__

#include "dRowAudio_AbstractDraggableWaveDisplay.h"

//====================================================================================
/** A class to display the waveform of an audio file in a coloured, frequency dependant form.
 
	This will load an audio file and display its waveform. The waveform can then
	be dragged to reposition the transport source. The horizontal zoom can be
	also adjusted.
 */
class ColouredDraggableWaveDisplay :	public AbstractDraggableWaveDisplay
{
public:
	//====================================================================================
	/** Creates a ColouredDraggableWaveDisplay.
        The file player associated with the display must be passed in along with
        the resolution to display it at. If the cache and thumbnail are not null these
        will be used to generate the waveform, igonoring the resolution passed in.
	 */
	explicit ColouredDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
                                           AudioFilePlayer* sourceToBeUsed,
                                           MultipleAudioThumbnailCache *cacheToUse =nullptr,
                                           ColouredAudioThumbnail *thumbnailToUse =nullptr);
	
	/** Destructor.
     */
	~ColouredDraggableWaveDisplay();
	
    /** @internal */
	void resized();
	
protected:
	//====================================================================================
	/**	This is called when a new file is loaded and should be used to pass
        the information on to any thumbnails you may be holding.
	 */
	void newFileLoaded();
	
	/**	This will be called repeatedly as your waveform is loading.
        This must update the numSamplesFinished and the waveformIsFullyLoaded
        members or the timers will run forever.
	 */
	void thumbnailLoading (bool &isFullyLoaded, int64 &numFinished);
	
	/**	This is called when a waveform image needs to refresh its contents.
        eg. the section of the waveform it is displaying.
	 */
	void refreshWaveform();
	
private:
	//====================================================================================
	OptionalScopedPointer<ColouredAudioThumbnail> thumbnailView;
	Image tempLargeImage;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColouredDraggableWaveDisplay);
};

#endif  // __DROWAUDIO_COLOUREDDRAGGABLEWAVEDISPLAY_H__