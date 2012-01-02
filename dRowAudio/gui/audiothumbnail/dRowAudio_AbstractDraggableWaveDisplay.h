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

#ifndef __DROWAUDIO_ABSTRACTDRAGGABLEWAVEDISPLAY_H__
#define __DROWAUDIO_ABSTRACTDRAGGABLEWAVEDISPLAY_H__

//==============================================================================
/**
	A base class to display the waveform of an audio file.
 
	This is the base class to use for draggable or scrolling waveforms to use.
	This will deal with all the mouse tracking, transport repositioning and image
	caching. Your base class will need to actually fill the images to be drawn
	however. take a look at the pure virtual functions for an idea of how to use
	it or the eamples in ColouredDraggableWaveDisplay.
	
	@todo	Render images on a background thread, possibly using a GraphicalComponentManager?
 */
class AbstractDraggableWaveDisplay :	public Component,
										public Thread,
										public MultiTimer,
                                        public ChangeListener,
										public AsyncUpdater,
										public AudioFilePlayer::Listener
{
public:
    //==============================================================================
	/// Used to start and stop the various internal timers
	enum
	{
		waveformUpdated,
		waveformMoved
	};
	
    //==============================================================================
	/**
	 Creates the display.
	 The file player associated with the display must be passed in along with
	 the current sample rate. This can later be changed with setSampleRate.
	 */
	explicit AbstractDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
										   AudioFilePlayer* sourceToBeUsed,
										   MultipleAudioThumbnailCache *cacheToUse =nullptr);
	
	/** Destructor.
		Your subclass will need to call signalThreadShouldExit() in its destructor as
		it will get destructed before this superclass.
	 */
	~AbstractDraggableWaveDisplay();
	
    //====================================================================================
	/** Sets the current horizontal zoom.
        This must be greater than 0 and the larger the number the more zoomed in the wave will be.
        A value of 1 is the waveform at its rendered resolution so any larger and blocking may occur.
	 */
	void setHorizontalZoom (float newZoomFactor);
	
	/**
	 Sets the offset of the white line that marks the current position.
	 This is as a fraction of the width of the display.
	 */
	inline void setPlayheadPosition (float newPlayheadPosition);
	
	/// Turns dragging to reposition the transport on or off
	inline void setDraggable (bool isWaveformDraggable);
	
	/// Returns true if dragging the waveform will reposition the audio source 
	inline bool getDraggable()              {   return isDraggable;   }
	    
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
		    
    /** @internal. */
	void fileChanged (AudioFilePlayer *player);
		
    /** @internal. */
    void run();
    
    /** @internal. */
	void handleAsyncUpdate();
    
    /** Called when your waveform source has updated.
     If you are using an AudioThumbnail equivalent make sure you register this class
     as a listener to it.
     */
    void changeListenerCallback (ChangeBroadcaster* source);
    
		
protected:
	//==============================================================================	
	struct WaveformSection {
        double lastTimeDrawn;
		bool needsRepaint;
        Image img;
	};
	
	/**	This is called when a new file is loaded and should be used to pass
		the information on to any thumbnails you may be holding.
	 */
	virtual void newFileLoaded() =0;

	/**	This will be called repeatedly as your waveform is loading.
		This must update the numSamplesFinished and the waveformIsFullyLoaded
		members or the timers will run forever.
	 */
	virtual void thumbnailLoading (bool &isFullyLoaded, int64 &numFinished) =0;
	
	/**	This is called when a waveform image needs to refresh its contents.
		eg. the section of the waveform it is displaying.
	 */
	virtual void refreshWaveform() =0;

    /** Tells the waveform to refresh on the background thread.
     */
	inline void refreshWaveformsOnBackgroundThread();

    /** Converts a number of pixels to a time at the current resolution. 
     */
    inline double pixelsToTime (double numPixels);

    /** Converts a time to a number of pixels at the current resolution. 
     */
	inline double timeToPixels (double timeInSecs);

    //==============================================================================	
    AudioFilePlayer* filePlayer;
	double fileLengthSecs, oneOverFileLength;
    double currentSampleRate, oneOverSampleRate;
    double timePerPixel;
	float zoomRatio, oneOverZoomRatio;
	float playheadPos;
	
	bool waveformIsFullyLoaded;
	
	// thumbnail classes
	const int sourceSamplesPerThumbSample;
	AudioFormatManager *formatManager;
	OptionalScopedPointer<MultipleAudioThumbnailCache> thumbnailCache;
	int64 numSamplesFinished;

    CriticalSection lock;
    Image playheadImage;
	WaveformSection waveformImage;

	bool isMouseDown, isDraggable, shouldBePlaying, mouseShouldTogglePlay;
	StateVariable<int> mouseX, movedX;
    
	friend class SwitchableDraggableWaveDisplay;
	
private:
    
	JUCE_LEAK_DETECTOR (AbstractDraggableWaveDisplay);
};

#endif  // __DROWAUDIO_ABSTRACTDRAGGABLEWAVEDISPLAY_H__