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

class AudioEngine : public ChangeListener,
					public DeletedAtShutdown
{
public:
	
	juce_DeclareSingleton (AudioEngine, false)
	
	AudioEngine();
	
	~AudioEngine();
		
	//=================================================================
	AudioDeviceManager* getMainAudioDeviceManager()		{	return &mainAudioDeviceManager;	}
	AudioDeviceManager* getMonitorAudioDeviceManager()	{	return &monitorAudioDeviceManager;	}
	
	void setCurrentMixer(Mixer *mixer)			{	currentMixer = mixer;		}
		
	void changeListenerCallback (ChangeBroadcaster* source);

	//=================================================================

private:
	
	//=================================================================
	class MainAudioCallback : public AudioIODeviceCallback
	{
	public:
		MainAudioCallback();
		~MainAudioCallback();
		
		void audioDeviceIOCallback (const float** inputChannelData,
									int totalNumInputChannels,
									float** outputChannelData,
									int totalNumOutputChannels,
									int numSamples);
		
		void audioDeviceAboutToStart (AudioIODevice* device);	
		
		void audioDeviceStopped();
		
	private:
		friend class AudioEngine;
		
		int currentBuffSize;
		OwnedArray<AudioSourcePlayer> audioSourcePlayers;
		AudioSampleBuffer buffer;
		AudioSampleBuffer monitorBuffer;
	};
	
	class MonitorAudioCallback : public AudioIODeviceCallback
	{
	public:
		MonitorAudioCallback();
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
	};
	
	//=================================================================
	class MonitorStruct
	{
	public:
		MonitorStruct()
		:	monitorCircularBufferL(513),
			needToMonitor(false),
			numMainSamples(128),
			numMonitorSamples(128)
		{
		}

		inline void setNumMonitorSamples(int numSamples)
		{
			numMonitorSamples = numSamples;
			resizeBuffer();
		}
		inline void setNumMainSamples(int numSamples)
		{
			numMainSamples = numSamples;
			resizeBuffer();
		}
		
		FIFOBuffer monitorCircularBufferL;
		bool needToMonitor;
			
	private:
		
		inline void resizeBuffer()
		{
			monitorCircularBufferL.setSize(jmax(numMonitorSamples, numMainSamples)+1);
		}
		int numMainSamples;
		int numMonitorSamples;

	};
	//=================================================================
	
	Mixer *currentMixer;
		
	// classes for audio playback
	AudioDeviceManager mainAudioDeviceManager;
	ScopedPointer<MainAudioCallback> mainAudioCallback;
	AudioDeviceManager monitorAudioDeviceManager;
	ScopedPointer<MonitorAudioCallback> monitorAudioCallback;

	MonitorStruct monitor;
};



#endif //_DECKS_AUDIOENGINE__H_