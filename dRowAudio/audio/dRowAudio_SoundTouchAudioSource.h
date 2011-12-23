/*
 *  SoundTouchAudioSource.h
 *  dRowAudio
 *
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _SOUNDTOUCHAUDIOSOURCE__H_
#define _SOUNDTOUCHAUDIOSOURCE__H_

#include "dRowAudio_SoundTouchProcessor.h"

//==============================================================================
class SoundTouchAudioSource :   public PositionableAudioSource
{
public:
    //==============================================================================
    SoundTouchAudioSource(PositionableAudioSource* source,
                          bool deleteSourceWhenDeleted = false,
                          int numberOfSamplesToBuffer = 32768,
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
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    void releaseResources();
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
    int64 volatile nextPlayPos, nextReadPos, effectiveNextPlayPos;
    double volatile sampleRate;
    bool isPrepared;
    
    SoundTouchProcessor soundTouchProcessor;
    int volatile numBuffered;
    
    bool readNextBufferChunk();
    void updateNextEffectivePlayPos();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundTouchAudioSource);
};

#endif //_SOUNDTOUCHAUDIOSOURCE__H_