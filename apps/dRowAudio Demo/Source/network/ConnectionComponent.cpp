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

#include "ConnectionComponent.h"

ConnectionComponent::ConnectionComponent()
:	curlSession (nullptr)
{
	addAndMakeVisible(&urlLabel);
	addAndMakeVisible(&hostnameLabel);
	addAndMakeVisible(&usernameLabel);
	addAndMakeVisible(&passwordLabel);
	addAndMakeVisible(&protocolLabel);
	
	urlLabel.setComponentID("urlLabel");
	hostnameLabel.setComponentID("hostnameLabel");
	usernameLabel.setComponentID("usernameLabel");
	passwordLabel.setComponentID("passwordLabel");
	protocolLabel.setComponentID("protocolLabel");
	
	urlLabel.setText("URL", false);
	hostnameLabel.setText("Hostname", false);
	usernameLabel.setText("Username", false);
	passwordLabel.setText("Password", false);
	protocolLabel.setText("Protocol", false);
	
	addAndMakeVisible(&urlEditor);
	addAndMakeVisible(&hostnameEditor);
	addAndMakeVisible(&usernameEditor);
	addAndMakeVisible(&passwordEditor);

	urlEditor.setComponentID("urlEditor");
	hostnameEditor.setComponentID("hostnameEditor");
	usernameEditor.setComponentID("usernameEditor");
	passwordEditor.setComponentID("passwordEditor");
	
	passwordEditor.setPasswordCharacter((juce_wchar) 0x2022);
	
	addAndMakeVisible(&protocolBox);
    protocolBox.addItemList (CURLManager::getInstance()->getSupportedProtocols(), 1);
    for (int i = 0; i < protocolBox.getNumItems(); ++i)
        if (protocolBox.getItemText (i) == "ftp")
            protocolBox.setSelectedItemIndex (i);
    
	addAndMakeVisible(&connectButton);
	connectButton.setButtonText("Connect");
	connectButton.addListener(this);
}

ConnectionComponent::~ConnectionComponent()
{
	connectButton.removeListener(this);
}

void ConnectionComponent::setCURLSessionToControl(CURLEasySession *sessionToControl)
{
	curlSession = sessionToControl;
}

void ConnectionComponent::resized()
{
	const int w = getWidth();
	const int m = 5;
	const int buttonW = 60;
	
//	urlLabel.setBounds(0, m, 100, 20);
//	hostnameLabel.setBounds(urlLabel.getX(), urlLabel.getBottom() + m, urlLabel.getWidth(), urlLabel.getHeight());
	hostnameLabel.setBounds(0, m, 100, 20);
	usernameLabel.setBounds(hostnameLabel.getX(), hostnameLabel.getBottom() + m, hostnameLabel.getWidth(), hostnameLabel.getHeight());
	passwordLabel.setBounds(usernameLabel.getX(), usernameLabel.getBottom() + m, usernameLabel.getWidth(), usernameLabel.getHeight());
	protocolLabel.setBounds(passwordLabel.getX(), passwordLabel.getBottom() + m, passwordLabel.getWidth(), passwordLabel.getHeight());
	
//	urlEditor.setBounds(urlLabel.getRight() + m, urlLabel.getY(), w - urlLabel.getRight() - 2*m, urlLabel.getHeight());
	hostnameEditor.setBounds(hostnameLabel.getRight() + m, hostnameLabel.getY(), w - hostnameLabel.getRight() - 2*m, hostnameLabel.getHeight());
	usernameEditor.setBounds(usernameLabel.getRight() + m, usernameLabel.getY(), w - usernameLabel.getRight() - 2*m, usernameLabel.getHeight());
	passwordEditor.setBounds(passwordLabel.getRight() + m, passwordLabel.getY(), w - passwordLabel.getRight() - 2*m, passwordLabel.getHeight());
	protocolBox.setBounds(protocolLabel.getRight() + m, protocolLabel.getY(), w - protocolLabel.getRight() - buttonW - 3*m, protocolLabel.getHeight());
	
	connectButton.setBounds(protocolBox.getRight() + m, protocolBox.getY(), buttonW, protocolBox.getHeight());
	
//	urlLabel.setBounds("0, 0, 100, 20");
//	hostnameLabel.setBounds("urlLabel.left, urlLabel.bottom + 5, urlLabel.width, urlLabel.height");
//	usernameLabel.setBounds("hostnameLabel.left, hostnameLabel.bottom + 5, hostnameLabel.width, hostnameLabel.height");
//	passwordLabel.setBounds("usernameLabel.left, usernameLabel.bottom + 5, usernameLabel.width, usernameLabel.height");
	
//	urlEditor.setBounds("urlLabel.right + 5, urlLabel.top, parent.width - urlLabel.right - 10, urlLabel.height");
//	hostnameEditor.setBounds("hostnameLabel.right + 5, hostnameLabel.top, parent.width - hostnameLabel.right - 10, hostnameLabel.height");
//	usernameEditor.setBounds("usernameLabel.right + 5, usernameLabel.top, parent.width - usernameLabel.right - 10, usernameLabel.height");
//	passwordEditor.setBounds("passwordEditor.right + 5, passwordEditor.top, parent.width - passwordEditor.right - 10, passwordEditor.height");
}

void ConnectionComponent::buttonClicked (Button* button)
{
	if (button == &connectButton)
	{
		if (curlSession != nullptr)
		{
			String path(protocolBox.getItemText(protocolBox.getSelectedItemIndex())+"://"+hostnameEditor.getText());
			DBG(path);
			curlSession->setRemotePath (path);
			curlSession->setUserNameAndPassword(usernameEditor.getText(), passwordEditor.getText());
            DBG("Type: "<<curlSession->getContentType());
			listeners.call (&Listener::connectionChanged, this);
		}
	}
}

//==============================================================================
void ConnectionComponent::addListener (ConnectionComponent::Listener* const newListener)
{
    listeners.add (newListener);
}

void ConnectionComponent::removeListener (ConnectionComponent::Listener* const listener)
{
    listeners.remove (listener);
}
