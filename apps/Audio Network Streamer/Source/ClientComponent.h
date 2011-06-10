/*
  ==============================================================================

    ClientComponent.h
    Created: 20 May 2011 6:03:07am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __CLIENTCOMPONENT_H_85C8B797__
#define __CLIENTCOMPONENT_H_85C8B797__

#include <dRowAudio/dRowAudio.h>
#include "Connections.h"

class ClientComponent :	public Component,
						public AudioIODeviceCallback,
						public NetworkAudio
{
public:
	ClientComponent();
	
	~ClientComponent();
	
	void resized();
	
	void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
	
    void audioDeviceAboutToStart (AudioIODevice* device);
	
    void audioDeviceStopped();
	
	void connectionMade()
	{
		DBG("client connectionMade");
	}
	
	void connectionLost()
	{
		DBG("client connectionLost");
	}
	
	void messageReceived (const MemoryBlock& message)
	{
		DBG("client messageReceived");
	}
	
private:
	
	AudioDeviceManager audioManager;
	ScopedPointer<AudioDeviceSelectorComponent> audioSelector;
	AudioOscilliscope scope;
	
	//OwnedArray <DemoInterprocessConnection, CriticalSection> activeConnections;
	ScopedPointer<AudioInterprocessConnectionServer> server;
	AudioInterprocessConnection *connection;
};

#endif  // __CLIENTCOMPONENT_H_85C8B797__
