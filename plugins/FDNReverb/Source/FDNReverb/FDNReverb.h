/*
  ==============================================================================

    FDNReverb.h
    Created: 14 Dec 2011 10:02:07am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __FDNREVERB_H_ECF80FC8__
#define __FDNREVERB_H_ECF80FC8__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MultitapDelayLine.h"
#include "FeedbackMatrixBuffer.h"

//==============================================================================
/**
 Performs a simple FDNReverb effect on a stream of audio data.
 
 This is a simple stereo FDNReverb, based on the technique and tunings used in FDNReverb.
 Use setSampleRate() to prepare it, and then call processStereo() or processMono() to
 apply the FDNReverb to your audio data.
 
 @see FDNReverbAudioSource
 */
class FDNReverb
{
public:
    //==============================================================================
    FDNReverb();
    
    //==============================================================================
    enum Params
    {
        earlyReflectionsTime,
        earlyReflectionsGain,
//        earlyReflectionsFeedback,
        numParameters
    };
    
    /** Holds the parameters being used by a FDNReverb object. */
    struct Parameters
    {
        Parameters() noexcept
        : earlyRelectionsGain (1.0f),
          earlyReflectionsTime (1.0f)//,
//          earlyReflectionsFeedback (1.0f)
        {}
        
        float earlyRelectionsGain;
        float earlyReflectionsTime;
//        float earlyReflectionsFeedback;
    };
    
    //==============================================================================
    /** Returns the FDNReverb's current parameters. */
    const Parameters& getParameters() const noexcept    { return parameters; }
    
    /** Applies a new set of parameters to the FDNReverb.
     Note that this doesn't attempt to lock the FDNReverb, so if you call this in parallel with
     the process method, you may get artifacts.
     */
    void setParameters (const Parameters& newParams);

    //==============================================================================
    /** Sets the sample rate that will be used for the FDNReverb.
     You must call this before the process methods, in order to tell it the correct sample rate.
     */
    void setSampleRate (const double sampleRate);
    
    /** Clears the FDNReverb's buffers. */
    void reset();
    
    //==============================================================================
    /** Applies the FDNReverb to two stereo channels of audio data. */
    void processStereo (float* const left, float* const right, const int numSamples) noexcept;
    
    /** Applies the FDNReverb to a single mono channel of audio data. */
    void processMono (float* const samples, const int numSamples) noexcept;
    
private:
    //==============================================================================
    Parameters parameters;
    
//    volatile bool shouldUpdateDamping;
//    float gain, wet1, wet2, dry;
    Random random;
    double currentSampleRate;
        
    enum { numCombs = 8, numAllPasses = 4, numChannels = 2 };
    
    MultitapDelayLine earlyReflections[numChannels];
    FeedbackMatrixBuffer late1[numChannels];
    //    IIRFilter iir [numChannels];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FDNReverb);
};

#endif  // __FDNREVERB_H_ECF80FC8__
