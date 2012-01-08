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

#ifndef __CONNECTIONCOMPONENT_H_48116F81__
#define __CONNECTIONCOMPONENT_H_48116F81__

#include "../../JuceLibraryCode/JuceHeader.h"

#if DROWAUDIO_USE_CURL

class ConnectionComponent : public Component,
							public Button::Listener
{
public:
	
	ConnectionComponent();
	
	~ConnectionComponent();

	void setCURLSessionToControl (CURLEasySession* sessionToControl);
	
	void resized();
	
	void buttonClicked (Button* button);
	
    //==============================================================================
    /**
	 Used to receive callbacks when a connection is changed.
	 */
    class Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener()                                     {}
		
        /** Called when the button is clicked. */
        virtual void connectionChanged (ConnectionComponent* /*connectionComponent*/)	{}
    };
	
    void addListener (Listener* newListener);
	
    void removeListener (Listener* listener);
	
    //==============================================================================
	
private:
	
	CURLEasySession* curlSession;
	ListenerList <Listener> listeners;

	Label urlLabel, hostnameLabel, usernameLabel, passwordLabel, protocolLabel;
	TextEditor urlEditor, hostnameEditor, usernameEditor, passwordEditor;
	ComboBox protocolBox;
	TextButton connectButton;
};

#endif
#endif  // __CONNECTIONCOMPONENT_H_48116F81__
