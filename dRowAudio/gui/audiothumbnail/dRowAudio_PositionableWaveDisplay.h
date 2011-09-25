/*
 *  dRowAudio_TestPositionableWaveDisplay.h
 *
 *  Created by David Rowland on 05/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_POSITIONALWAVEDISPLAY_H_
#define _DROWAUDIO_POSITIONALWAVEDISPLAY_H_

#include "../../core/dRowAudio_StandardHeader.h"

#include "../../utility/dRowAudio_StateVariable.h"
#include "../../audio/dRowAudio_AudioUtility.h"
#include "../../audio/dRowAudio_FilteringAudioFilePlayer.h";

/**
	A class to display the waveform of an audio file.
	
	This will load an audio file and display its waveform. Clicking on the waveform will
	reposition the transport source. You can change the file loaded by the associated 
	AudioFilePlayer by dragging a new file onto the display.
 */
class PositionableWaveDisplay : public Component,
								public MultiTimer,
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
		waveformResizing
	};
	
	/**
		Creates the display.
		The file player associated with the display must be passed in.
		To save on the number of threads in your program you can optionally pass in your own
		AudioThumbnailCache. If you pass in your own the caller is responsible for deleting it,
		if not the PositionableWaveform will create and delete its own when not needed.	 
	 */
	explicit PositionableWaveDisplay (FilteringAudioFilePlayer *sourceToBeUsed, AudioThumbnailCache *cacheToUse =0);
	
	/// Destructor
	~PositionableWaveDisplay ();
	
	//====================================================================================
	void resized ();
	
	void paint (Graphics &g);

	//====================================================================================
	void timerCallback (const int timerId);
	
	void fileChanged (FilteringAudioFilePlayer *player);
		
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
	bool isInterestedInDragSource (const SourceDetails& dragSourceDetails);
	
	void itemDragEnter (const SourceDetails& dragSourceDetails);
	
	void itemDragExit (const SourceDetails& dragSourceDetails);
	
	void itemDropped (const SourceDetails& dragSourceDetails);
	
	//==============================================================================	
	
private:
	
	void refreshWaveform();
	
	FilteringAudioFilePlayer* filePlayer;
	double fileLength, oneOverFileLength, currentSampleRate;
	
	// thumbnail classes
	AudioFormatManager* formatManager;
	ScopedPointer<AudioThumbnailCache> thumbnailCache;
	ScopedPointer<AudioThumbnail> thumbnailView;
	bool deleteCache;
	
	ScopedPointer<Image> waveformImage;
	
	StateVariable<int> transportLineXCoord;
	float zoomFactor, currentXScale;
	
	bool firstLoad, isMouseDown, interestedInDrag;
	double currentMouseX;
	
	JUCE_LEAK_DETECTOR (PositionableWaveDisplay);
};

#endif //_DROWAUDIO_POSITIONALWAVEDISPLAY_H_