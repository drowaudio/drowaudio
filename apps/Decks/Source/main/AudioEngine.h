/*
 *  AudioEngine.h
 *  Decks
 *
 *  Created by David Rowland on 15/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DECKS_AUDIOENGINE__H_
#define _DECKS_AUDIOENGINE__H_

#include <juce/juce.h>
#include "DeckManager.h"
#include "Settings.h"
#include "MixerSettings.h"
#include "../ui/controls/mixer/Mixer.h"

class AudioEngine : public AudioIODeviceCallback
{
public:
	
	juce_DeclareSingleton (AudioEngine, false)
	
	AudioEngine();
	
	~AudioEngine();
	
	void audioDeviceIOCallback (const float** inputChannelData,
								int totalNumInputChannels,
								float** outputChannelData,
								int totalNumOutputChannels,
								int numSamples);
	
	void audioDeviceAboutToStart (AudioIODevice* device);	
	
	void audioDeviceStopped();
	
	//=================================================================
	AudioDeviceManager* getMainAudioDeviceManager()		{	return &mainAudioDeviceManager;	}
	AudioDeviceManager* getMonitorAudioDeviceManager()	{	return &monitorAudioDeviceManager;	}
	
	void setCurrentMixer(Mixer *mixer)			{	currentMixer = mixer;		}
	//=================================================================	
	
private:
	
	//=================================================================
	class MonitorAudioCallback : public AudioIODeviceCallback
	{
	public:
		MonitorAudioCallback(AudioEngine *mainEngine);
		~MonitorAudioCallback();
		
		void audioDeviceIOCallback (const float** inputChannelData,
									int totalNumInputChannels,
									float** outputChannelData,
									int totalNumOutputChannels,
									int numSamples);
		
		void audioDeviceAboutToStart (AudioIODevice* device);	
		
		void audioDeviceStopped();
		
	private:
		friend class AudioEngine;
		DeckManager *deckManager;
		AudioEngine *mainEngine;
		AudioDeviceManager monitorAudioDeviceManager;
		
		double currentSampleRate;
		int currentBuffSize;
		OwnedArray<AudioSourcePlayer> audioSourcePlayers;
		ScopedPointer<AudioSampleBuffer> monitorBuffer;
	};
	//=================================================================

	DeckManager *deckManager;

	// classes for audio playback
	AudioDeviceManager mainAudioDeviceManager;
	AudioDeviceManager monitorAudioDeviceManager;
	MonitorAudioCallback *monitorAudioCallback;
	
	OwnedArray<AudioSourcePlayer> audioSourcePlayers;
	ScopedPointer<AudioSampleBuffer> buffer;
//	MixerAudioSource mixerSource;
//	AudioSourcePlayer audioSourcePlayer;
	
	double currentSampleRate;
	int currentBuffSize;
	
	Component::SafePointer<Mixer> currentMixer;
};



#endif //_DECKS_AUDIOENGINE__H_