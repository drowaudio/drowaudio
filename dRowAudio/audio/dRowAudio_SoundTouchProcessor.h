/*
 *  dRowAuidio_SoundTouchProcessor.h
 *  dRowAudio
 *
 *	Derived from code by Martin Robinson 2008
 *  Created by David Rowland on 10/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_SOUNDTOUCHPROCESSOR__H_
#define _DROWAUDIO_SOUNDTOUCHPROCESSOR__H_

#include "soundtouch/SoundTouch.h"

using namespace soundtouch;

// Need different input and output buffers for thread safety!

/** Wraps a SoundTouch object to enable pitch and tempo adjustments to an audio buffer;
 
    To use this is very simple, just create one, initialise it with the desired number
    of channels and sample rate then feed it with some samples and read them back out.
    This is not thread safe so make sure that the read and write methods are not called
    simultaneously by different threads.
 */
class SoundTouchProcessor
{
public:
    
    enum PlaybackSetting 
    {
        rateSetting,
        tempoSetting,
        pitchSetting,
        pitchSemitonesSetting
    };
    
    struct PlaybackSettings
    {
        float rate, tempo, pitch;
    };
    
    /** Create a default SoundTouchProcessor.
        Make sure that you call initialise before any processing take place.
        This will apply no shifting/stretching by default, use setPlaybackSetting() to
        apply these effects.
     */
    SoundTouchProcessor();
    
    /** Destructor.
     */
    ~SoundTouchProcessor();
    
    /** Puts the processor into a ready state.
        This must be set before any processing occurs as the results are undefiend if not.
        It is the callers responsibility to make sure the numChannels parameter matches
        those supplied to the read/write methods.
     */
    void initialise (int numChannels, double sampleRate);
    
    /** Writes samples into the pipline ready to be processed.
        Remember to keep a 1:1 ratio of input and output samples more or less samples may
        be required as input compared to output (think of a time stretch). You can find
        this ratio using getNumSamplesRequiredRatio().
     */
    void writeSamples (float** sourceChannelData, int numChannels, int numSamples, int startSampleOffset = 0);
    
    /** Reads out processed samples.
        This will read out as many samples as the processor has ready. Any additional
        space in the buffer will be slienced. As the processor takes a certain ammount of
        samples to calculate an output there is a latency of around 100ms involved in the process.
     */
    void readSamples (float** destinationChannelData, int numChannels, int numSamples, int startSampleOffset = 0);
    
    /** Clears the pipeline of all samples, ready for new processing.
     */
    void clear()                                                    {   soundTouch.clear();     }
    
    /** Flushes the last samples from the processing pipeline to the output.
        Clears also the internal processing buffers.
     
        Note: This function is meant for extracting the last samples of a sound
        stream. This function may introduce additional blank samples in the end
        of the sound stream, and thus it's not recommended to call this function
        in the middle of a sound stream.
     */
    void flush()                                                    {   soundTouch.flush();     }
    
    /** Returns the number of samples ready.
     */
    int getNumReady()                                               {   return soundTouch.numSamples(); }
    
    /** Sets all of the settings at once.
     */
    void setPlaybackSettings (PlaybackSettings newSettings);
    
    /** Returns all of the settings.
     */
    PlaybackSettings getPlaybackSettings()                          {   return settings;                                    }
    
    /** Returns the ratio of input samples required per ouput sample, 1.0 being the same.
     */
    double getNumSamplesRequiredRatio()                             {   return settings.rate * settings.tempo;              }
    
    /** Returns the number of samples in the pipeline but currently unprocessed.
     */
    int getNumUnprocessedSamples()                                  {   return soundTouch.numUnprocessedSamples();          }
        
    //==============================================================================
    
private:
    
    SoundTouch soundTouch;
    
    HeapBlock<float> interleavedInputBuffer, interleavedOutputBuffer;
    int interleavedInputBufferSize, interleavedOutputBufferSize;
    PlaybackSettings settings;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundTouchProcessor);
};

#endif //_DROWAUDIO_SOUNDTOUCHPROCESSOR__H_