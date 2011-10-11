/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-10 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef __DROWAUDIO_LOOPINGAUDIOSOURCE_H__
#define __DROWAUDIO_LOOPINGAUDIOSOURCE_H__

#include "../utility/dRowAudio_Utility.h"

//==============================================================================
/**
    A type of PositionalAudioSource that will read from a PositionableAudioSource
    and can loop between to set times.

    @see PositionableAudioSource, AudioTransportSource, BufferingAudioSource
*/
class LoopingAudioSource  : public PositionableAudioSource
{
public:
    //==============================================================================
    /** Creates an LoopingAudioFormatReaderSource for a given reader.

        @param sourceReader                     the reader to use as the data source
        @param deleteReaderWhenThisIsDeleted    if true, the reader passed-in will be deleted
                                                when this object is deleted; if false it will be
                                                left up to the caller to manage its lifetime
    */
    LoopingAudioSource (PositionableAudioSource* const inputSource,
                        const bool deleteInputWhenDeleted);

    /** Destructor. */
    ~LoopingAudioSource();

    //==============================================================================
    /** Sets the start and end times of the loop.
        This doesn't actually activate the loop, use setLoopBetweenTimes() to toggle this.
     */
	void setLoopTimes (double startTime, double endTime);
	
    /** Enables the loop point set.
     */
    void setLoopBetweenTimes (bool shouldLoop);

    /** Returns true if the loop is activated.
     */
    bool getLoopBetweenTimes();
	
    //==============================================================================
    /** Implementation of the AudioSource method. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);

    /** Implementation of the AudioSource method. */
    void releaseResources();

    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

    //==============================================================================
    /** Implements the PositionableAudioSource method. */
    void setNextReadPosition (int64 newPosition);
    
    /** Implements the PositionableAudioSource method. */
    int64 getNextReadPosition() const;
    
    /** Implements the PositionableAudioSource method. */
    int64 getTotalLength() const;
    
    /** Implements the PositionableAudioSource method. */
    bool isLooping() const;
    
private:
    //==============================================================================
    OptionalScopedPointer<PositionableAudioSource> input;

	bool volatile isLoopingBetweenTimes;
    double loopStartTime, loopEndTime;
    int64 loopStartSample, loopEndSample;
    double currentSampleRate;

    AudioSourceChannelInfo tempInfo;
    AudioSampleBuffer tempBuffer;
    	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopingAudioSource);
};

#endif   // __DROWAUDIO_LOOPINGAUDIOSOURCE_H__