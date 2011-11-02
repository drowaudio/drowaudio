/*
 *  dRowAudio_PositionableWaveDisplay.h
 *
 *  Created by David Rowland on 02/11/2011.
 *  Copyright 2011 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_POSITIONALWAVEDISPLAY_H__
#define __DROWAUDIO_POSITIONALWAVEDISPLAY_H__

#include "../../utility/dRowAudio_StateVariable.h"
#include "../../audio/dRowAudio_AudioUtility.h"

//====================================================================================
/** A class to display the waveform of an audio file.
	
	This will load an audio file and display its waveform. Clicking on the waveform will
	reposition the transport source. You can change the file loaded by the associated 
	AudioFilePlayer by dragging a new file onto the display.
 */
class PositionableWaveDisplay : public Component,
								public MultiTimer,
                                public AudioThumbnailImage::Listener
{
public:
	//====================================================================================
	/** Used to start and stop the various internal timers. */
	enum
	{
		waveformUpdated,
		waveformResizing
	};
	
	/** Creates the display.
		The file player associated with the display must be passed in.
		To save on the number of threads in your program you can optionally pass in your own
		AudioThumbnailCache. If you pass in your own the caller is responsible for deleting it,
		if not the PositionableWaveform will create and delete its own when not needed.	 
	 */
	explicit PositionableWaveDisplay (AudioThumbnailImage& sourceToBeUsed);
	
	/** Destructor.
     */
	~PositionableWaveDisplay ();
	
	//====================================================================================
    void imageChanged (AudioThumbnailImage* audioThumbnailImage);

    void imageUpdated (AudioThumbnailImage* audioThumbnailImage);
    
    void imageFinished (AudioThumbnailImage* audioThumbnailImage);
    
	//====================================================================================
	/** @internal */
    void resized ();
	
	/** @internal */
	void paint (Graphics &g);

	//====================================================================================
	/** @internal */
	void timerCallback (const int timerId);

	/** @internal */
//	void fileChanged (FilteringAudioFilePlayer *player);
		
	//====================================================================================
	/// Sets the current horizontal zoom
	void setZoomFactor (float newZoomFactor);
	
	//==============================================================================
	void mouseDown(const MouseEvent &e);
	
	void mouseUp(const MouseEvent &e);
	
	void mouseDrag(const MouseEvent &e);
		
private:
	
	//==============================================================================
	void refreshWaveform();
	
	double fileLength, oneOverFileLength, currentSampleRate;
	
    AudioThumbnailImage& audioThumbnailImage;	
    AudioFilePlayer* audioFilePlayer;
	Image cachedImage, cursorImage;
	
	StateVariable<int> transportLineXCoord;
	float currentXScale;
	
	bool interestedInDrag;
	double currentMouseX;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionableWaveDisplay);
};

#endif //__DROWAUDIO_POSITIONALWAVEDISPLAY_H__