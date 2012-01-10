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

#include "LocalDirectoryListBox.h"
#include "RemoteDirectoryListBox.h"

#if DROWAUDIO_USE_CURL

static StringArray getFilesForDirectory(String fullPath)
{
	StringArray files;
	files.add("..");
	
	File cwd(fullPath);
	if (cwd.isDirectory())
	{
		Array<File> childFiles;
		cwd.findChildFiles(childFiles, File::findFilesAndDirectories, false);

		for (int i = 0; i < childFiles.size(); ++i)
		{
			files.add(childFiles[i].getFileName());
		}
	}
	
	return files;
}

LocalDirectoryListBoxModel::LocalDirectoryListBoxModel()
:	currentWorkingDirectory (File::getSpecialLocation (File::userDesktopDirectory))
{
	setContents(getFilesForDirectory(currentWorkingDirectory.getFullPathName()));
}

LocalDirectoryListBoxModel::~LocalDirectoryListBoxModel()
{
}

int LocalDirectoryListBoxModel::getNumRows()
{
	return itemList.size();
}

void LocalDirectoryListBoxModel::paintListBoxItem (int rowNumber,
													Graphics& g,
													int width, int height,
													bool rowIsSelected)
{
	if (rowIsSelected) {
		g.setColour(Colours::lightblue);
		g.fillAll();
	}
	
	const int h = height;
	const int m = height * 0.15f;

	if (currentWorkingDirectory.getChildFile(itemList[rowNumber]).isDirectory()) {
		LookAndFeel::getDefaultLookAndFeel().getDefaultFolderImage()->drawWithin(g, juce::Rectangle<float>(m, m, h-2*m, h-2*m), RectanglePlacement(0), 1.0f);
	}
	else {
		LookAndFeel::getDefaultLookAndFeel().getDefaultDocumentFileImage()->drawWithin(g, juce::Rectangle<float>(m, m, h-2*m, h-2*m), RectanglePlacement(RectanglePlacement::centred), 1.0f);
	}
	
	g.setColour(Colours::black);
	g.drawFittedText(itemList[rowNumber],
					 height, 0, width-h-m, height,
					 Justification::centredLeft, 1);
}

void LocalDirectoryListBoxModel::setContents(StringArray newContents)
{
	itemList = newContents;
}

void LocalDirectoryListBoxModel::refresh()
{
	setContents(getFilesForDirectory(currentWorkingDirectory.getFullPathName()));
	sendChangeMessage();
}

void LocalDirectoryListBoxModel::listBoxItemDoubleClicked(int row, const MouseEvent& /*e*/)
{
	if (itemList[row] == "..")
	{
		currentWorkingDirectory = (currentWorkingDirectory.getParentDirectory());
		setContents(getFilesForDirectory(currentWorkingDirectory.getFullPathName()));
	}
	else if (currentWorkingDirectory.getChildFile(itemList[row]).isDirectory())
	{
		currentWorkingDirectory = (currentWorkingDirectory.getChildFile(itemList[row]));
		if (currentWorkingDirectory.isDirectory()) 
		{
			setContents(getFilesForDirectory(currentWorkingDirectory.getFullPathName()));
		}
	}
	
	sendChangeMessage();
}

var LocalDirectoryListBoxModel::getDragSourceDescription (const SparseSet<int> &currentlySelectedRows)
{
	if (currentlySelectedRows.size() > 0)
	{
		if (currentWorkingDirectory.getChildFile (itemList[currentlySelectedRows[0]]).existsAsFile())
		{
			return currentWorkingDirectory.getChildFile (itemList[currentlySelectedRows[0]]).getFullPathName();
		}
	}
	
	return String::empty;
}

//==============================================================================
LocalDirectoryListBox::LocalDirectoryListBox()
    : isInterestedInDrag (false)
{
	model.addChangeListener(this);
	setModel(&model);
}

LocalDirectoryListBox::~LocalDirectoryListBox()
{
	model.removeChangeListener(this);
}

void LocalDirectoryListBox::paintOverChildren (Graphics& g)
{
    if (isInterestedInDrag) 
    {
        g.setColour (Colours::orange);
        g.drawRect (getLocalBounds(), 3);
    }
}

void LocalDirectoryListBox::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &model)
	{
		DBG("updateContent");
		updateContent();
		repaint();
	}
}

bool LocalDirectoryListBox::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    RemoteDirectoryListBox *remote = dynamic_cast<RemoteDirectoryListBox*> (dragSourceDetails.sourceComponent.get());

	return remote != nullptr;
}

void LocalDirectoryListBox::itemDragEnter (const SourceDetails& dragSourceDetails)
{
    isInterestedInDrag = isInterestedInDragSource (dragSourceDetails);
    repaint();    
}

void LocalDirectoryListBox::itemDragExit (const SourceDetails& /*dragSourceDetails*/)
{
    isInterestedInDrag = false;
    repaint();
}

void LocalDirectoryListBox::itemDropped (const SourceDetails& dragSourceDetails)
{
	RemoteDirectoryListBox *remote = dynamic_cast<RemoteDirectoryListBox*> (dragSourceDetails.sourceComponent.get());
	
	if (remote != nullptr)
	{
		DBG(dragSourceDetails.description.toString());
		String remoteFileName(dragSourceDetails.description.toString().fromLastOccurrenceOf("/", true, false));
		
		CURLEasySession& session(remote->getCURLSession());
		session.setRemotePath(dragSourceDetails.description.toString());
		session.setLocalFile (File (model.getCurrentWorkingDirectory().getFullPathName()+remoteFileName));
		session.beginTransfer(false);
		model.refresh();
	}
    
    isInterestedInDrag = false;
    repaint();
}

#endif