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

class AudioEngine : public DeletedAtShutdown
{
public:
	
	juce_DeclareSingleton (AudioEngine, false)
	
	AudioEngine();
	
	~AudioEngine();
		
	//=================================================================
	AudioDeviceManager* getMainAudioDeviceManager()		{	return &mainAudioDeviceManager;	}
//	AudioDeviceManager* getMonitorAudioDeviceManager()	{	return &monitorAudioDeviceManager;	}
	
	void setCurrentMixer(Mixer *mixer)			{	currentMixer = mixer;		}

	//=================================================================	
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property);
	void valueTreeChildrenChanged (ValueTree &treeWhoseChildHasChanged) {}
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) {}
		
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
		ScopedPointer<AudioSampleBuffer> buffer;
		OwnedArray<AudioSourcePlayer> audioSourcePlayers;
		
		CriticalSection lock;
	};
	
//	class MonitorAudioCallback : public AudioIODeviceCallback
//	{
//	public:
//		MonitorAudioCallback(AudioEngine *mainEngine);
//		~MonitorAudioCallback();
//		
//		void audioDeviceIOCallback (const float** inputChannelData,
//									int totalNumInputChannels,
//									float** outputChannelData,
//									int totalNumOutputChannels,
//									int numSamples);
//		
//		void audioDeviceAboutToStart (AudioIODevice* device);	
//		
//		void audioDeviceStopped();
//		
//	private:
//		friend class AudioEngine;
//		DeckManager *deckManager;
//		AudioEngine *mainEngine;
//		AudioDeviceManager monitorAudioDeviceManager;
//		
//		double currentSampleRate;
//		int currentBuffSize;
//		OwnedArray<AudioSourcePlayer> audioSourcePlayers;
//		ScopedPointer<AudioSampleBuffer> monitorBuffer;
//	};
	//=================================================================
	
	DeckManager *deckManager;
	Mixer *currentMixer;
	
	// classes for audio playback
	AudioDeviceManager mainAudioDeviceManager;
	ScopedPointer<MainAudioCallback> mainAudioCallback;
	//	AudioDeviceManager monitorAudioDeviceManager;
	//	ScopedPointer<MonitorAudioCallback> monitorAudioCallback;
	
	// mixer settings
};



#endif //_DECKS_AUDIOENGINE__H_