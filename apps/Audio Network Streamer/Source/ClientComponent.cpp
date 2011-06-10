/*
  ==============================================================================

    ClientComponent.cpp
    Created: 20 May 2011 6:03:07am
    Author:  David Rowland

  ==============================================================================
*/

#include "ClientComponent.h"

ClientComponent::ClientComponent()
{
	server = new AudioInterprocessConnectionServer (*this);
	bool openedOk = server->beginWaitingForSocket(12345);
	if (openedOk) {
		DBG("client started ok");
	}
	connection = server->createConnectionObject();

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

ClientComponent::~ClientComponent()
{
	server->stop();
	activeConnections.clear();

	audioManager.removeAudioCallback(this);
	audioSelector = nullptr;
}

void ClientComponent::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	audioSelector->setBounds(0, 0, w, 200);
	scope.setBounds(0, audioSelector->getBottom()+m, w, h-audioSelector->getHeight()-m);
}

//==============================================================================
void ClientComponent::audioDeviceIOCallback (const float** inputChannelData,
											 int numInputChannels,
											 float** outputChannelData,
											 int numOutputChannels,
											 int numSamples)
{
	scope.processBlock(inputChannelData[0], numSamples);
}

void ClientComponent::audioDeviceAboutToStart (AudioIODevice* device)
{
	DBG("client audio device started");
}

void ClientComponent::audioDeviceStopped()
{
	DBG("client audio device stopped");
}

//==============================================================================