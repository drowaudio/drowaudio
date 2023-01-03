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

#ifndef REMOTE_DIRECTORY_LIST_BOX_H
#define REMOTE_DIRECTORY_LIST_BOX_H

#include "../DemoHeader.h"

#if DROWAUDIO_USE_CURL

class RemoteDirectoryListBoxModel : public juce::ListBoxModel,
public juce::ChangeBroadcaster
{
public:
    RemoteDirectoryListBoxModel();

    //==============================================================================
    void setCURLSession (CURLEasySession* sessionToControl);
    void setContents (const juce::StringArray& newContents);

    void refresh();

    //==============================================================================
    /** @internal */
    int getNumRows() override;
    /** @internal */
    void paintListBoxItem (int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    /** @internal */
    void listBoxItemDoubleClicked (int row, const juce::MouseEvent& e) override;
    /** @internal */
    juce::var getDragSourceDescription (const juce::SparseSet<int>& currentlySelectedRows) override;

private:
    //==============================================================================
    juce::StringArray itemList;
    CURLEasySession* curlSession;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RemoteDirectoryListBoxModel)
};

//==============================================================================
class RemoteDirectoryListBox : public juce::ListBox,
                               public juce::ChangeListener,
                               public juce::DragAndDropTarget
{
public:
    RemoteDirectoryListBox();

    ~RemoteDirectoryListBox();

    //==============================================================================
    CURLEasySession& getCURLSession() { return session; }

    juce::String getLastUrl() const { return session.getCurrentWorkingDirectory(); }

    void refresh();

    //==============================================================================
    /** @internal */
    void paintOverChildren (juce::Graphics& g) override;
    /** @internal */
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    /** @internal */
    bool isInterestedInDragSource (const SourceDetails& dragSourceDetails) override;
    /** @internal */
    void itemDragEnter (const SourceDetails& dragSourceDetails) override;
    /** @internal */
    void itemDragExit (const SourceDetails& dragSourceDetails) override;
    /** @internal */
    void itemDropped (const SourceDetails& dragSourceDetails) override;

private:
    //==============================================================================
    RemoteDirectoryListBoxModel model;
    CURLEasySession session;
    bool isInterestedInDrag;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RemoteDirectoryListBox)
};

#endif //DROWAUDIO_USE_CURL
#endif //REMOTE_DIRECTORY_LIST_BOX_H
