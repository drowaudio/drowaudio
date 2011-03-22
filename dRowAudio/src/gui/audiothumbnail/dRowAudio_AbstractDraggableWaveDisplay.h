/*
  ==============================================================================

    dRowAudio_AbstractDraggableWaveDisplay.h
    Created: 12 Mar 2011 8:58:38pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_ABSTRACTDRAGGABLEWAVEDISPLAY_H_A4879A19__
#define __DROWAUDIO_ABSTRACTDRAGGABLEWAVEDISPLAY_H_A4879A19__

#include "../../core/dRowAudio_StandardHeader.h"

#include "../../audio/dRowAudio_FilteringAudioFilePlayer.h"
#include "dRowAudio_MultipleAudioThumbnailCache.h"

/**
 A class to display the waveform of an audio file.
 
 This will load an audio file and display its waveform. The waveform can then
 be dragged to reposition the transport source. The horizontal zoom can be
 adjusted and you can change the file loaded by dragging a new file onto the display.
 
 @todo	Render images on a background thread, possibly using a GraphicalComponentManager?
 */
class AbstractDraggableWaveDisplay :	public Component,
										public Thread,
										public MultiTimer,
										public ChangeListener,
										public FilteringAudioFilePlayer::Listener,
										public FileDragAndDropTarget
{
public:
	
	enum WavefomImages {
		previousImage = 0,
		currentImage,
		nextImage,
		numWaveformImages
	};
	
	/// Used to start and stop the various internal timers
	enum
	{
		init,
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
	explicit AbstractDraggableWaveDisplay (int sourceSamplesPerThumbnailSample, FilteringAudioFilePlayer* sourceToBeUsed, MultipleAudioThumbnailCache *cacheToUse =0);
	
	/// Destructor
	~AbstractDraggableWaveDisplay ();
	
	//====================================================================================
	void resized ();
	
	void paint (Graphics &g);
	
	//====================================================================================
	void timerCallback (const int timerId);
	
	void changeListenerCallback(ChangeBroadcaster* changedObject);
	
	void fileChanged (FilteringAudioFilePlayer *player);
	
    void run();
    
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
	void setPlayheadPosition(float newPlayheadPosition);
	
	/// Turns dragging to reposition the transport on or off
	void setDraggable (bool isWaveformDraggable);
	
	/// Returns true if dragging the waveform will reposition the audio source 
	bool getDraggable ();
	
	//==============================================================================
	void mouseDown(const MouseEvent &e);
	
	void mouseUp(const MouseEvent &e);
	
	//==============================================================================
	bool isInterestedInFileDrag (const StringArray &files);
	void fileDragEnter (const StringArray &files, int x, int y);
	void fileDragExit (const StringArray &files);
	void filesDropped (const StringArray &files, int x, int y);
	
	//==============================================================================	
	
protected:
	
	struct WaveformSection {
		double startTime;
		bool needToRepaint;
		ScopedPointer<Image> img;
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
	virtual void refreshWaveform(int waveformNumber) =0;

	void createNewImageForWaveform(int waveformNumber);
	void cycleImages(bool cycleForwards);
	
	FilteringAudioFilePlayer* filePlayer;
	double fileLengthSecs, currentSampleRate, timePerPixel;
	StateVariable<int> samplesPerPixel;
	StateVariable<float> zoomFactor;
	float playheadPos;
	
	bool waveformIsFullyLoaded;
	
	// thumbnail classes
	const int sourceSamplesPerThumbSample;
	AudioFormatManager *formatManager;
	ScopedPointer<MultipleAudioThumbnailCache> thumbnailCache;
	int64 numSamplesFinished;
	bool deleteCache;
    bool cycleDirection;

	OwnedArray<WaveformSection> waveImgs;

	bool  isMouseDown, isDraggable, shouldBePlaying, mouseShouldTogglePlay;
	StateVariable<int> mouseX, movedX;

	friend class SwitchableDraggableWaveDisplay;
	
private:
	JUCE_LEAK_DETECTOR (AbstractDraggableWaveDisplay);
};

#endif  // __DROWAUDIO_ABSTRACTDRAGGABLEWAVEDISPLAY_H_A4879A19__
