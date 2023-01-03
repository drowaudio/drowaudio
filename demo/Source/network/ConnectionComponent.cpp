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

#include "ConnectionComponent.h"

#if DROWAUDIO_USE_CURL

ConnectionComponent::ConnectionComponent() :
    curlSession (nullptr)
{
    urlLabel.setComponentID ("urlLabel");
    hostnameLabel.setComponentID ("hostnameLabel");
    usernameLabel.setComponentID ("usernameLabel");
    passwordLabel.setComponentID ("passwordLabel");
    protocolLabel.setComponentID ("protocolLabel");

    urlLabel.setText ("URL", dontSendNotification);
    hostnameLabel.setText ("Hostname", dontSendNotification);
    usernameLabel.setText ("Username", dontSendNotification);
    passwordLabel.setText ("Password", dontSendNotification);
    protocolLabel.setText ("Protocol", dontSendNotification);

    urlEditor.setComponentID ("urlEditor");
    hostnameEditor.setComponentID ("hostnameEditor");
    usernameEditor.setComponentID ("usernameEditor");
    passwordEditor.setComponentID ("passwordEditor");

    passwordEditor.setPasswordCharacter ((juce_wchar) 0x2022);

    protocolBox.addItemList (CURLManager::getInstance()->getSupportedProtocols(), 1);
    for (int i = 0; i < protocolBox.getNumItems(); ++i)
        if (protocolBox.getItemText (i) == "ftp")
            protocolBox.setSelectedItemIndex (i);

    connectButton.setButtonText ("Connect");
    connectButton.addListener (this);

    addAndMakeVisible (&urlLabel);
    addAndMakeVisible (&hostnameLabel);
    addAndMakeVisible (&usernameLabel);
    addAndMakeVisible (&passwordLabel);
    addAndMakeVisible (&protocolLabel);
    addAndMakeVisible (&urlEditor);
    addAndMakeVisible (&hostnameEditor);
    addAndMakeVisible (&usernameEditor);
    addAndMakeVisible (&passwordEditor);
    addAndMakeVisible (&protocolBox);
    addAndMakeVisible (&connectButton);
}

void ConnectionComponent::setCURLSessionToControl (CURLEasySession* sessionToControl)
{
    curlSession = sessionToControl;
}

void ConnectionComponent::resized()
{
    const int w = getWidth();
    const int m = 5;
    const int buttonW = 60;

    hostnameLabel.setBounds (0, m, 100, 20);
    usernameLabel.setBounds (hostnameLabel.getX(), hostnameLabel.getBottom() + m, hostnameLabel.getWidth(), hostnameLabel.getHeight());
    passwordLabel.setBounds (usernameLabel.getX(), usernameLabel.getBottom() + m, usernameLabel.getWidth(), usernameLabel.getHeight());
    protocolLabel.setBounds (passwordLabel.getX(), passwordLabel.getBottom() + m, passwordLabel.getWidth(), passwordLabel.getHeight());

    hostnameEditor.setBounds (hostnameLabel.getRight() + m, hostnameLabel.getY(), w - hostnameLabel.getRight() - 2 * m, hostnameLabel.getHeight());
    usernameEditor.setBounds (usernameLabel.getRight() + m, usernameLabel.getY(), w - usernameLabel.getRight() - 2 * m, usernameLabel.getHeight());
    passwordEditor.setBounds (passwordLabel.getRight() + m, passwordLabel.getY(), w - passwordLabel.getRight() - 2 * m, passwordLabel.getHeight());
    protocolBox.setBounds (protocolLabel.getRight() + m, protocolLabel.getY(), w - protocolLabel.getRight() - buttonW - 3 * m, protocolLabel.getHeight());

    connectButton.setBounds (protocolBox.getRight() + m, protocolBox.getY(), buttonW, protocolBox.getHeight());
}

void ConnectionComponent::buttonClicked (Button* button)
{
    if (button == &connectButton)
    {
        if (curlSession != nullptr)
        {
            const juce::String path (protocolBox.getItemText (protocolBox.getSelectedItemIndex()) + "://" + hostnameEditor.getText());
            DBG (path);
            curlSession->setRemotePath (path);
            curlSession->setUserNameAndPassword (usernameEditor.getText(), passwordEditor.getText());

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

#endif //DROWAUDIO_USE_CURL
