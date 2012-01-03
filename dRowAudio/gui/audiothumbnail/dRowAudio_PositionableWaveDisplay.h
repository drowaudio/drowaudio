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

#ifndef __DROWAUDIO_POSITIONALWAVEDISPLAY_H__
#define __DROWAUDIO_POSITIONALWAVEDISPLAY_H__

#include "../../utility/dRowAudio_StateVariable.h"
#include "../../audio/dRowAudio_AudioUtility.h"

//====================================================================================
/** A class to display the entire waveform of an audio file.
	
	This will load an audio file and display its waveform. Clicking on the waveform will
	reposition the transport source.
 */
class PositionableWaveDisplay : public Component,
								public Timer,
                                public AudioThumbnailImage::Listener
{
public:
	//====================================================================================
	/** Creates the display.
		The AudioThumbnailImage associated with the display must be passed in.
	 */
	explicit PositionableWaveDisplay (AudioThumbnailImage& sourceToBeUsed);
	
	/** Destructor.
     */
	~PositionableWaveDisplay();

    //====================================================================================
	/** Sets whether or not the transport cursor should be displayed;
     */
    void setCursorDisplayed (bool shoudldDisplayCursor);
    
    /** Sets the colour to use for the background.
     */
    void setBackgroundColour (Colour newBackgroundColour);
    
    /** Sets the colour to use for the waveform.
     */
    void setWaveformColour (Colour newWaveformColour);
        
	/** Sets the current horizontal zoom.
        1.0 displays the whole waveform, 0.5 will show half etc. 
     */
	void setZoomRatio (double newZoomRatio);
    
    /** Sets an offset used to start the waveform at a faction of the display.
        A value of 0.5 will show the waveform starting at the halfway point etc.
     */
	void setStartOffsetRatio (double newStartOffsetRatio);
    
    /** Sets a new vertical zoom ratio.
        Values greater than 1.0 will expand the waveform vertically, less will contract it.
     */
    void setVerticalZoomRatio (double newVerticalZoomRatio);
        
	//====================================================================================
	/** @internal */
    void imageChanged (AudioThumbnailImage* audioThumbnailImage);

	/** @internal */
    void imageUpdated (AudioThumbnailImage* audioThumbnailImage);
    
	/** @internal */
    void imageFinished (AudioThumbnailImage* audioThumbnailImage);
    
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
	void refreshWaveform();
	
	double fileLength, oneOverFileLength, currentSampleRate;
    double zoomRatio, startOffsetRatio, verticalZoomRatio;
	
    AudioThumbnailImage& audioThumbnailImage;
    Colour backgroundColour;
    Colour waveformColour;
    AudioFilePlayer* audioFilePlayer;
	Image cachedImage, cursorImage;
	
	StateVariable<int> transportLineXCoord;
	float currentXScale;
	bool showTransportCursor;
    
	bool interestedInDrag;
	double currentMouseX;
	
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionableWaveDisplay);
};

#endif //__DROWAUDIO_POSITIONALWAVEDISPLAY_H__