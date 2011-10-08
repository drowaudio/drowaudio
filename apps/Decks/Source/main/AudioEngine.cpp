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
:	currentMixer(0)
{	
	mainAudioCallback = new MainAudioCallback();

	// find audio device names
	DBG("\nAudio device names:");
	const OwnedArray<AudioIODeviceType> &types (mainAudioDeviceManager.getAvailableDeviceTypes());
	StringArray names;
	for (int i = 0 ; i < types.size(); i++) {
		names.addArray(types[i]->getDeviceNames());
	}
	for (int i = 0; i < names.size(); i++) {
		DBG(names[i]);
	}
	
	// set up the main audio device
	String error = mainAudioDeviceManager.initialise (0, /* number of input channels */
													  2, /* number of output channels */
													  0, /* no XML settings.. */
													  true  /* select default device on failure */);
	if (error.isNotEmpty()) {
		DBG(error);
	}
	mainAudioDeviceManager.addAudioCallback (mainAudioCallback);
	mainAudioDeviceManager.addChangeListener(this);
	DBG("\nInititalised main audio");
	
	// monitor
	monitorAudioCallback = new MonitorAudioCallback();
	DBG("Created monitor callback");
	// set up the main audio device
	if (names.size() > 1) {
		error = monitorAudioDeviceManager.initialise (0, /* number of input channels */
											  2, /* number of output channels */
											  0, /* no XML settings.. */
											 false,  /* select default device on failure */
											 names[1]);
		DBG("Inititalised monitor audio");
		if (error.isNotEmpty())
			DBG(error);
	}
	monitorAudioDeviceManager.addAudioCallback (monitorAudioCallback);
	monitorAudioDeviceManager.addChangeListener(this);
}

AudioEngine::~AudioEngine()
{
	// zero the device manager and source player to avoid crashing
	monitorAudioDeviceManager.removeAudioCallback (monitorAudioCallback);
	monitorAudioDeviceManager.removeChangeListener(this);
	mainAudioDeviceManager.removeAudioCallback (mainAudioCallback);
	mainAudioDeviceManager.removeChangeListener(this);

	DBG("AudioEngine deleted");
}

void AudioEngine::changeListenerCallback (ChangeBroadcaster* source)
{
	DBG("audio device changed");
	if (source == &monitorAudioDeviceManager) {
		if (monitorAudioDeviceManager.getCurrentAudioDevice() == 0)
			monitor.needToMonitor = false;
	}
}

//=========================================================================================
// Main Audio Callback
// This  callback deals with the main audio system
//=========================================================================================
AudioEngine::MainAudioCallback::MainAudioCallback()
:	currentBuffSize(0),
	buffer(2, 512),
	monitorBuffer(2, 512)
{
	for (int i = 0; i < DeckManager::getInstance()->getMaxNoDecks(); i++) {
		audioSourcePlayers.add(new AudioSourcePlayer());
		audioSourcePlayers[i]->setSource(DeckManager::getInstance()->getDeck(i)->getMainFilePlayer());
	}
}

AudioEngine::MainAudioCallback::~MainAudioCallback()
{
//	for (int i=0; i < audioSourcePlayers.size(); i++)
//	{
//		audioSourcePlayers[i]->audioDeviceStopped();
//		audioSourcePlayers[i]->setSource(0);	
//	}
	DBG("Main audio callback deleted");	
}

void AudioEngine::MainAudioCallback::audioDeviceIOCallback (const float** inputChannelData,
															int totalNumInputChannels,
															float** outputChannelData,
															int totalNumOutputChannels,
															int numSamples)
{
	DeckManager *manager = DeckManager::getInstance();
	AudioEngine *engine = AudioEngine::getInstance();
	
	// only need to change the buffer sizes if it needs to be bigger
	if (currentBuffSize != numSamples)
	{
		currentBuffSize = numSamples;
		buffer.setSize(totalNumOutputChannels, currentBuffSize, false, false, true);
		DBG("Changing buffer size: "<<currentBuffSize);
		
		monitorBuffer.setSize(totalNumOutputChannels, currentBuffSize, false, false, true);
		engine->monitor.setNumMainSamples(numSamples);
	}
		
	buffer.clear();
	monitorBuffer.clear();
	
	float yGain = manager->getXFaderSetting(XFADER_SETTING(level));
	float xGain = 1.0f - yGain;

	int assignX = manager->getXFaderSetting(XFADER_SETTING(assignX));
	int assignY = manager->getXFaderSetting(XFADER_SETTING(assignY));
	
	float masterGain = manager->getMasterSetting(MASTER_SETTING(gain));
	
	for (int i = 0; i < audioSourcePlayers.size(); i++)
	{		
		if (manager->getDeckSetting(i, CHANNEL_SETTING(on)))
		{
			bool bypassed = manager->getDeckSetting(i, CHANNEL_SETTING(bypass));
			bool cue = manager->getDeckSetting(i, CHANNEL_SETTING(cue));
			float channelLevel = manager->getDeckSetting(i, CHANNEL_SETTING(level));
						
			// if it is not bypassed
			if (bypassed == false)
			{
				audioSourcePlayers.getUnchecked(i)->audioDeviceIOCallback (inputChannelData, totalNumInputChannels,
																		   outputChannelData, totalNumOutputChannels,
																		   numSamples);

				if (cue == true) {
					for (int c = 0; c < totalNumOutputChannels; c++)
						monitorBuffer.addFrom(c, 0, outputChannelData[c], numSamples);
				}
					
				if (engine->currentMixer != nullptr)
					engine->currentMixer->updateMeterForChannel(i, outputChannelData, numSamples, totalNumOutputChannels);
								
				if (channelLevel != 0.0f)
				{
					// apply xFader gain
					if (assignX == (i + 1)) {
						for (int c = 0; c < totalNumOutputChannels; c++)
							for (int s = 0; s < numSamples; s++) {
								outputChannelData[c][s] *= xGain;
							}
					}
					if (assignY == (i + 1)) {
						for (int c = 0; c < totalNumOutputChannels; c++)
							for (int s = 0; s < numSamples; s++) {
								outputChannelData[c][s] *= yGain;
							}
					}
					
					// add the samples to the output buffer
					for (int c = 0; c < totalNumOutputChannels; c++)
						buffer.addFrom(c, 0, outputChannelData[c], numSamples, channelLevel);
				}
			}
			else // if bypassed we still need to update playhead positions
			{
				AudioTransportSource *player = manager->getDeck(i)->getMainFilePlayer()->getAudioTransportSource();
				if (player->isPlaying() && (player->getNextReadPosition() < player->getTotalLength()))
					player->setNextReadPosition(player->getNextReadPosition() + numSamples);
			}
		}
	}
	
	// add monitor buffer to CircularBuffer here
	if (engine->monitor.needToMonitor == true)
	{
		float monGain = manager->getMasterSetting(MASTER_SETTING(monitorGain));
		monitorBuffer.applyGain(0, numSamples, monGain);
		engine->monitor.monitorCircularBufferL.writeSamples(monitorBuffer.getArrayOfChannels()[0], numSamples);
	}
	
	// apply master gain level
	buffer.applyGain(0, numSamples, masterGain);
	
	// copy local buffer to output buffer
    float** bufferChannelArray = buffer.getArrayOfChannels();
	for (int c = 0; c < totalNumOutputChannels; c++) {
		for (int s = 0; s < numSamples; s++) {
			outputChannelData[c][s] = bufferChannelArray[c][s];
		}
	}	

	if (engine->currentMixer != nullptr)
		engine->currentMixer->updateMasterMeter(outputChannelData, numSamples, totalNumOutputChannels);
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

	for (int i = 0; i < audioSourcePlayers.size(); i++)
		audioSourcePlayers[i]->audioDeviceStopped ();	
}


//=========================================================================================
// Monitor Audio Callback
// This second callback deals with the monitoring system
//=========================================================================================
AudioEngine::MonitorAudioCallback::MonitorAudioCallback()
{
}

AudioEngine::MonitorAudioCallback::~MonitorAudioCallback()
{
}

//=========================================================================================
void AudioEngine::MonitorAudioCallback::audioDeviceIOCallback (const float** inputChannelData,
															   int totalNumInputChannels,
															   float** outputChannelData,
															   int totalNumOutputChannels,
															   int numSamples)
{
	AudioEngine *engine = AudioEngine::getInstance();
	if (engine->monitor.needToMonitor == false) {
		engine->monitor.needToMonitor = true;
		engine->monitor.setNumMonitorSamples(numSamples);
	}
	
//	float numToDo = engine->monitor.monitorCircularBufferL.getNumAvailable() / (float)numSamples;
//	if (numToDo > 1) {
//		DBG("num to do: "<<numToDo);
//	}
	
	int numDone = 0;
	while (engine->monitor.monitorCircularBufferL.getNumAvailable() >= numSamples) {
		engine->monitor.monitorCircularBufferL.readSamples(outputChannelData[0] + numDone, numSamples);
		numDone += numSamples;
	}
	if (totalNumOutputChannels > 1)
		zeromem(outputChannelData[1], numSamples * sizeof (float));
	
	// need to do sample rate conversion here
}

void AudioEngine::MonitorAudioCallback::audioDeviceAboutToStart (AudioIODevice* device)
{
	DBG("Monitor Audio Manager device started");
}

void AudioEngine::MonitorAudioCallback::audioDeviceStopped()
{
	DBG("Monitor Audio Manager device stopped");
}
//=========================================================================================
