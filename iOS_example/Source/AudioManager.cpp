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
    audioDeviceManager.initialise (0, 2, nullptr, true);
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
void AudioManager::audioDeviceIOCallback (const float** inputChannelData,
                                          int numInputChannels,
                                          float** outputChannelData,
                                          int numOutputChannels,
                                          int numSamples)
{
    audioSourcePlayer.audioDeviceIOCallback (inputChannelData,
                                             numInputChannels,
                                             outputChannelData,
                                             numOutputChannels,
                                             numSamples);
}

void AudioManager::audioDeviceAboutToStart (AudioIODevice* device)
{
    audioSourcePlayer.audioDeviceAboutToStart (device);
}

void AudioManager::audioDeviceStopped()
{
    audioSourcePlayer.audioDeviceStopped();
}