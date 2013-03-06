/*
  ==============================================================================

  This file is part of the dRowAudio JUCE module
  Copyright 2004-13 by dRowAudio.

  ------------------------------------------------------------------------------

  dRowAudio is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
  SOFTWARE.

  ==============================================================================
*/

#include "NetworkDemo.h"

#if DROWAUDIO_USE_CURL

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

void NetworkDemo::buttonClicked(Button* /*button*/)
{
}

#endif