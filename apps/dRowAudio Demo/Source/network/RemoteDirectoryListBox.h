/*
  ==============================================================================

    RemoteDirectoryListBox.h
    Created: 18 May 2011 11:48:14pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __REMOTEDIRECTORYLISTBOX_H_780D9778__
#define __REMOTEDIRECTORYLISTBOX_H_780D9778__

#include "../../JuceLibraryCode/JuceHeader.h"

class RemoteDirectoryListBoxModel : public ListBoxModel,
									public ChangeBroadcaster
{
public:
	
	RemoteDirectoryListBoxModel();
	
	~RemoteDirectoryListBoxModel();
	
	void setCURLSession(CURLEasySession *sessionToControl);
	
	void refresh();
	
	int getNumRows();

	void paintListBoxItem (int rowNumber,
						   Graphics& g,
                           int width, int height,
                           bool rowIsSelected);	
	
	void setContents(StringArray newContents);
	
	void listBoxItemDoubleClicked(int row, const MouseEvent &e);
	
	var getDragSourceDescription (const SparseSet<int> &currentlySelectedRows);

private:
	
	StringArray itemList;
	CURLEasySession* curlSession;
};


class RemoteDirectoryListBox :	public ListBox,
								public ChangeListener,
								public DragAndDropTarget
{
public:
	RemoteDirectoryListBox();
	
	~RemoteDirectoryListBox();

    void paintOverChildren (Graphics& g);

	CURLEasySession& getCURLSession()	{	return session;	}
	
	String getLastUrl()	{	return session.getCurrentWorkingDirectory();	}
	
	void refresh();
	
	void changeListenerCallback(ChangeBroadcaster* source);
	
	bool isInterestedInDragSource (const SourceDetails& dragSourceDetails);
	
    void itemDragEnter (const SourceDetails& dragSourceDetails);
    
    void itemDragExit (const SourceDetails& dragSourceDetails);
    
    void itemDropped (const SourceDetails& dragSourceDetails);
	
private:
	
	RemoteDirectoryListBoxModel model;
	CURLEasySession session;
    bool isInterestedInDrag;
};

#endif  // __REMOTEDIRECTORYLISTBOX_H_780D9778__
