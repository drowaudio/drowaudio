/*
  ==============================================================================

    NetworkDemo.cpp
    Created: 19 May 2011 1:17:21am
    Author:  David Rowland

  ==============================================================================
*/

#include "NetworkDemo.h"

NetworkDemo::NetworkDemo()
//:	localBrowser(FileBrowserComponent::openMode
//				 + FileBrowserComponent::canSelectFiles
//				 + FileBrowserComponent::canSelectDirectories,
//				 File::getSpecialLocation(File::currentExecutableFile),
//				 nullptr)
{
	addAndMakeVisible(&connectionComponent);
	connectionComponent.addListener(this);
	connectionComponent.setCURLSessionToControl(&remoteBrowser.getCURLSession());
	
	addAndMakeVisible(&localBrowser);
	addAndMakeVisible(&remoteBrowser);
	
	localBrowser.setComponentID("local");
	remoteBrowser.setComponentID("remote");
	
	addAndMakeVisible(&pwdButton);
    pwdButton.setButtonText ("PWD");
	pwdButton.addListener(this);
}

NetworkDemo::~NetworkDemo()
{
	connectionComponent.removeListener(this);
	pwdButton.removeListener(this);
}

void NetworkDemo::resized()
{
	const int w = getWidth();
	const int h = getHeight();

	connectionComponent.setBounds(0, 0, w, 100);
	pwdButton.setBounds(0, connectionComponent.getBottom()+5, 100, 20);
	localBrowser.setBounds(0, pwdButton.getBottom()+5, w/2-2, h-pwdButton.getBottom()-5);
	remoteBrowser.setBounds(localBrowser.getRight()+2, pwdButton.getBottom()+5, w/2-2, h-pwdButton.getBottom()-5);
//	localBrowser.setBounds("0, 0, parent.width/2-2, parent.height");
//	remoteBrowser.setBounds("(remote.right)+2, 0, (parent.width/2)-2, parent.height");
}

void NetworkDemo::connectionChanged (ConnectionComponent* changedConnectionComponent)
{
	if (changedConnectionComponent == &connectionComponent)
	{
		DBG("connection changed");
		remoteBrowser.refresh();
	}
}

void NetworkDemo::buttonClicked(Button *button)
{
	if (button == &pwdButton)
    {
		DBG(remoteBrowser.getLastUrl());
	}
}