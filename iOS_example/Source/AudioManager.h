/*
  ==============================================================================

    AudioManager.h
    Created: 8 Jun 2012 7:40:13am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __AUDIOMANAGER_H_76F77D43__
#define __AUDIOMANAGER_H_76F77D43__

#include "StandardHeader.h"

//==============================================================================
class AudioManager :    public AudioIODeviceCallback
{
public:
    //==============================================================================
    AudioManager();
    
    ~AudioManager();
    
    AudioFilePlayer& getAudioFilePlayer()   {   return audioFilePlayer; }
    
    //==============================================================================
    void audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                           int numInputChannels,
                                           float* const* outputChannelData,
                                           int numOutputChannels,
                                           int numSamples,
                                           const AudioIODeviceCallbackContext& context);
    
    void audioDeviceAboutToStart (AudioIODevice* device);
    
    void audioDeviceStopped();
    
private:
    //==============================================================================
    AudioDeviceManager audioDeviceManager;
    AudioSourcePlayer audioSourcePlayer;
    AudioFilePlayer audioFilePlayer;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioManager);
};


#endif  // __AUDIOMANAGER_H_76F77D43__
