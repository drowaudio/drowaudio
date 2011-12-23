/*
  ==============================================================================

    LocalDirectoryListBox.cpp
    Created: 19 May 2011 1:54:53am
    Author:  David Rowland

  ==============================================================================
*/

#include "LocalDirectoryListBox.h"
#include "RemoteDirectoryListBox.h"

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
	const int m = height * 0.15;

	if (currentWorkingDirectory.getChildFile(itemList[rowNumber]).isDirectory()) {
		LookAndFeel::getDefaultLookAndFeel().getDefaultFolderImage()->drawWithin(g, Rectangle<float>(m, m, h-2*m, h-2*m), RectanglePlacement(0), 1.0f);
	}
	else {
		LookAndFeel::getDefaultLookAndFeel().getDefaultDocumentFileImage()->drawWithin(g, Rectangle<float>(m, m, h-2*m, h-2*m), RectanglePlacement(RectanglePlacement::centred), 1.0f);
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

void LocalDirectoryListBoxModel::listBoxItemDoubleClicked(int row, const MouseEvent &e)
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
		if (currentWorkingDirectory.getChildFile(itemList[currentlySelectedRows[0]]).existsAsFile())
		{
			return currentWorkingDirectory.getChildFile(itemList[currentlySelectedRows[0]]).getFullPathName();
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

void LocalDirectoryListBox::itemDragExit (const SourceDetails& dragSourceDetails)
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

/*LocalDirectoryListBox::LocalDirectoryListBox(int flags_,
											 const File& initialFileOrDirectory,
//											 const FileFilter* fileFilter_,
											 FilePreviewComponent* previewComp_)
:	FileBrowserComponent(flags_,
						 initialFileOrDirectory,
						 //fileFilter_,
						 previewComp_)
{
}

LocalDirectoryListBox::~LocalDirectoryListBox()
{
}

void LocalDirectoryListBox::fileClicked(const File &f, const MouseEvent &e)
{
	FileBrowserComponent::fileClicked(f, e);
	
	startDragging(f.getFullPathName(),
				  this);
//	if (e.getDistanceFromDragStart()) {
//		DBG("file dragged");
//	}
}

bool LocalDirectoryListBox::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
	return true;
}

void LocalDirectoryListBox::itemDropped (const SourceDetails& dragSourceDetails)
{
	RemoteDirectoryListBox *remote = dynamic_cast<RemoteDirectoryListBox*> (dragSourceDetails.sourceComponent.get());
	
	if (remote != nullptr)
	{
		DBG(dragSourceDetails.description.toString());
		String remoteFileName(dragSourceDetails.description.toString().fromLastOccurrenceOf("/", true, false));
		DBG(remoteFileName);
		remote->getCURLSession().setRemotePath(dragSourceDetails.description.toString());
//		remote->getCURLSession()->setLocalPath(getSelectedFile(0).getParentDirectory().getFullPathName()+remoteFileName);
		remote->getCURLSession().setLocalPath(getRoot().getFullPathName()+remoteFileName);
		remote->getCURLSession().beginTransfer(false);
	}
}*/

//void LocalDirectoryListBox::mouseUp(const MouseEvent& e)
//{
//	FileBrowserComponent::mouseUp(e);
//	isMouseDragging = false;
//}
//
//void LocalDirectoryListBox::mouseExit(const MouseEvent& e)
//{
//	FileBrowserComponent::mouseExit(e);
//	isMouseDragging = false;
//}
//
//void LocalDirectoryListBox::mouseDrag(const MouseEvent& e)
//{
//	FileBrowserComponent::mouseDrag(e);
//	
//	isMouseDragging = true;
//	startDragging(getSelectedFile(<#int index#>), <#Component *sourceComponent#>, <#const Image dragImage#>, <#bool allowDraggingToOtherJuceWindows#>, <#const Point *imageOffsetFromMouse#>)
//}