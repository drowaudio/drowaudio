/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
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
}

NetworkDemo::~NetworkDemo()
{
	connectionComponent.removeListener(this);
}

void NetworkDemo::resized()
{
	const int w = getWidth();
	const int h = getHeight();

	connectionComponent.setBounds(0, 0, w, 100);
	localBrowser.setBounds(0, connectionComponent.getBottom()+5, w/2-2, h-connectionComponent.getBottom()-5);
	remoteBrowser.setBounds(localBrowser.getRight()+2, connectionComponent.getBottom()+5, w/2-2, h-connectionComponent.getBottom()-5);
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
}