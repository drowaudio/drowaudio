/*
 *  AudioEngine.cpp
 *  Decks
 *
 *  Created by David Rowland on 15/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "AudioEngine.h"

juce_ImplementSingleton (AudioEngine)

AudioEngine::AudioEngine()
:	deckManager(DeckManager::getInstance()),
	currentMixer(0)
{	
	mainAudioCallback = new MainAudioCallback();
	
	// set up the main audio device
	mainAudioDeviceManager.initialise (1, /* number of input channels */
									   2, /* number of output channels */
									   0, /* no XML settings.. */
									   true  /* select default device on failure */);
	mainAudioDeviceManager.addAudioCallback (mainAudioCallback);
	
}

AudioEngine::~AudioEngine()
{
	// zero the device manager and source player to avoid crashing
	mainAudioDeviceManager.removeAudioCallback (mainAudioCallback);

	DBG("AudioEngine deleted");
}

//=========================================================================================
// Main Audio Callback
// This  callback deals with the main audio system
//=========================================================================================
AudioEngine::MainAudioCallback::MainAudioCallback()
:	currentBuffSize(0)
{
	buffer = new AudioSampleBuffer(2, currentBuffSize);

	for (int i = 0; i < DeckManager::getInstance()->getMaxNoDecks(); i++) {
		audioSourcePlayers.add(new AudioSourcePlayer());
		audioSourcePlayers[i]->setSource(DeckManager::getInstance()->getDeck(i)->getMainFilePlayer());
	}
}

AudioEngine::MainAudioCallback::~MainAudioCallback()
{
	for (int i=0; i < audioSourcePlayers.size(); i++)
		audioSourcePlayers[i]->setSource(0);	
	audioSourcePlayers.clear();

	DBG("Main audio callback deleted");	
}

void AudioEngine::MainAudioCallback::audioDeviceIOCallback (const float** inputChannelData,
															int totalNumInputChannels,
															float** outputChannelData,
															int totalNumOutputChannels,
															int numSamples)
{
	DeckManager* manager = DeckManager::getInstance();
	
	// only need to change the buffer size if it needs to be bigger
	if (currentBuffSize != numSamples)
	{
		currentBuffSize = numSamples;
		buffer->setSize(totalNumOutputChannels, currentBuffSize, false, false, true);
		DBG("Changing size");
	}
	buffer->clear();
	
	float yGain = manager->getXFaderSetting(XFADER_SETTING(level));
	float xGain = 1.0f-yGain;

	int assignX = manager->getXFaderSetting(XFADER_SETTING(assignX));
	int assignY = manager->getXFaderSetting(XFADER_SETTING(assignY));
	
	float masterGain = manager->getMasterSetting(MASTER_SETTING(gain));
	
	for (int i=0; i < audioSourcePlayers.size(); i++)
	{		
		if (manager->getDeckSetting(i, CHANNEL_SETTING(on)))
		{
			bool bypassed = manager->getDeckSetting(i, CHANNEL_SETTING(bypass));
			float channelLevel = manager->getDeckSetting(i, CHANNEL_SETTING(level));
						
			// if it is not bypassed
			if (!bypassed && (channelLevel != 0.0f))
			{
				audioSourcePlayers.getUnchecked(i)->audioDeviceIOCallback (inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);

				if (AudioEngine::getInstance()->currentMixer)
					AudioEngine::getInstance()->currentMixer->updateMeterForChannel(i, outputChannelData, numSamples, totalNumOutputChannels);
				
				// apply xFader gain
				if (assignX == (i+1)) {
					for (int c=0; c<totalNumOutputChannels; c++)
						for (int s=0; s<numSamples; s++) {
							outputChannelData[c][s]*=xGain;
						}
				}
				if (assignY == (i+1)) {
					for (int c=0; c<totalNumOutputChannels; c++)
						for (int s=0; s<numSamples; s++) {
							outputChannelData[c][s]*=yGain;
						}
				}
				
				// add the samples to the output buffer
				for (int c=0; c<totalNumOutputChannels; c++)
					buffer->addFrom(c, 0, outputChannelData[c], numSamples, channelLevel);
			}
			else // if bypassed we still need to update playhead positions
			{
				FilteringAudioFilePlayer *player = DeckManager::getInstance()->getDeck(i)->getMainFilePlayer();
				if (player->isPlaying() && (player->getNextReadPosition() < player->getTotalLength()))
					player->setNextReadPosition(player->getNextReadPosition() + numSamples);
			}
		}
	}
	
	
	// apply master fader level
	buffer->applyGain(0, numSamples, masterGain);
	
	// copy local buffer to output buffer
	for (int c=0; c < totalNumOutputChannels; c++) {
		for (int s=0; s<numSamples; s++) {
			outputChannelData[c][s] = buffer->getArrayOfChannels()[c][s];
		}
	}	

	if (AudioEngine::getInstance()->currentMixer)
		AudioEngine::getInstance()->currentMixer->updateMasterMeter(outputChannelData, numSamples, totalNumOutputChannels);
}

void AudioEngine::MainAudioCallback::audioDeviceAboutToStart (AudioIODevice* device)
{
	DBG("Main audio device about to start");
	for (int i=0; i < audioSourcePlayers.size(); i++)
		audioSourcePlayers[i]->audioDeviceAboutToStart (device);	
}

void AudioEngine::MainAudioCallback::audioDeviceStopped()
{
	DBG("Main audio device stopped");
	for (int i=0; i < audioSourcePlayers.size(); i++)
		audioSourcePlayers[i]->audioDeviceStopped ();	
}


//=========================================================================================
// Monitor Audio Callback
// This second callback deals with the monitoring system
//=========================================================================================
/*AudioEngine::MonitorAudioCallback::MonitorAudioCallback(AudioEngine *mainEngine_)
:	deckManager(DeckManager::getInstance()),
	mainEngine(mainEngine_)
{
	for (int i = 0; i < deckManager->getMaxNoDecks(); i++) {
		audioSourcePlayers.add(new AudioSourcePlayer());
		audioSourcePlayers[i]->setSource(deckManager->getDeck(i)->getMonitorFilePlayer());
	}
}

AudioEngine::MonitorAudioCallback::~MonitorAudioCallback(){}

//=========================================================================================
void AudioEngine::MonitorAudioCallback::audioDeviceIOCallback (const float** inputChannelData,
															   int totalNumInputChannels,
															   float** outputChannelData,
															   int totalNumOutputChannels,
															   int numSamples)
{
	for (int i=0; i < audioSourcePlayers.size(); i++)
	{
		if (Settings::getInstance()->getPropertyOfChannel(i, CHANNEL_SETTING(cue)))
		{
			audioSourcePlayers.getUnchecked(i)->audioDeviceIOCallback (inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
		}
	}
}

void AudioEngine::MonitorAudioCallback::audioDeviceAboutToStart (AudioIODevice* device)
{
	DBG("Monitor Audio Manager device started");
	for (int i=0; i < audioSourcePlayers.size(); i++)
		audioSourcePlayers[i]->audioDeviceAboutToStart (device);
}

void AudioEngine::MonitorAudioCallback::audioDeviceStopped()
{
	DBG("Monitor Audio Manager device stopped");
	for (int i=0; i < audioSourcePlayers.size(); i++)
		audioSourcePlayers[i]->audioDeviceStopped ();
}
//=========================================================================================
*/