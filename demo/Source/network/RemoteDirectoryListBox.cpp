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

#include "RemoteDirectoryListBox.h"
#include "LocalDirectoryListBox.h"

#if DROWAUDIO_USE_CURL

RemoteDirectoryListBoxModel::RemoteDirectoryListBoxModel() :
    curlSession (nullptr)
{
}

void RemoteDirectoryListBoxModel::setCURLSession (CURLEasySession* sessionToControl)
{
    curlSession = sessionToControl;
    setContents (curlSession->getDirectoryListing());
    sendChangeMessage();
}

void RemoteDirectoryListBoxModel::refresh()
{
    setContents (curlSession->getDirectoryListing());
}

int RemoteDirectoryListBoxModel::getNumRows()
{
    return itemList.size();
}

void RemoteDirectoryListBoxModel::paintListBoxItem (int rowNumber, juce::Graphics& g,
                                                    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.setColour (juce::Colours::lightblue);
        g.fillAll();
    }

    g.setColour (juce::Colours::black);
    g.drawFittedText (itemList[rowNumber],
                      0, 0, width, height,
                      juce::Justification::centredLeft, 1);
}

void RemoteDirectoryListBoxModel::setContents (const juce::StringArray& newContents)
{
    itemList = newContents;
}

void RemoteDirectoryListBoxModel::listBoxItemDoubleClicked (int row, const juce::MouseEvent& /*e*/)
{
    //*** Need to navigate session here
    if (itemList[row] == "..")
    {
        curlSession->setRemotePath (curlSession->getRemotePath().upToLastOccurrenceOf ("/", false, false));
        setContents (curlSession->getDirectoryListing());
    }
    else
    {
        if (! itemList[row].contains ("."))
        {
            juce::String newCWD (curlSession->getRemotePath().upToLastOccurrenceOf ("/", true, false));
            newCWD << itemList[row] << "/";
            curlSession->setRemotePath (newCWD);
            setContents (curlSession->getDirectoryListing());
        }
    }

    sendChangeMessage();
}

juce::var RemoteDirectoryListBoxModel::getDragSourceDescription (const juce::SparseSet<int>& currentlySelectedRows)
{
    if (curlSession != nullptr && currentlySelectedRows.size() > 0)
        return curlSession->getRemotePath().upToLastOccurrenceOf ("/", true, false) + itemList[currentlySelectedRows[0]];

    return juce::String();
}

//==============================================================================
RemoteDirectoryListBox::RemoteDirectoryListBox() :
    isInterestedInDrag (false)
{
    //session.setRemotePath ("ftp://www.aggravatedmusic.co.uk/rss/agro_news_feed.xml");
    session.setLocalFile (juce::File::getSpecialLocation (juce::File::userDesktopDirectory));

    model.addChangeListener (this);
    model.setCURLSession (&session);
    setModel (&model);
}

RemoteDirectoryListBox::~RemoteDirectoryListBox()
{
    model.removeChangeListener (this);
}

void RemoteDirectoryListBox::paintOverChildren (juce::Graphics& g)
{
    if (isInterestedInDrag)
    {
        g.setColour (juce::Colours::orange);
        g.drawRect (getLocalBounds(), 3);
    }
}

void RemoteDirectoryListBox::refresh()
{
    model.refresh();
    updateContent();
}

void RemoteDirectoryListBox::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &model)
        updateContent();
}

bool RemoteDirectoryListBox::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dynamic_cast<LocalDirectoryListBox*> (dragSourceDetails.sourceComponent.get()) != nullptr;
}

void RemoteDirectoryListBox::itemDragEnter (const SourceDetails& dragSourceDetails)
{
    isInterestedInDrag = isInterestedInDragSource (dragSourceDetails);
    repaint();
}

void RemoteDirectoryListBox::itemDragExit (const SourceDetails&)
{
    isInterestedInDrag = false;
    repaint();
}

void RemoteDirectoryListBox::itemDropped (const SourceDetails& dragSourceDetails)
{
    if (dynamic_cast<LocalDirectoryListBox*> (dragSourceDetails.sourceComponent.get()) != nullptr)
    {
        const juce::File file (dragSourceDetails.description.toString());
        juce::String localFileName (juce::File (dragSourceDetails.description.toString()).getFileName());

        session.setRemotePath (session.getRemotePath().upToLastOccurrenceOf ("/", true, false) + file.getFullPathName());
        session.setLocalFile (file);
        session.beginTransfer (true);
    }

    isInterestedInDrag = false;
    repaint();
}

#endif //DROWAUDIO_USE_CURL
