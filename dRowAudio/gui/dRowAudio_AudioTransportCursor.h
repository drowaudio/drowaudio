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

#ifndef __DROWAUDIO_AUDIOTRANSPORTCURSOR_H__
#define __DROWAUDIO_AUDIOTRANSPORTCURSOR_H__

#include "../utility/dRowAudio_StateVariable.h"
#include "../audio/dRowAudio_AudioUtility.h"

//====================================================================================
/** A class to display a scolling cursor to prepresent the position of an audio file.
	
	Clicking and dragging on the display will reposition the transport source.
 */
class AudioTransportCursor :    public Component,
                                public Timer,
                                public AudioFilePlayer::Listener
{
public:
	//====================================================================================
	/** Creates the display.
		The AudioFilePlayer associated with the display must be passed in.
	 */
	explicit AudioTransportCursor (AudioFilePlayer& sourceToBeUsed);
	
	/** Destructor.
     */
	~AudioTransportCursor();

    //====================================================================================
	/** Sets whether or not the transport cursor should be displayed;
     */
    void setCursorDisplayed (bool shouldDisplayCursor);
    
	/** Sets the current horizontal zoom.
        1.0 means the end of the audio file reaches the end of the component, 0.5 means
        the end of the component is halfway through the file etc.
     */
	void setZoomRatio (double newZoomRatio);
    
    /** Sets an offset used to start the waveform at a faction of the component's width.
        A value of 0.5 means the start of the component is halfway through the file etc.
     */
	void setStartOffsetRatio (double newStartOffsetRatio);
            
	//====================================================================================
	/** @internal */
    void fileChanged (AudioFilePlayer* player);
    
	/** @internal */
    void playerStoppedOrStarted (AudioFilePlayer* player);
    
	//====================================================================================
	/** @internal */
    void resized ();
	
	/** @internal */
	void paint (Graphics &g);

	/** @internal */
	void mouseDown (const MouseEvent &e);
	
	/** @internal */
	void mouseUp (const MouseEvent &e);
	
	/** @internal */
	void mouseDrag (const MouseEvent &e);
    
	//====================================================================================
	/** @internal */
	void timerCallback();
		
private:
	//==============================================================================
    AudioFilePlayer& audioFilePlayer;

	double fileLength, oneOverFileLength, currentSampleRate;
    double zoomRatio, startOffsetRatio;
    bool shouldStopTimer;
	
	Image cursorImage;
	
	StateVariable<int> transportLineXCoord;
	float currentXScale;
	bool showTransportCursor;
    
	bool interestedInDrag;
	double currentMouseX;

    //==============================================================================
    void startTimerIfNeeded();
    
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioTransportCursor);
};

#endif // __DROWAUDIO_AUDIOTRANSPORTCURSOR_H__