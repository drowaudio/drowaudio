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
:	deckManager(DeckManager::getInstance())//,
//	currentMixer(0)
{
//	MixerSettings::getInstance();
	
	// set up the main audio device
	mainAudioDeviceManager.initialise (1, /* number of input channels */
									   2, /* number of output channels */
									   0, /* no XML settings.. */
									   true  /* select default device on failure */);
	mainAudioDeviceManager.addAudioCallback (this);
	
	currentSampleRate = (mainAudioDeviceManager.getCurrentAudioDevice())->getCurrentSampleRate();
	currentBuffSize = mainAudioDeviceManager.getCurrentAudioDevice()->getCurrentBufferSizeSamples();

	buffer = new AudioSampleBuffer(2, currentBuffSize);

//	monitorAudioCallback = new MonitorAudioCallback(this);
//	// set up the main audio device
//	String error = monitorAudioDeviceManager.initialise (0, /* number of input channels */
//														 2, /* number of output channels */
//														 0, /* no XML settings.. */
//														 false  /* select default device on failure */);
//	DBG(error);
//	if (error == String::empty) {
//		monitorAudioDeviceManager.addAudioCallback (monitorAudioCallback);
//	}
	


	
	for (int i = 0; i < deckManager->getMaxNoDecks(); i++) {
		audioSourcePlayers.add(new AudioSourcePlayer());
		audioSourcePlayers[i]->setSource(deckManager->getDeck(i)->getMainFilePlayer());
		audioSourcePlayers[i]->audioDeviceAboutToStart (mainAudioDeviceManager.getCurrentAudioDevice());
	}
}

AudioEngine::~AudioEngine()
{
	// zero the device manager and source player to avoid crashing
	mainAudioDeviceManager.removeAudioCallback (this);
//	monitorAudioDeviceManager.removeAudioCallback (monitorAudioCallback);

	audioSourcePlayers.clear();
//	audioSourcePlayer.setSource (0);
}

void AudioEngine::audioDeviceIOCallback (const float** inputChannelData,
										 int totalNumInputChannels,
										 float** outputChannelData,
										 int totalNumOutputChannels,
										 int numSamples)
{		
	// pass the audio callback on to our player source
//	audioSourcePlayer.audioDeviceIOCallback (inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
	
	if (currentBuffSize != numSamples)
	{
		currentBuffSize = numSamples;
		buffer->setSize(totalNumOutputChannels, currentBuffSize, false, false, true);
		DBG("Changing size");
	}
	buffer->clear();
	
	for (int i=0; i < audioSourcePlayers.size(); i++)
	{
		// if it is not bypassed
		if (!Settings::getInstance()->getPropertyOfChannel(i, CHANNEL_SETTING(bypass)))
		{
			audioSourcePlayers.getUnchecked(i)->audioDeviceIOCallback (inputChannelData, totalNumInputChannels, outputChannelData, totalNumOutputChannels, numSamples);
				
			// update the meters pre-fader
//			if (currentMixer)
//				currentMixer->updateMeterForChannel(i, outputChannelData, numSamples, totalNumOutputChannels);
			
			// apply xFader gain
//			if (int(deckManager->getXFaderSettingVar(DeckManager::assignX)) == (i+1)) {
			if (int(Settings::getInstance()->getPropertyOfXFader(XFADER_SETTING(assignX))) == (i+1)) {
				float xGain = 1.0f-float(Settings::getInstance()->getPropertyOfXFader(XFADER_SETTING(level)));
				for (int c=0; c<totalNumOutputChannels; c++)
					for (int s=0; s<numSamples; s++) {
						outputChannelData[c][s]*=xGain;
					}
			}
//			if (int(deckManager->getXFaderSettingVar(DeckManager::assignY)) == (i+1)) {
			if (int(Settings::getInstance()->getPropertyOfXFader(XFADER_SETTING(assignY))) == (i+1)) {
				float yGain = float(Settings::getInstance()->getPropertyOfXFader(XFADER_SETTING(level)));
				for (int c=0; c<totalNumOutputChannels; c++)
					for (int s=0; s<numSamples; s++) {
						outputChannelData[c][s]*=yGain;
					}
			}

			// add the samples to the output buffer
			for (int c=0; c<totalNumOutputChannels; c++)
				buffer->addFrom(c, 0, outputChannelData[c], numSamples, Settings::getInstance()->getPropertyOfChannel(i, CHANNEL_SETTING(level)));
		}
	}

	// apply master fader level
	buffer->applyGain(0, numSamples, Settings::getInstance()->getPropertyOfMaster(MASTER_SETTING(gain)));

	for (int c=0; c < totalNumOutputChannels; c++) {
		for (int s=0; s<numSamples; s++) {
			outputChannelData[c][s] = buffer->getArrayOfChannels()[c][s];
		}
	}
	
//	if (currentMixer)
//		currentMixer->updateMasterMeter(outputChannelData, numSamples, totalNumOutputChannels);
}

void AudioEngine::audioDeviceAboutToStart (AudioIODevice* device)
{
	DBG("Audio Manager device about to start");
//	audioSourcePlayer.audioDeviceAboutToStart (device);
	for (int i=0; i < audioSourcePlayers.size(); i++)
		audioSourcePlayers[i]->audioDeviceAboutToStart (device);
}

void AudioEngine::audioDeviceStopped()
{
	DBG("Audio Manager device stopped");
//	audioSourcePlayer.audioDeviceStopped();
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