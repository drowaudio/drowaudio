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
	compressionLevel(Settings::noCompression),
	settingsTree(Settings::getInstance()->getValueTree()),
	audioBufferL(220500),
	audioBufferR(220500)
{
	server = new DemoInterprocessConnectionServer (*this);
 	
	initialiseAudio();
	Settings::getInstance()->audioManager->closeAudioDevice();

	// local copy of settings
	compressionLevel = static_cast<Settings::CompressionLevel>(int(settingsTree[SettingsNames[Settings::compress]]));
	
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
	DBG("commsDemo destroyed");
}

//==============================================================================
// Just closes any connections that are currently open.
void InterprocessCommsDemo::close()
{
	server->stop();
	activeConnections.clear();
	
	settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::disconnected, nullptr);
}

void InterprocessCommsDemo::open (bool asSender)
{
	isSender = asSender;
	close();
	
	// and try to open the socket or pipe...
	bool openedOk = false;
	
	if (asSender)
	{
		// if we're connecting to an existing server, we can just create a connection object directly
		DemoInterprocessConnection* newConnection = new DemoInterprocessConnection (*this);
		
		openedOk = newConnection->connectToSocket (settingsTree[SettingsNames[Settings::host]].toString(),
												   int(settingsTree[SettingsNames[Settings::port]]),
												   1000);
		
		if (openedOk)
		{
			activeConnections.add (newConnection);
			settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::waiting, nullptr);
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
		}
	}
	
	if (! openedOk)
	{
		settingsTree.setProperty(SettingsNames[Settings::mode], 1, nullptr);
		settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::disconnected, nullptr);

		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 "Audio Network Streamer",
									 "Failed to open the socket...");
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
			// header
			const int actualNumInputChannels = jlimit(0, 2, numInputChannels);
			const int bytesPerChannel = numSamples * sizeof(float);
			const int uncompressedAudioBlockSize = actualNumInputChannels * bytesPerChannel;

			AudioBlockHeader header = {
				compressionLevel,
				uncompressedAudioBlockSize,
				actualNumInputChannels,
				numSamples
			};

			// raw samples
			sampleData.setSize(uncompressedAudioBlockSize, false);

			if (actualNumInputChannels > 0)
				memcpy(sampleData.getData(), inputChannelData[0], bytesPerChannel);
			if (actualNumInputChannels > 1)
				memcpy(addBytesToPointer(sampleData.getData(), bytesPerChannel), inputChannelData[1], bytesPerChannel);

			if (compressionLevel == Settings::noCompression)
			{
				const int maxAudioDataSize = sizeof(AudioBlockHeader) + uncompressedAudioBlockSize;
				audioData.setSize(maxAudioDataSize, false);

				memcpy(audioData.getData(), &header, sizeof(AudioBlockHeader));
				memcpy(addBytesToPointer(audioData.getData(), sizeof(AudioBlockHeader)), sampleData.getData(), sampleData.getSize());
			}
			else if (compressionLevel == Settings::gZipCompression)
			{
				//compressedData.setSize(audioBlockSize, false);
				compressMemoryBlockWithSize(sampleData.getData(), uncompressedAudioBlockSize, compressedData);
				
				const int maxAudioDataSize = sizeof(AudioBlockHeader) + compressedData.getSize();
				audioData.setSize(maxAudioDataSize, false);
				
				memcpy(audioData.getData(), &header, sizeof(AudioBlockHeader));
				memcpy(addBytesToPointer(audioData.getData(), sizeof(AudioBlockHeader)), compressedData.getData(), compressedData.getSize());
			}
			else if(compressionLevel == Settings::shortCompression)
			{
				const int sizeNeededForShortData = actualNumInputChannels * numSamples * sizeof(short);
				const int maxAudioDataSize = sizeof(AudioBlockHeader) + sizeNeededForShortData;
				audioData.setSize(maxAudioDataSize, false);
				memcpy(audioData.getData(), &header, sizeof(AudioBlockHeader));

				short *shortPointer = static_cast<short*>(addBytesToPointer(audioData.getData(), sizeof(AudioBlockHeader)));
				const short maxValue = std::numeric_limits<short>::max();
				for (int c = 0; c < actualNumInputChannels; c++)
				{
					for (int i = 0; i < numSamples; i++)
					{
						*shortPointer++ = jlimit(-1.0f, 1.0f, inputChannelData[c][i]) * maxValue;
					}
				}
			}
			else if(compressionLevel == Settings::shortGzipCompression)
			{
				const int sizeNeededForShortData = actualNumInputChannels * numSamples * sizeof(short);
				shortData.setSize(sizeNeededForShortData, false);
				
				short *shortPointer = static_cast<short*>(shortData.getData());
				const short maxValue = std::numeric_limits<short>::max();
				for (int c = 0; c < actualNumInputChannels; c++)
				{
					for (int i = 0; i < numSamples; i++)
					{
						*shortPointer++ = jlimit(-1.0f, 1.0f, inputChannelData[c][i]) * maxValue;
					}
				}
				
				//compress here
				compressMemoryBlockWithSize(shortData.getData(), sizeNeededForShortData, compressedData);

				const int maxAudioDataSize = sizeof(AudioBlockHeader) + compressedData.getSize();
				audioData.setSize(maxAudioDataSize, false);
				
				memcpy(audioData.getData(), &header, sizeof(AudioBlockHeader));
				memcpy(addBytesToPointer(audioData.getData(), sizeof(AudioBlockHeader)), compressedData.getData(), compressedData.getSize());
			}
			else if(compressionLevel == Settings::shortDerivativeCompression)
			{
				const int sizeNeededForShortData = actualNumInputChannels * numSamples * sizeof(short);
				shortData.setSize(sizeNeededForShortData, false);
				
				short *shortPointer = static_cast<short*>(shortData.getData());
				const short maxValue = std::numeric_limits<short>::max();
				for (int c = 0; c < actualNumInputChannels; c++)
				{
					short lastSample = jlimit(-1.0f, 1.0f, inputChannelData[c][0]) * maxValue;
					*shortPointer++ = lastSample;
					
					for (int i = 1; i < numSamples; i++)
					{
						short currentSample = jlimit(-1.0f, 1.0f, inputChannelData[c][i]) * maxValue;
						*shortPointer++ = currentSample - lastSample;
						lastSample = currentSample;
					}
				}
				
				//compress here
				compressMemoryBlockWithSize(shortData.getData(), sizeNeededForShortData, compressedData);
				
				const int maxAudioDataSize = sizeof(AudioBlockHeader) + compressedData.getSize();
				audioData.setSize(maxAudioDataSize, false);
				
				memcpy(audioData.getData(), &header, sizeof(AudioBlockHeader));
				memcpy(addBytesToPointer(audioData.getData(), sizeof(AudioBlockHeader)), compressedData.getData(), compressedData.getSize());
			}
			else
			{
				// just add header and normal
				header.compressionLevel = Settings::noCompression;
				const int maxAudioDataSize = sizeof(AudioBlockHeader) + uncompressedAudioBlockSize;
				audioData.setSize(maxAudioDataSize, false);
				
				memcpy(audioData.getData(), &header, sizeof(AudioBlockHeader));
				memcpy(addBytesToPointer(audioData.getData(), sizeof(AudioBlockHeader)), sampleData.getData(), sampleData.getSize());
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
	if (property == SettingsNames[Settings::compress])
	{
		DBG("compress button clicked");
		compressionLevel = static_cast<Settings::CompressionLevel>(int(settingsTree[SettingsNames[Settings::compress]]));
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
	Settings::getInstance()->audioManager->closeAudioDevice();
}

void InterprocessCommsDemo::DemoInterprocessConnection::connectionMade()
{
	Settings::getInstance()->audioManager->addAudioCallback(&owner);
	Settings::getInstance()->audioManager->restartLastAudioDevice();
	owner.isConnected = true;
	
	if (owner.isSender) 
	{
		owner.settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::connectedSender, nullptr);
	}
	else {
		owner.settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::connectedReciever, nullptr);
	}
}

void InterprocessCommsDemo::DemoInterprocessConnection::connectionLost()
{
	Settings::getInstance()->audioManager->removeAudioCallback(&owner);
	Settings::getInstance()->audioManager->closeAudioDevice();
	owner.isConnected = false;
	
	if (owner.isSender) {
		owner.settingsTree.setProperty(SettingsNames[Settings::mode], 1, nullptr);
		owner.settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::disconnected, nullptr);
	}
	else
		owner.settingsTree.setProperty(SettingsNames[Settings::status], (int)StatusComponent::waiting, nullptr);
}

void InterprocessCommsDemo::DemoInterprocessConnection::messageReceived (const MemoryBlock& message)
{
	const AudioBlockHeader &header = *static_cast<AudioBlockHeader*>(message.getData());
	void* startOfAudioBlock = addBytesToPointer(message.getData(), sizeof(AudioBlockHeader));
	size_t sizeOfAudioBlock = message.getSize()-sizeof(AudioBlockHeader);

	DBG("block size: "<<(int)message.getSize()<<" - cl: "<<header.compressionLevel
		<<" ub: "<<header.uncompressedBlockSize
		<<" nc: "<<header.numChannels
		<<" ns: "<<header.numSamples);

	if (header.compressionLevel == Settings::noCompression)
	{
	}
	else if (header.compressionLevel == Settings::gZipCompression)
	{
		if(owner.decompressedBlock.getSize() < header.uncompressedBlockSize)
			owner.decompressedBlock.ensureSize(header.uncompressedBlockSize, false);
		
		decompressMemoryWithKnownSize(startOfAudioBlock, sizeOfAudioBlock, owner.decompressedBlock, header.uncompressedBlockSize);
		startOfAudioBlock = owner.decompressedBlock.getData();
	}
	else if (header.compressionLevel == Settings::shortCompression)
	{
		if(owner.decompressedBlock.getSize() < header.uncompressedBlockSize)
			owner.decompressedBlock.ensureSize(header.uncompressedBlockSize, false);
		
		short *shortPointer = static_cast<short*> (startOfAudioBlock);
		float *floatPointer = static_cast<float*> (owner.decompressedBlock.getData());
		const double oneOverMaxValue = 1.0 / std::numeric_limits<short>::max();

		for (int c = 0; c < header.numChannels; c++)
		{
			for (int i = 0; i < header.numSamples; i++)
			{
				*floatPointer++ = static_cast<float> (*shortPointer++ * oneOverMaxValue);
			}
		}
		
		startOfAudioBlock = owner.decompressedBlock.getData();
	}
	else if(header.compressionLevel == Settings::shortGzipCompression)
	{
		owner.decompressedBlock.ensureSize(header.uncompressedBlockSize, false);
		const int sizeOfShortBlock = header.numSamples * header.numChannels * sizeof(short);
		owner.shortBlock.ensureSize(sizeOfShortBlock, false);
		
		decompressMemoryWithKnownSize(startOfAudioBlock, sizeOfAudioBlock, owner.shortBlock, sizeOfShortBlock);

		short *shortPointer = static_cast<short*> (owner.shortBlock.getData());
		float *floatPointer = static_cast<float*> (owner.decompressedBlock.getData());
		const double oneOverMaxValue = 1.0 / std::numeric_limits<short>::max();
		
		for (int c = 0; c < header.numChannels; c++)
		{
			for (int i = 0; i < header.numSamples; i++)
			{
				*floatPointer++ = static_cast<float> (*shortPointer++ * oneOverMaxValue);
			}
		}		
		
		startOfAudioBlock = owner.decompressedBlock.getData();
	}
	else if(header.compressionLevel == Settings::shortDerivativeCompression)
	{
		owner.decompressedBlock.ensureSize(header.uncompressedBlockSize, false);
		const int sizeOfShortBlock = header.numSamples * header.numChannels * sizeof(short);
		owner.shortBlock.ensureSize(sizeOfShortBlock, false);
		
		decompressMemoryWithKnownSize(startOfAudioBlock, sizeOfAudioBlock, owner.shortBlock, sizeOfShortBlock);
		
		short *shortPointer = static_cast<short*> (owner.shortBlock.getData());
		float *floatPointer = static_cast<float*> (owner.decompressedBlock.getData());
		const double oneOverMaxValue = 1.0 / std::numeric_limits<short>::max();
		
		for (int c = 0; c < header.numChannels; c++)
		{
			short lastSample = *shortPointer++;
			*floatPointer++ = static_cast<float> (lastSample * oneOverMaxValue);

			for (int i = 1; i < header.numSamples; i++)
			{
				lastSample += *shortPointer++;
				*floatPointer++ = static_cast<float> (lastSample * oneOverMaxValue);
			}
		}		
		
		startOfAudioBlock = owner.decompressedBlock.getData();
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