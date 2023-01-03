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

#ifndef LOCAL_DIRECTORY_LIST_BOX_H
#define LOCAL_DIRECTORY_LIST_BOX_H

#include "../DemoHeader.h"

#if DROWAUDIO_USE_CURL

class LocalDirectoryListBoxModel : public juce::ListBoxModel,
                                   public juce::ChangeBroadcaster
{
public:
    LocalDirectoryListBoxModel();

    //==============================================================================
    const juce::File& getCurrentWorkingDirectory() const { return currentWorkingDirectory; }

    void refresh();
    void setContents (const juce::StringArray& newContents);

    //==============================================================================
    int getNumRows() override;
    void paintListBoxItem (int rowNumber,
                           juce::Graphics& g,
                           int width, int height,
                           bool rowIsSelected) override;
    void listBoxItemDoubleClicked (int row, const juce::MouseEvent& e) override;
    juce::var getDragSourceDescription (const juce::SparseSet<int>& currentlySelectedRows) override;

private:
    //==============================================================================
    juce::StringArray itemList;
    juce::File currentWorkingDirectory;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LocalDirectoryListBoxModel)
};

//==============================================================================
class LocalDirectoryListBox : public juce::ListBox,
                              public juce::ChangeListener,
                              public juce::DragAndDropTarget
{
public:
    LocalDirectoryListBox();

    ~LocalDirectoryListBox();

    //==============================================================================
    void paintOverChildren (juce::Graphics& g) override;
    void changeListenerCallback (juce::ChangeBroadcaster* source) override;
    bool isInterestedInDragSource (const SourceDetails& dragSourceDetails) override;
    void itemDragEnter (const SourceDetails& dragSourceDetails) override;
    void itemDragExit (const SourceDetails& dragSourceDetails) override;
    void itemDropped (const SourceDetails& dragSourceDetails) override;

private:
    //==============================================================================
    LocalDirectoryListBoxModel model;
    bool isInterestedInDrag;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LocalDirectoryListBox)
};

#endif //DROWAUDIO_USE_CURL
#endif //LOCAL_DIRECTORY_LIST_BOX_H
