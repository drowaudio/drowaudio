/*
  ==============================================================================

    ServerComponent.cpp
    Created: 20 May 2011 6:02:55am
    Author:  David Rowland

  ==============================================================================
*/

#include "ServerComponent.h"

ServerComponent::ServerComponent()
{
	server = new AudioInterprocessConnectionServer (*this);
	connection = server->createConnectionObject();
	bool openedOk = connection->connectToSocket ("localhost",
												 12345,
												 10000);
	if (openedOk) {
		DBG("server opened ok");
	}
	
	addAndMakeVisible(&hostEditor);
	(new Label("hostLabel", "Host:"))->attachToComponent(&hostEditor, true);
	addAndMakeVisible(&portEditor);
	(new Label("portLabel", "Port:"))->attachToComponent(&portEditor, true);
	addAndMakeVisible(&connectButton);
	connectButton.setButtonText("Connect");
	
	const String error (audioManager.initialise (1, /* number of input channels */
												 2, /* number of output channels */
												 0, /* no XML settings.. */
												 true  /* select default device on failure */));
	
	if (error.isNotEmpty())
	{
		AlertWindow::showMessageBox (AlertWindow::WarningIcon,
									 T("Audio Demo"),
									 T("Couldn't open an output device!\n\n") + error);
	}
	else
	{
		audioManager.addAudioCallback(this);
	}

	audioSelector = new AudioDeviceSelectorComponent(audioManager,
													 1,
													 2,
													 2,
													 2,
													 true,
													 true,
													 false,
													 false);
	addAndMakeVisible(audioSelector);
	addAndMakeVisible(&scope);
}

ServerComponent::~ServerComponent()
{
	server->stop();
	activeConnections.clear();

	audioManager.removeAudioCallback(this);
	audioSelector = nullptr;
}

void ServerComponent::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	hostEditor.setBounds(w/6, m, w/6, 20);
	portEditor.setBounds((w/6)*3, m, w/6, 20);
	connectButton.setBounds((w/6)*4+m, m, w/3-2*m, 20);
	
	audioSelector->setBounds(0, hostEditor.getBottom()+m, w, 200);
	scope.setBounds(0, audioSelector->getBottom()+m, w, h-audioSelector->getHeight()-m);
}

//==============================================================================
void ServerComponent::audioDeviceIOCallback (const float** inputChannelData,
											 int numInputChannels,
											 float** outputChannelData,
											 int numOutputChannels,
											 int numSamples)
{
	scope.processBlock(inputChannelData[0], numSamples);
	
	MemoryBlock audioData (inputChannelData[0], numSamples * sizeof(float));
	if(! connection->sendMessage (audioData)) {
		//DBG("write failed");
	}
//	for (int i = activeConnections.size(); --i >= 0;)
//	{
//		if (! activeConnections[i]->sendMessage (messageData))
//		{
//			// the write failed, so indicate that the connection has broken..
//			DBG ("write failed");
//		}
//	}
	
}

void ServerComponent::audioDeviceAboutToStart (AudioIODevice* device)
{
	DBG("server audio device started");
}

void ServerComponent::audioDeviceStopped()
{
	DBG("server audio device stopped");
}

//==============================================================================
