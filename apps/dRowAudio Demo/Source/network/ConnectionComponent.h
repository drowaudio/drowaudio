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

#ifndef __CONNECTIONCOMPONENT_H_48116F81__
#define __CONNECTIONCOMPONENT_H_48116F81__

#include "../DemoHeader.h"

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
