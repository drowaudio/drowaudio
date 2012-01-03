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

#ifndef __NetworkDemo_H_577067C6__
#define __NetworkDemo_H_577067C6__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "ConnectionComponent.h"
#include "LocalDirectoryListBox.h"
#include "RemoteDirectoryListBox.h"

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

#endif  // __NetworkDemo_H_577067C6__
