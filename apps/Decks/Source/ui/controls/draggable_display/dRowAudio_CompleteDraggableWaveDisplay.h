/*
  ==============================================================================

    dRowAudio_CompleteDraggableWaveDisplay.h
    Created: 12 Mar 2011 8:58:38pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_COMPLETEDRAGGABLEWAVEDISPLAY_H_A4879A19__
#define __DROWAUDIO_COMPLETEDRAGGABLEWAVEDISPLAY_H_A4879A19__

#include <dRowAudio/dRowAudio.h>

class PlayheadComponent : public Component
{
public:
    PlayheadComponent()
    {
    }
    
    void resized()
    {
        const int w = 3;
        const int h = getHeight();
        setSize(w, getHeight());
        playheadImage = Image(Image::RGB, w, h, true);
        Graphics g(playheadImage);
        g.setColour (Colours::black);
        g.drawVerticalLine(1, 0, h);
        g.drawVerticalLine(3, 0, h);
        
        g.setColour(Colours::white);
        g.drawVerticalLine(1, 0, h);	        
    }
    
    void paint(Graphics& g)
    {
        g.drawImageAt(playheadImage, 0, playheadImage.getWidth());
    }
    
private:
    Image playheadImage;
};

/**
	A base class to display the waveform of an audio file.
 
	This is the base class to use for draggable or scrolling waveforms to use.
	This will deal with all the mouse tracking, transport repositioning and image
	caching. Your base class will need to actually fill the images to be drawn
	however. take a look at the pure virtual functions for an idea of how to use
	it or the eamples in ColouredDraggableWaveDisplay.
	
	@todo	Render images on a background thread, possibly using a GraphicalComponentManager?
 */
class CompleteDraggableWaveDisplay :	public Component,
										public Thread,
										public MultiTimer,
                                        public ChangeListener,
										public AsyncUpdater,
										public FilteringAudioFilePlayer::Listener,
										public DragAndDropTarget,
										public FileDragAndDropTarget
{
public:
	
	/// Used to start and stop the various internal timers
	enum
	{
		waveformUpdated,
		waveformLoading,
		waveformMoved,
		waveformZoomChanged
	};
	
	/**
	 Creates the display.
	 The file player associated with the display must be passed in along with
	 the current sample rate. This can later be changed with setSampleRate.
	 */
	explicit CompleteDraggableWaveDisplay (int sourceSamplesPerThumbnailSample,
										   FilteringAudioFilePlayer* sourceToBeUsed,
										   MultipleAudioThumbnailCache *cacheToUse =0);
	
	/** Destructor.
		Your subclass will need to call signalThreadShouldExit() in its destructor as
		it will get destructed before this superclass.
	 */
	~CompleteDraggableWaveDisplay();
	
	//====================================================================================
	void resized();
	
	void paint (Graphics &g);
	
	//====================================================================================
	void timerCallback (const int timerId);
		
    /** Called when your waveform source has updated.
        If you are using an AudioThumbnail equivalent make sure you register this class
        as a listener to it.
     */
    void changeListenerCallback (ChangeBroadcaster* source);
    
	void fileChanged (FilteringAudioFilePlayer *player);
	
	/**	Re-freshes the waveform section times and re-draws the images for them.
	 */
	void markAsDirty();
	
    void run();
    
	/**	Used to handle repaints.
	 */
	void handleAsyncUpdate();
	
	//====================================================================================
	/** Sets the current horizontal zoom.
		This must be greater than 0 and the larger the number the more zoomed in the wave will be.
		A value of 1 means that the wave is being filled from its cache rather than having to
		re-read the source file.
	 */
	void setZoomFactor (float newZoomFactor);
	
	/**
	 Sets the offset of the white line that marks the current position.
	 This is as a fraction of the width of the display.
	 */
	void setPlayheadPosition (float newPlayheadPosition);
	
	/// Turns dragging to reposition the transport on or off
	void setDraggable (bool isWaveformDraggable);
	
	/// Returns true if dragging the waveform will reposition the audio source 
	bool getDraggable();
	
	//==============================================================================
	void mouseDown (const MouseEvent &e);
	
	void mouseUp (const MouseEvent &e);
	
	//==============================================================================
	bool isInterestedInFileDrag (const StringArray &files);
	void fileDragEnter (const StringArray &files, int x, int y);
	void fileDragExit (const StringArray &files);
	void filesDropped (const StringArray &files, int x, int y);
	
	//==============================================================================
	bool isInterestedInDragSource (const SourceDetails& dragSourceDetails);
	void itemDragExit (const SourceDetails& dragSourceDetails);
	void itemDropped (const SourceDetails& dragSourceDetails);

	//==============================================================================	
	
protected:
	
	struct WaveformSection {
        double lastTimeDrawn;
		bool needsRepaint;
        Image img;
	};
	
	double pixelsToTime(double numPixels);
	double timeToPixels(double timeInSecs);

	/**	This is called when a new file is loaded and should be used to pass
		the information on to any thumbnails you may be holding.
	 */
	virtual void newFileLoaded() =0;

	/**	This will be called repeatedly as your waveform is loading.
		This must update the numSamplesFinished and the waveformIsFullyLoaded
		members or the timers will run forever.
	 */
	virtual void thumbnailLoading(bool &isFullyLoaded, int64 &numFinished) =0;
	
	/**	This is called when a waveform image needs to refresh its contents.
		eg. the section of the waveform it is displaying.
	 */
	virtual void refreshWaveform() =0;

	void refreshWaveformsOnBackgroundThread();
	
	CriticalSection lock;
	
	FilteringAudioFilePlayer* filePlayer;
	double fileLengthSecs, oneOverFileLength, currentSampleRate, oneOverSampleRate, timePerPixel;
	StateVariable<int> samplesPerPixel;
	float zoomRatio, oneOverZoomRatio;
	float playheadPos;
	bool shouldRefreshWaveforms;
	
	bool waveformIsFullyLoaded;
	
	// thumbnail classes
	const int sourceSamplesPerThumbSample;
	AudioFormatManager *formatManager;
	OptionalScopedPointer<MultipleAudioThumbnailCache> thumbnailCache;
	int64 numSamplesFinished;

    Image playheadImage;
	WaveformSection waveformImage;

	bool isMouseDown, isDraggable, shouldBePlaying, mouseShouldTogglePlay;
	StateVariable<int> mouseX, movedX;

	friend class SwitchableDraggableWaveDisplay;
	
private:
	JUCE_LEAK_DETECTOR (CompleteDraggableWaveDisplay);
};

#endif  // __DROWAUDIO_ABSTRACTDRAGGABLEWAVEDISPLAY_H_A4879A19__
