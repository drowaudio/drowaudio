/*
  ==============================================================================

    ConnectionComponent.h
    Created: 19 May 2011 5:40:17am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __CONNECTIONCOMPONENT_H_48116F81__
#define __CONNECTIONCOMPONENT_H_48116F81__

#include "../../JuceLibraryCode/JuceHeader.h"

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
        virtual void connectionChanged (ConnectionComponent* connectionComponent)	{}
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

#endif  // __CONNECTIONCOMPONENT_H_48116F81__
