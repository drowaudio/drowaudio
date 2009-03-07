/*
 *  dRowAudio_DraggableWaveDisplay.h
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DRAGGABLEWAVEDISPLAY_H_
#define _DRAGGABLEWAVEDISPLAY_H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

/**
	A class to display the waveform of an audio file.
	
	This will load an audio file and display its waveform. The waveform can then
	be dragged to reposition the transport source. The horizontal zoom can be
	adjusted and you can change the file loaded by dragging a new file onto the display. 
 */
class DraggableWaveDisplay : public Component,
							 public MultiTimer,
							 public ChangeListener,
							 public FileDragAndDropTarget
{
public:
	
	/// Used to start and stop the various internal timers
	enum
	{
		waveformUpdated,
		waveformLoading,
		waveformMoved
	};
	
	/**
		Creates the display.
		The file player associated with the display must be passed in along with
		the current sample rate. This can later be changed with setSampleRate.
	 */
	DraggableWaveDisplay (AudioFilePlayer* sourceToBeUsed, double sampleRate);
	
	/// Destructor
	~DraggableWaveDisplay ();
	
	//====================================================================================
	void resized ();
	
	void paint (Graphics &g);
	//====================================================================================
	void timerCallback (const int timerId);
	
	void changeListenerCallback(void* changedObject);
	//====================================================================================
	/// Use this to update the sample rate if it changes
	void setSampleRate (double newSampleRate);
	
	/// Sets the current horizontal zoom
	void setZoomFactor (float newZoomFactor);
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
	
private:
	
	AudioFilePlayer* filePlayer;
	double fileLength, currentSampleRate;
	
	// thumbnail classes
	AudioFormatManager* formatManager;
	AudioThumbnailCache* thumbnailCache;
	AudioThumbnail* thumbnailViewLow;
	
	int currentWidth, currentHeight;
	double currentPos;
	
	float zoomFactor, currentXScale;
	
	bool isMouseDown;
	int currentXDrag, currentMouseX, lastMouseX;
};

#endif //_DRAGGABLEWAVEDISPLAY_H_