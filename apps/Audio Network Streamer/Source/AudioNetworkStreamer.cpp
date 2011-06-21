/*
  ==============================================================================

    AudioNetworkStreamer.cpp
    Created: 20 May 2011 10:44:54am
    Author:  David Rowland

  ==============================================================================
*/

#include "AudioNetworkStreamer.h"

//==============================================================================
InterprocessCommsDemo::InterprocessCommsDemo()
:	isConnected(false),
	isSender(true),
	compressAudio(false),
	settingsTree(Settings::getInstance()->getValueTree()),
	audioBufferL(220500),
	audioBufferR(220500)
{
	server = new DemoInterprocessConnectionServer (*this);
 	
	initialiseAudio();

	// local copy of settings
	compressAudio = settingsTree[SettingsNames[Settings::compress]];//compressAudioButton.getToggleState();
	
	settingsTree.addListener(this);
}

InterprocessCommsDemo::~InterprocessCommsDemo()
{
	close();
	
	ScopedPointer<XmlElement> audioXml(Settings::getInstance()->audioManager->createStateXml());
	if (&*audioXml != nullptr) {
		settingsTree.setProperty(SettingsNames[Settings::audioSettings], audioXml->createDocument(""), nullptr);
	}
	
	settingsTree.removeListener(this);
}

//==============================================================================
// Just closes any connections that are currently open.
void InterprocessCommsDemo::close()
{
	server->stop();
	activeConnections.clear();
	
	settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::disconnected, nullptr);
	//status->setStatus(StatusComponent::disconnected);//(TextButton::buttonColourId, Colours::red);
}

void InterprocessCommsDemo::open (bool asSender)
{
	isSender = asSender;
	close();
	
	// and try to open the socket or pipe...
	bool openedOk = false;
	
	if (asSender)
	{
		// if we're connecting to an existing server, we can just create a connection object
		// directly.
		DemoInterprocessConnection* newConnection = new DemoInterprocessConnection (*this);
		
		openedOk = newConnection->connectToSocket (settingsTree[SettingsNames[Settings::host]].toString(),//socketHost->getText(),
												   int(settingsTree[SettingsNames[Settings::port]]),
												   1000);
		
		if (openedOk)
		{
			activeConnections.add (newConnection);
			settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::waiting, nullptr);
			//status->setStatus(StatusComponent::waiting);//setColour(TextButton::buttonColourId, Colours::orange);
		}
		else
			delete newConnection;
	}
	else
	{
		// if we're starting up a server, we need to tell the server to start waiting for
		// clients to connect. It'll then create connection objects for us when clients arrive.
		openedOk = server->beginWaitingForSocket (settingsTree[SettingsNames[Settings::port]]);
		
		if (openedOk)
		{
			settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::waiting, nullptr);
			//status->setStatus(StatusComponent::waiting);//setColour(TextButton::buttonColourId, Colours::orange);
		}
	}
	
	if (! openedOk)
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 "Audio Network Streamer",
									 "Failed to open the socket...");
		settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::disconnected, nullptr);
		//status->setStatus(StatusComponent::disconnected);//setColour(TextButton::buttonColourId, Colours::red);
	}
}

//==============================================================================
void InterprocessCommsDemo::initialiseAudio()
{
	ScopedPointer<XmlElement> savedSettings(XmlDocument::parse(settingsTree[SettingsNames[Settings::audioSettings]]));
	const String error (Settings::getInstance()->audioManager->initialise (2, /* number of input channels */
																		   2, /* number of output channels */
																		   savedSettings, /* XML settings.. */
																		   true  /* select default device on failure */));
	
	if (error.isNotEmpty())
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 "Audio Error",
									 "Couldn't open an output device!\n\n" + error);
	}
}

void InterprocessCommsDemo::audioDeviceIOCallback (const float** inputChannelData,
												   int numInputChannels,
												   float** outputChannelData,
												   int numOutputChannels,
												   int numSamples)
{
	if (isSender)
	{
		if (isConnected)
		{
			MemoryBlock audioData;

			const int audioBlockSize = jlimit(0, 2, numInputChannels) * numSamples * sizeof(float);
			AudioBlockHeader header = {
				compressAudio,
				audioBlockSize,
				numInputChannels,
				numSamples
			};
			audioData.append(&header, sizeof(AudioBlockHeader));

			if (compressAudio)
			{
				MemoryBlock rawData;
				if (numInputChannels > 0)
					rawData.append(inputChannelData[0], numSamples * sizeof(float));
				if (numInputChannels > 1)
					rawData.append(inputChannelData[1], numSamples * sizeof(float));	
				
				MemoryBlock compressedData;
				compressMemoryBlock(rawData, compressedData);
				
				audioData.append(compressedData.getData(), compressedData.getSize());
			}
			else
			{
				if (numInputChannels > 0)
					audioData.append(inputChannelData[0], numSamples * sizeof(float));
				if (numInputChannels > 1)
					audioData.append(inputChannelData[1], numSamples * sizeof(float));
			}
			
			for (int i = activeConnections.size(); --i >= 0;)
			{
				if (! activeConnections[i]->sendMessage (audioData))
				{
					DBG("write failed");
				}
			}
		}
	}
	else if (isConnected) //reciever
	{
		if (numOutputChannels > 0)
		{
			if(audioBufferL.getNumInUse() >= numSamples)
				audioBufferL.readSamples(outputChannelData[0], numSamples);
			else
				zeromem(outputChannelData[0], numSamples * sizeof(float));

			if (numOutputChannels > 1 && audioBufferR.getNumInUse() >= numSamples)
				audioBufferR.readSamples(outputChannelData[1], numSamples);
			else
				zeromem(outputChannelData[1], numSamples * sizeof(float));
		}
	}
	else
	{
		for (int i = 0; i < numOutputChannels; i++) {
			zeromem(outputChannelData[i], numSamples * sizeof(float));
		}
	}
}

void InterprocessCommsDemo::audioDeviceAboutToStart (AudioIODevice* device)
{
	DBG("audio device started");
}

void InterprocessCommsDemo::audioDeviceStopped()
{
	DBG("audio device stopped");
}

//==============================================================================
void InterprocessCommsDemo::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
	DBG("InterprocessCommsDemo: "<<property.toString());

	if (property == SettingsNames[Settings::compress])
	{
		DBG("compress button clicked");
		compressAudio = settingsTree[SettingsNames[Settings::compress]];
	}
	else if(property == SettingsNames[Settings::mode])
	{
		DBG("mode changed");
		const int modeId = settingsTree[SettingsNames[Settings::mode]];		
		close();
		
		if (modeId > 1 && modeId < 4)
		{
			open ((modeId == 3) ? true : false);
		}		
	}
}

//==============================================================================
InterprocessCommsDemo::DemoInterprocessConnection::DemoInterprocessConnection (InterprocessCommsDemo& owner_)
:	InterprocessConnection (true),
	owner (owner_)
{
	static int totalConnections = 0;
	ourNumber = ++totalConnections;
}

InterprocessCommsDemo::DemoInterprocessConnection::~DemoInterprocessConnection()
{
	Settings::getInstance()->audioManager->removeAudioCallback(&owner);
}

void InterprocessCommsDemo::DemoInterprocessConnection::connectionMade()
{
	Settings::getInstance()->audioManager->addAudioCallback(&owner);
	owner.isConnected = true;
	
	if (owner.isSender) 
	{
		owner.settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::connectedSender, nullptr);
		//owner.status->setStatus(StatusComponent::connectedSender);
	}
	else {
		owner.settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::connectedReciever, nullptr);
		//owner.status->setStatus(StatusComponent::connectedReciever);
	}
}

void InterprocessCommsDemo::DemoInterprocessConnection::connectionLost()
{
	Settings::getInstance()->audioManager->removeAudioCallback(&owner);
	owner.isConnected = false;
	
	if (owner.isSender)
		owner.settingsTree.setProperty(SettingsNames[Settings::mode], 1, nullptr);

	owner.settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::waiting, nullptr);
	//owner.status->setStatus(StatusComponent::waiting);
}

void InterprocessCommsDemo::DemoInterprocessConnection::messageReceived (const MemoryBlock& message)
{
	const AudioBlockHeader &header = *static_cast<AudioBlockHeader*>(message.getData());
	void* startOfAudioBlock = addBytesToPointer(message.getData(), sizeof(AudioBlockHeader));
	size_t sizeOfAudioBlock = message.getSize()-sizeof(AudioBlockHeader);
	
	MemoryBlock decompressedBlock;
	if (header.isCompressed)
	{
		decompressMemoryWithKnownSize(startOfAudioBlock, sizeOfAudioBlock, decompressedBlock, header.uncompressedBlockSize);
		startOfAudioBlock = decompressedBlock.getData();
	}
	
	if (header.numChannels > 0)
	{
		const int bytesPerChannel = header.numSamples * sizeof(float);
		
		owner.audioBufferL.writeSamples((float*)startOfAudioBlock, header.numSamples);
		
		if (header.numChannels > 1)
			owner.audioBufferR.writeSamples((float*)addBytesToPointer(startOfAudioBlock, bytesPerChannel), header.numSamples);
	}
}
	
//==============================================================================
InterprocessCommsDemo::DemoInterprocessConnectionServer::DemoInterprocessConnectionServer (InterprocessCommsDemo& owner_)
:	owner (owner_)
{
}

InterprocessCommsDemo::DemoInterprocessConnectionServer::~DemoInterprocessConnectionServer()
{
}

InterprocessConnection* InterprocessCommsDemo::DemoInterprocessConnectionServer::createConnectionObject()
{
	DemoInterprocessConnection* newConnection = new DemoInterprocessConnection (owner);
	
	owner.activeConnections.add (newConnection);
	return newConnection;
}