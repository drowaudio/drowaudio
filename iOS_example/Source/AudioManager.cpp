/*
  ==============================================================================

    AudioManager.cpp
    Created: 8 Jun 2012 7:40:13am
    Author:  David Rowland

  ==============================================================================
*/

#include "AudioManager.h"

AudioManager::AudioManager()
{
    audioDeviceManager.initialiseWithDefaultDevices (0, 2);
    audioDeviceManager.addAudioCallback (this);
    
    audioSourcePlayer.setSource (&audioFilePlayer);
    
    audioFilePlayer.getAudioFormatManager()->registerFormat (new AVAssetAudioFormat(), false);
}

AudioManager::~AudioManager()
{
    audioDeviceManager.removeAudioCallback (this);

    audioSourcePlayer.setSource (nullptr);
}

//==============================================================================
void AudioManager::audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                                     int numInputChannels,
                                                     float* const* outputChannelData,
                                                     int numOutputChannels,
                                                     int numSamples,
                                                     const AudioIODeviceCallbackContext& context)
{
    audioSourcePlayer.audioDeviceIOCallbackWithContext (inputChannelData,
                                                        numInputChannels,
                                                        outputChannelData,
                                                        numOutputChannels,
                                                        numSamples,
                                                        context);
}

void AudioManager::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioSourcePlayer.audioDeviceAboutToStart (device);
}

void AudioManager::audioDeviceStopped()
{
    audioSourcePlayer.audioDeviceStopped();
}