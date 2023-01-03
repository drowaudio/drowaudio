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

#ifndef NETWORK_DEMO_H
#define NETWORK_DEMO_H

#include "ConnectionComponent.h"
#include "LocalDirectoryListBox.h"
#include "RemoteDirectoryListBox.h"

#if DROWAUDIO_USE_CURL

/** @todo
    Refresh directory on right click
    Better look of file list
    Thread CURLEasySession
*/
class NetworkDemo : public juce::Component,
                    public juce::DragAndDropContainer,
                    public ConnectionComponent::Listener
{
public:
    NetworkDemo();

    //==============================================================================
    /** @internal */
    void resized();
    /** @internal */
    void connectionChanged (ConnectionComponent* connectionComponent);

private:
    //==============================================================================
    ConnectionComponent connectionComponent;
    LocalDirectoryListBox localBrowser;
    RemoteDirectoryListBox remoteBrowser;

    // unused:
    std::unique_ptr<juce::MemoryInputStream> inputStream;
    std::unique_ptr<CURLEasySession> curlSession;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NetworkDemo)
};

#endif //DROWAUDIO_USE_CURL
#endif //NETWORK_DEMO_H
