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

#ifndef __NetworkDemo_H_577067C6__
#define __NetworkDemo_H_577067C6__

#include "../DemoHeader.h"
#include "ConnectionComponent.h"
#include "LocalDirectoryListBox.h"
#include "RemoteDirectoryListBox.h"

#if DROWAUDIO_USE_CURL

/**	@todo
	Refresh directory on right click
	Better look of file list
	Thread CURLEasySession
 */
class NetworkDemo : public Component,
                    public DragAndDropContainer,
                    public ConnectionComponent::Listener,
                    public Button::Listener
{
public:
	NetworkDemo();
	
	~NetworkDemo();
	
	void resized();
	
	void connectionChanged (ConnectionComponent* connectionComponent);

	void buttonClicked (Button* button);
	
	//==============================================================================

private:
	
//	FileBrowserComponent localBrowser;
	ConnectionComponent connectionComponent;
	LocalDirectoryListBox localBrowser;
	RemoteDirectoryListBox remoteBrowser;
    
    MemoryBlock block;
    ScopedPointer<MemoryInputStream> inputStream;
    ScopedPointer<CURLEasySession> curlSession;
};

#endif
#endif  // __NetworkDemo_H_577067C6__
