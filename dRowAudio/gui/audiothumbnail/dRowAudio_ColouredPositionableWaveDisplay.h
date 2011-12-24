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

#ifndef __DROWAUDIO_COLOUREDPOSITIONABLEWAVEDISPLAY_H__
#define __DROWAUDIO_COLOUREDPOSITIONABLEWAVEDISPLAY_H__

#include "../../utility/dRowAudio_StateVariable.h"
#include "../../audio/dRowAudio_AudioFilePlayer.h"

//==============================================================================	
/** A class to display the waveform of an audio file.
	
	This will load an audio file and display its waveform. Clicking on the waveform will
	reposition the transport source. You can change the file loaded by the associated 
	AudioFilePlayer by dragging a new file onto the display.
 */
class ColouredPositionableWaveDisplay : public Component,
                                        public MultiTimer,
                                        public ChangeListener,
                                        public AsyncUpdater,
                                        public AudioFilePlayer::Listener
{
public:
	
	//==============================================================================	
	/** Used to start and stop the various internal timers. */
	enum
	{
		waveformUpdated,
		waveformResizing
	};
	
    //==============================================================================	
    /**
		Creates the display.
		The file player associated with the display must be passed in.
		To save on the number of threads in your program you can optionally pass in your own
		AudioThumbnailCache. If you pass in your own the caller is responsible for deleting it,
		if not the PositionableWaveform will create and delete its own when not needed.	 
	 */
	explicit ColouredPositionableWaveDisplay (AudioFilePlayer* sourceToBeUsed,
                                              MultipleAudioThumbnailCache* cacheToUse =nullptr,
                                              ColouredAudioThumbnail* thumbnailToUse =nullptr);
	
	/** Destructor. */
	~ColouredPositionableWaveDisplay ();
	
	//====================================================================================
    /** Sets the image resolution in lines per pixel.
     */
    void setResolution (double newResolution);
    
	//====================================================================================
	void resized ();
	
	void paint (Graphics &g);

	//====================================================================================
	void timerCallback (const int timerId);

    void changeListenerCallback (ChangeBroadcaster* source);
    
    void handleAsyncUpdate();
	
	void fileChanged (AudioFilePlayer *player);
			
	//==============================================================================
	void mouseDown(const MouseEvent &e);
	
	void mouseUp(const MouseEvent &e);
	
	void mouseDrag(const MouseEvent &e);
	
private:
	//==============================================================================	
	void refreshWaveform();
	
	AudioFilePlayer* filePlayer;
	double fileLength, oneOverFileLength, currentSampleRate, oneOverSampleRate;
	
	// thumbnail classes
	AudioFormatManager* formatManager;
	OptionalScopedPointer<MultipleAudioThumbnailCache> thumbnailCache;
	OptionalScopedPointer<ColouredAudioThumbnail> thumbnailView;
	
	Image waveformImage, displayImage;
	
	StateVariable<int> transportLineXCoord;
	float zoomFactor, currentXScale;
	
	bool isInitialised;
	double currentMouseX;
    
    double lastTimeDrawn, resolution;
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColouredPositionableWaveDisplay);
};

#endif  // __DROWAUDIO_COLOUREDPOSITIONABLEWAVEDISPLAY_H__
