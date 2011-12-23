/*
  ==============================================================================

    NetworkDemo.h
    Created: 19 May 2011 1:17:21am
    Author:  David Rowland

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
	TextButton pwdButton;
    
    MemoryBlock block;
    ScopedPointer<MemoryInputStream> inputStream;
    ScopedPointer<CURLEasySession> curlSession;
};

#endif  // __NetworkDemo_H_577067C6__
