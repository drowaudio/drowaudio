/*
  ==============================================================================

    AudioNetworkStreamer.cpp
    Created: 20 May 2011 10:44:54am
    Author:  David Rowland

  ==============================================================================
*/

#include "AudioNetworkStreamer.h"

//==============================================================================
void compressMemoryBlock(const MemoryBlock &sourceBlock, MemoryBlock &destinationBlock)
{
	MemoryOutputStream memoryStream(destinationBlock, true);
	GZIPCompressorOutputStream zipStream(&memoryStream);
	
	zipStream.write(sourceBlock.getData(), sourceBlock.getSize());
	zipStream.flush();
	
	DBG("Original: "<<(int)sourceBlock.getSize()<<" - Comp: "<<(int)destinationBlock.getSize()<<" - Ratio: "<<(destinationBlock.getSize()/(float)sourceBlock.getSize()));
}

void decompressMemoryBlock(const MemoryBlock &sourceBlock, MemoryBlock &destinationBlock, const int bufferSize =8192)
{
	MemoryInputStream memoryStream(sourceBlock, false);
	GZIPDecompressorInputStream zipStream(memoryStream);
		
	destinationBlock.ensureSize(bufferSize, false);
	void* currentPos = destinationBlock.getData();
	int64 totalSize = 0;
	while (! zipStream.isExhausted())
	{
		int64 bytesRead = zipStream.read(currentPos, bufferSize);
		totalSize += bytesRead;
		currentPos = addBytesToPointer(currentPos, bytesRead);
		destinationBlock.ensureSize(destinationBlock.getSize()+bufferSize, false);
	}
	destinationBlock.setSize(totalSize, false);
	
	DBG("Comp: "<<(int)sourceBlock.getSize()<<" - Original: "<<(int)destinationBlock.getSize()<<" - Ratio: "<<(destinationBlock.getSize()/(float)sourceBlock.getSize())<<"\n");
}

void decompressMemory(const void *sourceBlock, size_t sourceBlockSize, MemoryBlock &destinationBlock, const int bufferSize =8192)
{
	MemoryInputStream memoryStream(sourceBlock, sourceBlockSize, false);
	GZIPDecompressorInputStream zipStream(memoryStream);
	
	destinationBlock.ensureSize(bufferSize, false);
	void* currentPos = destinationBlock.getData();
	int64 totalSize = 0;
	while (! zipStream.isExhausted())
	{
		int64 bytesRead = zipStream.read(currentPos, bufferSize);
		totalSize += bytesRead;
		currentPos = addBytesToPointer(currentPos, bytesRead);
		destinationBlock.ensureSize(destinationBlock.getSize()+bufferSize, false);
	}
	destinationBlock.setSize(totalSize, false);
	
	DBG("Comp: "<<(int)sourceBlockSize<<" - Original: "<<(int)destinationBlock.getSize()<<" - Ratio: "<<(destinationBlock.getSize()/(float)sourceBlockSize)<<"\n");
}


void serialiseAudio(MemoryBlock &serialisedBlock,
					const int numChannels, const int numSamples, const float** channelData)
{
	serialisedBlock.append(&numChannels, sizeof(int));
	serialisedBlock.append(&numSamples, sizeof(int));
	if (numChannels > 0)
		serialisedBlock.append(channelData[0], numSamples * sizeof(float));
	if (numChannels > 1)
		serialisedBlock.append(channelData[1], numSamples * sizeof(float));	
}

void deserialiseAudio(MemoryBlock &serialisedBlock,
					  int &numChannels, int &numSamples, float** channelData)
{
	numChannels = *static_cast<int*>(serialisedBlock.getData());
	numSamples = *static_cast<int*>(addBytesToPointer(serialisedBlock.getData(),sizeof(int)));
	
	if (numChannels > 0)
	{
		const int bytesPerChannel = (serialisedBlock.getSize()-(2*sizeof(int))) / numChannels;
		
		if (numChannels > 0)
			memcpy(channelData[0], (float*)addBytesToPointer(serialisedBlock.getData(),2*sizeof(int)), bytesPerChannel);
		if (numChannels > 1)
			memcpy(channelData[1], (float*)addBytesToPointer(serialisedBlock.getData(),2*sizeof(int)+bytesPerChannel), bytesPerChannel);
	}
}

//==============================================================================
InterprocessCommsDemo::InterprocessCommsDemo()
:	isConnected(false),
	isSender(true),
	compressAudio(0),
	settingsTree(Settings::getInstance()->getValueTree()),
	audioBufferL(220500),
	audioBufferR(220500)
{
	server = new DemoInterprocessConnectionServer (*this);
 	
	initialiseAudio();
	audioBlock = new AudioData();
	
	setName (T("Interprocess Communication"));
	
	// create all our UI bits and pieces..
	addAndMakeVisible (modeSelector = new ComboBox (T("mode:")));
	(new Label (modeSelector->getName(), modeSelector->getName()))->attachToComponent (modeSelector, true);
	
	modeSelector->addItem (T("(Disconnected)"), 1);
	modeSelector->addSeparator();
	modeSelector->addItem (T("Socket (listening)"), 2);
	modeSelector->addItem (T("Socket (connect to existing socket)"), 3);
	
	modeSelector->getSelectedIdAsValue().referTo(settingsTree.getPropertyAsValue(SettingsNames[Settings::mode], nullptr));
	modeSelector->addListener (this);
		
	addAndMakeVisible (socketNumber = new TextEditor (T("socket port:")));
	socketNumber->setMultiLine (false);
	socketNumber->setInputRestrictions (5, T("0123456789"));
	(new Label (socketNumber->getName(), socketNumber->getName()))->attachToComponent (socketNumber, true);
	socketNumber->getTextValue().referTo(settingsTree.getPropertyAsValue(SettingsNames[Settings::port], nullptr));
	
	addAndMakeVisible (socketHost = new TextEditor (T("socket host:")));
	socketHost->setMultiLine (false);
	socketNumber->setInputRestrictions (512);
	(new Label (socketHost->getName(), socketHost->getName()))->attachToComponent (socketHost, true);
	socketHost->getTextValue().referTo(settingsTree.getPropertyAsValue(SettingsNames[Settings::host], nullptr));

	addAndMakeVisible(&compressAudioButton);
	compressAudioButton.setButtonText("Compress");
	compressAudioButton.addListener(this);
	
	addAndMakeVisible(incomingMessages = new TextEditor (T("messages")));
	incomingMessages->setReadOnly (true);
	incomingMessages->setMultiLine (true);
	
	addAndMakeVisible(&audioSettingsButton);
	audioSettingsButton.setButtonText("Audio Settings");
	audioSettingsButton.addListener(this);

	triggerAsyncUpdate();
}

InterprocessCommsDemo::~InterprocessCommsDemo()
{
	close();
	
	ScopedPointer<XmlElement> audioXml(audioManager.createStateXml());
	if (&*audioXml != nullptr) {
		settingsTree.setProperty(SettingsNames[Settings::audioSettings], audioXml->createDocument(""), nullptr);
	}
	
	///removeChildComponent(&scope);
	removeChildComponent(&audioSettingsButton);
	removeChildComponent(&compressAudioButton);
	deleteAllChildren();
}

void InterprocessCommsDemo::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	modeSelector->setBounds (60, 25, 200, 24);
	socketNumber->setBounds (350, 25, 80, 24);
	socketHost->setBounds (530, 25, 130, 24);
		
	audioSettingsButton.setBounds(socketHost->getRight()+m, socketHost->getY(), 100, socketHost->getHeight());
	
	compressAudioButton.setBounds(m, modeSelector->getBottom()+m, 100, 20);
	
	incomingMessages->setBounds (30, compressAudioButton.getBottom()+m, w-2*m, 100);
}

void InterprocessCommsDemo::handleAsyncUpdate()
{
	// call this to set up everything's state correctly.
	comboBoxChanged (modeSelector);
}

void InterprocessCommsDemo::buttonClicked (Button* button)
{
	if (button == &audioSettingsButton)
	{
		AudioDeviceSelectorComponent audioSettingsComp (audioManager,
														1, 2,
														1, 2,
														false,
														false,
														false,
														false);
		
		// ...and show it in a DialogWindow...
		audioSettingsComp.setSize (500, 400);
		DialogWindow::showModalDialog ("Audio Settings",
									   &audioSettingsComp,
									   this,
									   Colours::azure,
									   true);
	}
	else if(button == &compressAudioButton)
	{
		compressAudio = compressAudioButton.getToggleState();
	}
}

void InterprocessCommsDemo::comboBoxChanged (ComboBox* changedBox)
{
	if (changedBox == modeSelector)
	{
		// This is called when the user picks a different mode from the drop-down list..
		const int modeId = modeSelector->getSelectedId();
		
		close();
		
		if (modeId > 1 && modeId < 4)
		{
			open ((modeId == 3) ? true : false);
		}
	}
}

//==============================================================================
// Just closes any connections that are currently open.
void InterprocessCommsDemo::close()
{
	server->stop();
	activeConnections.clear();
	
	appendMessage ("Select a mode from the dropdown to begin.");
}

void InterprocessCommsDemo::open (bool asSender)
{
	isSender = asSender;
	incomingMessages->setText (String::empty, false);
	close();
	
	// and try to open the socket or pipe...
	bool openedOk = false;
	
	if (asSender)
	{
		socketHost->setVisible(true);
		
		// if we're connecting to an existing server, we can just create a connection object
		// directly.
		DemoInterprocessConnection* newConnection = new DemoInterprocessConnection (*this);
		
		openedOk = newConnection->connectToSocket (socketHost->getText(),
												   socketNumber->getText().getIntValue(),
												   1000);
		
		if (openedOk)
			activeConnections.add (newConnection);
		else
			delete newConnection;
	}
	else
	{
		socketHost->setVisible(false);

		// if we're starting up a server, we need to tell the server to start waiting for
		// clients to connect. It'll then create connection objects for us when clients arrive.
		openedOk = server->beginWaitingForSocket (socketNumber->getText().getIntValue());
		
		if (openedOk)
			appendMessage (T("Waiting for another app to connect to this socket.."));
	}
	
	if (! openedOk)
	{
		modeSelector->setSelectedId (1);
		
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 T("Interprocess Comms Demo"),
									 T("Failed to open the socket or pipe..."));
	}
//	else {
//		audioManager.addAudioCallback(this);
//	}
	
	repaint();
}

void InterprocessCommsDemo::appendMessage (const String& message)
{
	incomingMessages->setCaretPosition (INT_MAX);
	incomingMessages->insertTextAtCaret (message + T("\n"));
	incomingMessages->setCaretPosition (INT_MAX);
}
	
//==============================================================================
void InterprocessCommsDemo::initialiseAudio()
{
	ScopedPointer<XmlElement> savedSettings(XmlDocument::parse(settingsTree[SettingsNames[Settings::audioSettings]]));
	const String error (audioManager.initialise (2, /* number of input channels */
												 2, /* number of output channels */
												 savedSettings, /* no XML settings.. */
												 true  /* select default device on failure */));
	
	if (error.isNotEmpty())
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 T("Audio Demo"),
									 T("Couldn't open an output device!\n\n") + error);
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
			audioData.append(&compressAudio, sizeof(int));
			
			if (compressAudio)
			{
				MemoryBlock rawData;
				rawData.append(&numInputChannels, sizeof(int));
				rawData.append(&numSamples, sizeof(int));
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
				audioData.append(&numInputChannels, sizeof(int));
				audioData.append(&numSamples, sizeof(int));
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
			if(audioBufferL.getNumAvailable() > 0)
				audioBufferL.readSamples(outputChannelData[0], numSamples);

			if (numOutputChannels > 1 && audioBufferR.getNumAvailable() > 0)
				audioBufferR.readSamples(outputChannelData[1], numSamples);
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
InterprocessCommsDemo::DemoInterprocessConnection::DemoInterprocessConnection (InterprocessCommsDemo& owner_)
:	InterprocessConnection (true),
	owner (owner_)
{
	static int totalConnections = 0;
	ourNumber = ++totalConnections;
}

InterprocessCommsDemo::DemoInterprocessConnection::~DemoInterprocessConnection()
{
}

void InterprocessCommsDemo::DemoInterprocessConnection::connectionMade()
{
	owner.audioManager.addAudioCallback(&owner);
	owner.isConnected = true;
	owner.appendMessage (T("Connection #") + String (ourNumber) + T(" - connection started"));
	
	if (owner.isSender) {
		owner.appendMessage ("Sending Audio");
	}
	else {
		owner.appendMessage ("Recieving Audio");
	}
}

void InterprocessCommsDemo::DemoInterprocessConnection::connectionLost()
{
	owner.audioManager.removeAudioCallback(&owner);
	owner.isConnected = false;
	owner.appendMessage (T("Connection #") + String (ourNumber) + T(" - connection lost"));
}

void InterprocessCommsDemo::DemoInterprocessConnection::messageReceived (const MemoryBlock& message)
{
	const int isDataCompressed = *static_cast<int*>(message.getData());
	void* startOfBlock = addBytesToPointer(message.getData(), sizeof(int));
	size_t sizeOfBlock = message.getSize()-sizeof(int);

	if (isDataCompressed)
	{
		//uncompress here
		MemoryBlock decompressedBlock;
		decompressMemory(startOfBlock, sizeOfBlock, decompressedBlock);
		
		startOfBlock = decompressedBlock.getData();//uncompressed data
		sizeOfBlock = decompressedBlock.getSize();
	}

	const int numChannels = *static_cast<int*>(startOfBlock);
	const int numSamples = *static_cast<int*>(addBytesToPointer(startOfBlock, sizeof(int)));
	if (numChannels > 0)
	{
		const int bytesPerChannel = (sizeOfBlock-(2*sizeof(int)))/numChannels;

		owner.audioBufferL.writeSamples((float*)addBytesToPointer(startOfBlock, 2*sizeof(int)), numSamples);

		if (numChannels > 1)
			owner.audioBufferR.writeSamples((float*)addBytesToPointer(startOfBlock, 2*sizeof(int)+bytesPerChannel), numSamples);
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