/*
  ==============================================================================

    ServerComponent.h
    Created: 20 May 2011 6:02:55am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __SERVERCOMPONENT_H_1BDC211F__
#define __SERVERCOMPONENT_H_1BDC211F__

#include <dRowAudio/dRowAudio.h>
#include "Connections.h"

class ServerComponent :	public Component,
						public AudioIODeviceCallback,
						public NetworkAudio
{
public:
	ServerComponent();
	
	~ServerComponent();
	
	void resized();
	
//	void setConnection()
		
	void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
	
    void audioDeviceAboutToStart (AudioIODevice* device);
	
    void audioDeviceStopped();

	void connectionMade()
	{
		DBG("server connectionMade");
	}
	
	void connectionLost()
	{
		DBG("server connectionLost");
	}
	
	void messageReceived (const MemoryBlock& message)
	{
		DBG("server messageReceived");
	}
	
private:
	
	TextEditor hostEditor, portEditor;
	TextButton connectButton;
	
	AudioDeviceManager audioManager;
	ScopedPointer<AudioDeviceSelectorComponent> audioSelector;
	AudioOscilliscope scope;
	
	ScopedPointer<AudioInterprocessConnectionServer> server;
};

#endif  // __SERVERCOMPONENT_H_1BDC211F__
