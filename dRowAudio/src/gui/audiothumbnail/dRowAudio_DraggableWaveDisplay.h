/*
 *  dRowAudio_DraggableWaveDisplay.h
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DRAGGABLEWAVEDISPLAY_H_
#define _DRAGGABLEWAVEDISPLAY_H_

#include "../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_AudioUtility.h";
#include "dRowAudio_FilteringAudioFilePlayer.h";
#include "../utility/dRowAudio_StateVariable.h"

/**
	A class to display the waveform of an audio file.
	
	This will load an audio file and display its waveform. The waveform can then
	be dragged to reposition the transport source. The horizontal zoom can be
	adjusted and you can change the file loaded by dragging a new file onto the display.
 
	You can either drag normal audio files onto the waveform or XmlElements in the form:
	@code
	<ITEMS>
		<ITEM Location="/absolutePathToFile"/>
	<ITEMS>
	@endcode
 
	@todo	Render images on a background thread, possibly using a GraphicalComponentManager?
 */
class DraggableWaveDisplay :	public Component,
								public MultiTimer,
								public ChangeListener,
								public FilteringAudioFilePlayer::Listener,
								public DragAndDropTarget,
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
	explicit DraggableWaveDisplay (FilteringAudioFilePlayer* sourceToBeUsed, AudioThumbnailCache *cacheToUse =0);
	
	/// Destructor
	~DraggableWaveDisplay ();
	
	//====================================================================================
	void resized ();
	
	void paint (Graphics &g);
	
	//====================================================================================
	void timerCallback (const int timerId);
	
	void changeListenerCallback(ChangeBroadcaster* changedObject);
	
	void fileChanged (FilteringAudioFilePlayer *player);
	
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
	bool getDraggable ();
	
	//==============================================================================
	void mouseDown(const MouseEvent &e);
	
	void mouseUp(const MouseEvent &e);
	
	void mouseDrag(const MouseEvent &e);
	
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
	
private:
	
	struct WaveformSection {
		double startTime;
		ScopedPointer<Image> img;
	};
	
	double pixelsToTime(double numPixels);
	double timeToPixels(double timeInSecs);
	void createNewImageForWaveform(int waveformNumber);
	void refreshWaveform(int waveformNumber);
	void cycleImages(bool cycleForwards);
	
	FilteringAudioFilePlayer* filePlayer;
	double fileLengthSecs, currentSampleRate, timePerPixel;
	StateVariable<int> samplesPerPixel;
	StateVariable<float> zoomFactor;
	float playheadPos;
	
	// thumbnail classes
	const int sourceSamplesPerThumbSample;
	AudioFormatManager *formatManager;
	ScopedPointer<AudioThumbnailCache> thumbnailCache;
	ScopedPointer<AudioThumbnail> thumbnailView;
	int64 numSamplesFinished;
	bool deleteCache;

	OwnedArray<WaveformSection> waveImgs;
	
	bool isMouseDown, isDraggable, shouldBePlaying;
	StateVariable<int> mouseX, movedX;
	
	JUCE_LEAK_DETECTOR (DraggableWaveDisplay);
};

#endif //_DRAGGABLEWAVEDISPLAY_H_