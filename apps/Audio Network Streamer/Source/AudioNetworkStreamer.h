/*
  ==============================================================================

    AudioNetworkStreamer.h
    Created: 20 May 2011 10:44:54am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __AUDIONETWORKSTREAMER_H_C786D4B4__
#define __AUDIONETWORKSTREAMER_H_C786D4B4__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "dRowAudio_AudioOscilloscope.h"
#include "dRowAudio_FIFOBuffer.h"

//==============================================================================
class InterprocessCommsDemo  :	public Component,
								public AsyncUpdater,
								public Button::Listener,
								public ComboBox::Listener,
								public AudioIODeviceCallback
{
public:
    //==============================================================================
    InterprocessCommsDemo();
	
    ~InterprocessCommsDemo();
	
	void resized();
	
	void handleAsyncUpdate();
	
    void buttonClicked (Button* button);
	
    void comboBoxChanged (ComboBox*);
	
    void close();
	
    void open (bool asSender);
	
    void appendMessage (const String& message);
	
    //==============================================================================
	void initialiseAudio();
	
	void audioDeviceIOCallback (const float** inputChannelData,
                                int numInputChannels,
                                float** outputChannelData,
                                int numOutputChannels,
                                int numSamples);
	
    void audioDeviceAboutToStart (AudioIODevice* device);
	
    void audioDeviceStopped();
	
    //==============================================================================
    class DemoInterprocessConnection  : public InterprocessConnection
    {
        InterprocessCommsDemo& owner;
        int ourNumber;
		
    public:
        DemoInterprocessConnection (InterprocessCommsDemo& owner_);
		
        ~DemoInterprocessConnection();
		
        void connectionMade();
		
        void connectionLost();
		
        void messageReceived (const MemoryBlock& message);
    };
	
    //==============================================================================
    class DemoInterprocessConnectionServer   : public InterprocessConnectionServer
    {
        InterprocessCommsDemo& owner;
		
    public:
        DemoInterprocessConnectionServer (InterprocessCommsDemo& owner_);
		
        ~DemoInterprocessConnectionServer();
		
        InterprocessConnection* createConnectionObject();
    };

    //==============================================================================
		
private:
	
	struct AudioBlockHeader
	{
		int blockSize;
		int isCompressed;
		int numChannels;
		int numSamples;
	};
	
	friend class DemoInterprocessConnection;
	OwnedArray <DemoInterprocessConnection, CriticalSection> activeConnections;
	bool isConnected;
	bool isSender;
	bool sendAudio;	
	int compressAudio;
	
	ValueTree settingsTree;
	
    ComboBox *modeSelector;
    TextEditor *incomingMessages;
	
    TextEditor *socketNumber;
    TextEditor *socketHost;
		
	AudioDeviceManager audioManager;
	TextButton audioSettingsButton;
	ToggleButton compressAudioButton;
	
	ScopedPointer<DemoInterprocessConnectionServer> server;
	ScopedPointer<AudioData> audioBlock;
	FIFOBuffer audioBufferL, audioBufferR;
};

#endif  // __AUDIONETWORKSTREAMER_H_C786D4B4__
