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

#ifndef __DROWAUDIO_DRAGGABLEWAVEDISPLAY_H__
#define __DROWAUDIO_DRAGGABLEWAVEDISPLAY_H__

#include "../../utility/dRowAudio_StateVariable.h"
#include "../../audio/dRowAudio_AudioUtility.h"

//==============================================================================
/** A class to display the waveform of an audio file which can be dragged to
    reposition the source.
    This class takes an AudioThumbnailImage as its source an then will get
    updated as the thumbnail is generated. You can set the zoom levels of the
    waveform and drag the image to reposition the source.
*/
class DraggableWaveDisplay :    public Component,
                                public MultiTimer,
                                public AudioThumbnailImage::Listener
{
public:
    //==============================================================================
	/** Creates the display.
        The file player associated with the display must be passed in along with
        the current sample rate. This can later be changed with setSampleRate.
	 */
	explicit DraggableWaveDisplay (AudioThumbnailImage& sourceToBeUsed);
	
	/** Destructor.
		Your subclass will need to call signalThreadShouldExit() in its destructor as
		it will get destructed before this superclass.
	 */
	~DraggableWaveDisplay();
	
    //====================================================================================
	/** Sets the current horizontal zoom.
        This must be greater than 0 and the larger the number the more zoomed in the wave will be.
        A value of 1 is the waveform at its rendered resolution so any larger and blocking may occur.
	 */
	void setHorizontalZoom (float newZoomFactor);
	
	/** Sets the offset of the white line that marks the current position.
        This is as a fraction of the width of the display.
	 */
	inline void setPlayheadPosition (float newPlayheadPosition);
	
	/** Turns dragging to reposition the transport on or off.
     */
	inline void setDraggable (bool isWaveformDraggable);
	
	/** Returns true if dragging the waveform will reposition the audio source 
     */
	inline bool getDraggable()              {   return isDraggable;   }
	    
    //====================================================================================
	/** @internal */
    void imageChanged (AudioThumbnailImage* audioThumbnailImage);

	//====================================================================================
	/** @internal */
	void resized();
	
	/** @internal */
	void paint (Graphics &g);
	
	/** @internal */
    void mouseDown (const MouseEvent &e);
	
	/** @internal */
	void mouseUp (const MouseEvent &e);
    
	//====================================================================================
    /** @internal. */
	void timerCallback (int timerId);
				
private:
	//==============================================================================	
    /** Converts a number of pixels to a time at the current resolution. 
     */
    inline double pixelsToTime (double numPixels);

    /** Converts a time to a number of pixels at the current resolution. 
     */
	inline double timeToPixels (double timeInSecs);

    /** Used to start and stop the various internal timers.
     */
	enum
	{
		waveformUpdated,
		waveformMoved
	};
    
    //==============================================================================	
    AudioThumbnailImage& audioThumbnailImage;
    AudioFilePlayer* filePlayer;
	double fileLengthSecs, oneOverFileLength;
    double currentSampleRate, oneOverSampleRate;
    double timePerPixel;
	float zoomRatio, oneOverZoomRatio;
	float playheadPos;
	
	bool waveformIsFullyLoaded;
	
    CriticalSection lock;
    Image playheadImage;

	bool isMouseDown, isDraggable, shouldBePlaying, mouseShouldTogglePlay;
	StateVariable<int> mouseX, movedX;
    
	friend class SwitchableDraggableWaveDisplay;
    
    //==============================================================================	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DraggableWaveDisplay);
};

#endif  // __DROWAUDIO_DRAGGABLEWAVEDISPLAY_H__