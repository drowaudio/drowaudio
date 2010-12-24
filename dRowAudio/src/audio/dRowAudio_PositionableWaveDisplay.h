/*
 *  dRowAudio_TestPositionableWaveDisplay.h
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_POSITIONALWAVEDISPLAY_H_
#define _DROWAUDIO_POSITIONALWAVEDISPLAY_H_

#include "../core/dRowAudio_StandardHeader.h"

#include "../utility/dRowAudio_StateVariable.h"
#include "dRowAudio_AudioUtility.h"
#include "dRowAudio_FilteringAudioFilePlayer.h";

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
	PositionableWaveDisplay (FilteringAudioFilePlayer* sourceToBeUsed);
	
	/// Destructor
	~PositionableWaveDisplay ();
	
	//====================================================================================
	void resized ();
	
	void paint (Graphics &g);
	
	//====================================================================================
	void timerCallback (const int timerId);
	
	void changeListenerCallback(ChangeBroadcaster* changedObject);
	
	//====================================================================================
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
	
	void refreshWaveform();
	
	FilteringAudioFilePlayer* filePlayer;
	double fileLength, oneOverFileLength, currentSampleRate;
	
	// thumbnail classes
	AudioFormatManager* formatManager;
	ScopedPointer<AudioThumbnailCache> thumbnailCache;
	ScopedPointer<AudioThumbnail> thumbnailView;
	
	ScopedPointer<Image> waveformImage;
	
	StateVariable<int> transportLineXCoord;
	float zoomFactor, currentXScale;
	
	bool isMouseDown;
	double currentMouseX;
	
	JUCE_LEAK_DETECTOR (PositionableWaveDisplay);
};

#endif //_DROWAUDIO_POSITIONALWAVEDISPLAY_H_