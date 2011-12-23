/*
  ==============================================================================

    RemoteDirectoryListBox.cpp
    Created: 18 May 2011 11:48:14pm
    Author:  David Rowland

  ==============================================================================
*/

#include "RemoteDirectoryListBox.h"
#include "LocalDirectoryListBox.h"

RemoteDirectoryListBoxModel::RemoteDirectoryListBoxModel()
:	curlSession(nullptr)
{
}

RemoteDirectoryListBoxModel::~RemoteDirectoryListBoxModel()
{
}

void RemoteDirectoryListBoxModel::setCURLSession(CURLEasySession *sessionToControl)
{
	curlSession = sessionToControl;
	setContents(curlSession->getDirectoryListing());
	sendChangeMessage();
}

void RemoteDirectoryListBoxModel::refresh()
{
	setContents(curlSession->getDirectoryListing());
}

int RemoteDirectoryListBoxModel::getNumRows()
{
	return itemList.size();
}

void RemoteDirectoryListBoxModel::paintListBoxItem (int rowNumber,
													Graphics& g,
													int width, int height,
													bool rowIsSelected)
{
	if (rowIsSelected) {
		g.setColour(Colours::lightblue);
		g.fillAll();
	}
	
	g.setColour(Colours::black);
	g.drawFittedText(itemList[rowNumber],
					 0, 0, width, height,
					 Justification::centredLeft, 1);
}

void RemoteDirectoryListBoxModel::setContents(StringArray newContents)
{
	itemList = newContents;
}

void RemoteDirectoryListBoxModel::listBoxItemDoubleClicked(int row, const MouseEvent &e)
{
	DBG(itemList[row]);
	
	//*** Need to navigate session here
	if (itemList[row] == "..") {
		DBG("move up");
		curlSession->setRemotePath(curlSession->getRemotePath().upToLastOccurrenceOf("/", false, false));
		setContents(curlSession->getDirectoryListing());
	}
	else if (! itemList[row].contains(".")) {
		DBG("directory");
		String newCWD(curlSession->getRemotePath().upToLastOccurrenceOf("/", true, false));
		newCWD<<itemList[row]<<"/";
		DBG(newCWD);
		curlSession->setRemotePath(newCWD);
		setContents(curlSession->getDirectoryListing());
	}

	sendChangeMessage();
}

var RemoteDirectoryListBoxModel::getDragSourceDescription (const SparseSet<int> &currentlySelectedRows)
{
	if (currentlySelectedRows.size() > 0) {
		return curlSession->getRemotePath().upToLastOccurrenceOf("/", true, false)+itemList[currentlySelectedRows[0]];
	}
	
	return String::empty;
}

//==============================================================================
RemoteDirectoryListBox::RemoteDirectoryListBox()
    : isInterestedInDrag (false)
{
//	session.setLocalFile (File ("/Users/Dave/Documents/Developement/Juce Projects/cURL/Builds/MacOSX/build/Debug/agro_download_test2.xml"));
//	session.setRemotePath("ftp://www.aggravatedmusic.co.uk/rss/agro_news_feed.xml");
	session.setLocalFile (File::getSpecialLocation (File::userDesktopDirectory));
	
	model.addChangeListener(this);
	model.setCURLSession(&session);
	setModel(&model);
}

RemoteDirectoryListBox::~RemoteDirectoryListBox()
{
	model.removeChangeListener(this);
}

void RemoteDirectoryListBox::paintOverChildren (Graphics& g)
{
    if (isInterestedInDrag) 
    {
        g.setColour (Colours::orange);
        g.drawRect (getLocalBounds(), 3);
    }
}

void RemoteDirectoryListBox::refresh()
{
	model.refresh();
	updateContent();
}

void RemoteDirectoryListBox::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &model)
	{
		DBG("updateContent");
		updateContent();
	}
}

bool RemoteDirectoryListBox::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    LocalDirectoryListBox *local = dynamic_cast<LocalDirectoryListBox*> (dragSourceDetails.sourceComponent.get());

	return local != nullptr;
}

void RemoteDirectoryListBox::itemDragEnter (const SourceDetails& dragSourceDetails)
{
    isInterestedInDrag = isInterestedInDragSource (dragSourceDetails);
    repaint();    
}

void RemoteDirectoryListBox::itemDragExit (const SourceDetails& dragSourceDetails)
{
    isInterestedInDrag = false;
    repaint();
}

void RemoteDirectoryListBox::itemDropped (const SourceDetails& dragSourceDetails)
{
	LocalDirectoryListBox *local = dynamic_cast<LocalDirectoryListBox*> (dragSourceDetails.sourceComponent.get());
	
	if (local != nullptr)
	{
		DBG(dragSourceDetails.description.toString());
		{
			String localFileName(File(dragSourceDetails.description.toString()).getFileName());
			
			session.setRemotePath(session.getRemotePath().upToLastOccurrenceOf("/", true, false)+localFileName);
			session.setLocalFile (File (dragSourceDetails.description.toString()));
		}
		session.beginTransfer(true);
	}
}
