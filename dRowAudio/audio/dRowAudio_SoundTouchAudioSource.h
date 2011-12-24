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

#ifndef _SOUNDTOUCHAUDIOSOURCE__H_
#define _SOUNDTOUCHAUDIOSOURCE__H_

#include "dRowAudio_SoundTouchProcessor.h"

//==============================================================================
class SoundTouchAudioSource :   public PositionableAudioSource
{
public:
    //==============================================================================
    SoundTouchAudioSource (PositionableAudioSource* source,
                           bool deleteSourceWhenDeleted = false,
                           int numberOfSamplesToBuffer = 2048,
                           int numberOfChannels = 2);
    
    /** Destructor. */
    ~SoundTouchAudioSource();
    
    /** Sets all of the settings at once.
     */
    void setPlaybackSettings (SoundTouchProcessor::PlaybackSettings newSettings);
    
    /** Returns all of the settings.
     */
    SoundTouchProcessor::PlaybackSettings getPlaybackSettings() {   return soundTouchProcessor.getPlaybackSettings();    }
    
    /** Returns the lock used when setting the buffer read positions.
     */
    inline const CriticalSection& getBufferLock()               {   return bufferStartPosLock;  }
    
    /** Returns the SoundTouchProcessor being used.
     */
    inline SoundTouchProcessor& getSoundTouchProcessor()        {   return soundTouchProcessor; }
    
    //==============================================================================
    /** @internal. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    
    /** @internal. */
    void releaseResources();
    
    /** @internal. */
    void getNextAudioBlock (const AudioSourceChannelInfo& info);
    
    //==============================================================================
    /** Implements the PositionableAudioSource method. */
    void setNextReadPosition (int64 newPosition);
    
    /** Implements the PositionableAudioSource method. */
    int64 getNextReadPosition() const;
    
    /** Implements the PositionableAudioSource method. */
    int64 getTotalLength() const                { return source->getTotalLength();  }
    
    /** Implements the PositionableAudioSource method. */
    bool isLooping() const                      { return source->isLooping();       }
    
    /** Implements the PositionableAudioSource method. */
    void setLooping (bool shouldLoop)           { source->setLooping (shouldLoop);  }
    
private:
    //==============================================================================
    OptionalScopedPointer<PositionableAudioSource> source;
    int numberOfSamplesToBuffer, numberOfChannels;
    AudioSampleBuffer buffer;
    CriticalSection bufferStartPosLock;
    int64 volatile nextReadPos, effectiveNextPlayPos;
    double volatile sampleRate;
    bool isPrepared;
    
    SoundTouchProcessor soundTouchProcessor;
    
    void readNextBufferChunk();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundTouchAudioSource);
};

#endif //_SOUNDTOUCHAUDIOSOURCE__H_