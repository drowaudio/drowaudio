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
#include "StatusComp.h"
#include "CompressionFunctions.h"
#include "dRowAudio_AudioOscilloscope.h"
#include "dRowAudio_FIFOBuffer.h"

//==============================================================================
class InterprocessCommsDemo  :	public ValueTree::Listener,
								public AudioIODeviceCallback
{
public:
    //==============================================================================
    InterprocessCommsDemo();
	
    ~InterprocessCommsDemo();
	
	//==============================================================================
    void close();
	
    void open (bool asSender);
	
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
	void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved) {}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) {}
	
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
		Settings::CompressionLevel compressionLevel;
		int uncompressedBlockSize;
		int numChannels;
		int numSamples;
	};
	
	friend class DemoInterprocessConnection;
	
	OwnedArray <DemoInterprocessConnection, CriticalSection> activeConnections;
	bool isConnected;
	bool isSender;
	Settings::CompressionLevel compressionLevel;
	
	ValueTree settingsTree;
	
	ScopedPointer<DemoInterprocessConnectionServer> server;
	FIFOBuffer audioBufferL, audioBufferR;
	
	// sender
	MemoryBlock audioData;
	MemoryBlock sampleData;
	MemoryBlock compressedData;
	MemoryBlock shortData;
	
	// reciever
	MemoryBlock decompressedBlock;
	MemoryBlock shortBlock;
};

#endif  // __AUDIONETWORKSTREAMER_H_C786D4B4__
