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

#ifndef __LOCALDIRECTORYLISTBOX_H_AEB1756D__
#define __LOCALDIRECTORYLISTBOX_H_AEB1756D__

#include "../DemoHeader.h"

#if DROWAUDIO_USE_CURL

class LocalDirectoryListBoxModel :  public ListBoxModel,
									public ChangeBroadcaster
{
public:
	
	LocalDirectoryListBoxModel();
	
	~LocalDirectoryListBoxModel();
	
	const File& getCurrentWorkingDirectory()	{	return currentWorkingDirectory;	}
	
	int getNumRows();
	
	void paintListBoxItem (int rowNumber,
						   Graphics& g,
                           int width, int height,
                           bool rowIsSelected);	
	
	void setContents(StringArray newContents);
	
	void refresh();

	void listBoxItemDoubleClicked(int row, const MouseEvent &e);
	
	var getDragSourceDescription (const SparseSet<int> &currentlySelectedRows);
	
private:
	
	StringArray itemList;
	File currentWorkingDirectory;
};


class LocalDirectoryListBox :	public ListBox,
								public ChangeListener,
								public DragAndDropTarget
{
public:
	LocalDirectoryListBox();
	
	~LocalDirectoryListBox();
		
    void paintOverChildren (Graphics& g);

	void changeListenerCallback(ChangeBroadcaster* source);
	
	bool isInterestedInDragSource (const SourceDetails& dragSourceDetails);
	
    void itemDragEnter (const SourceDetails& dragSourceDetails);
    
    void itemDragExit (const SourceDetails& dragSourceDetails);
    
    void itemDropped (const SourceDetails& dragSourceDetails);
	
private:
	
	LocalDirectoryListBoxModel model;
    bool isInterestedInDrag;
};

/*class LocalDirectoryListBox :	public FileBrowserComponent,
								public DragAndDropTarget,
								public DragAndDropContainer
{
public:
	LocalDirectoryListBox(int flags,
                          const File& initialFileOrDirectory,
//                          const FileFilter* fileFilter,
                          FilePreviewComponent* previewComp);
	
	~LocalDirectoryListBox();
	
	void fileClicked(const File &f, const MouseEvent &e);
	
	bool isInterestedInDragSource (const SourceDetails& dragSourceDetails);

    void itemDropped (const SourceDetails& dragSourceDetails);

//	void mouseUp(const MouseEvent& e);
//
//	void mouseExit(const MouseEvent& e);
//
//	void mouseDrag(const MouseEvent& e);
	
private:
	
//	bool isMouseDragging;
};*/

#endif
#endif  // __LOCALDIRECTORYLISTBOX_H_AEB1756D__
