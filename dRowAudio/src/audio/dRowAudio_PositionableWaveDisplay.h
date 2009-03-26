/*
 *  dRowAudio_PositionableWaveDisplay.h
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _POSITIONABLEWAVEDISPLAY_H_
#define _POSITIONABLEWAVEDISPLAY_H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

/**
	A class to display the waveform of an audio file.
	
	This will load an audio file and display its waveform. Clicking on the waveform will
	reposition the transport source. You can change the file loaded by the associated 
	AudioFilePlayer by dragging a new file onto the display.
 */
class PositionableWaveDisplay : public Component,
								public MultiTimer,
								public ChangeListener,
								public FileDragAndDropTarget
{
public:
	
	/// Used to start and stop the various internal timers
	enum
	{
		waveformUpdated,
		waveformLoading
	};
	
	/**
		Creates the display.
		The file player associated with the display must be passed in along with
		the current sample rate. This can later be changed with setSampleRate.
	 */
	PositionableWaveDisplay (FilteringAudioFilePlayer* sourceToBeUsed, double sampleRate);
	
	/// Destructor
	~PositionableWaveDisplay ();
	
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
	
	FilteringAudioFilePlayer* filePlayer;
	double fileLength, oneOverFileLength, currentSampleRate;
	
	// thumbnail classes
	AudioFormatManager* formatManager;
	AudioThumbnailCache* thumbnailCache;
	AudioThumbnail* thumbnailViewLow;
	
	int currentWidth, currentHeight;
	double currentPos;
	
	float zoomFactor, currentXScale;
	
	bool isMouseDown;
	double currentMouseX;
};

#endif //_POSITIONABLEWAVEDISPLAY_H_