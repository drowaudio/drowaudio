/*
 *  dRowAudio_ColumnFileBrowser.h
 *  dRowAudio
 *
 *  Created by David Rowland on 27/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_COLUMNFILEBROWSER__H_
#define _DROWAUDIO_COLUMNFILEBROWSER__H_

#include "../../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_BasicFileBrowser.h"

//==================================================================================
class BrowserColumn :	public BasicFileBrowser,
						public DragAndDropContainer
{
public:
	BrowserColumn(WildcardFileFilter *filesToDisplay_, int _columnNumber)
	:	BasicFileBrowser(BasicFileBrowser::loadFileMode,
						 File::getSpecialLocation(File::userMusicDirectory),
						 filesToDisplay_),
		columnNumber(_columnNumber),
		active(false),
		fileDragEnabled(false)
	{
		addMouseListener(this, true);
	}
	
	~BrowserColumn()
	{
		deleteAllChildren();
	}
			
	void mouseDrag (const MouseEvent& e)
	{
		if (fileDragEnabled)
		{
			StringArray files;
			files.add(currentFile);
			File file(currentFile);
			if (file.existsAsFile()) {
				fileDragEnabled = false;
				performExternalDragDropOfFiles(files, false);
			}		
		}
	}
	
	void mouseUp (const MouseEvent& e)
	{
		fileDragEnabled = false;
	}
	
	void selectionChanged()
	{
		if (getHighlightedFile().exists()) {
			sendChangeMessage();
		}
	}
		
	void fileClicked (const File &file, const MouseEvent &e)
	{
		currentFile = file.getFullPathName();
		fileDragEnabled = true;
	}
	
	void fileDoubleClicked (const File& f){}
		
	void resized()
	{		
		BasicFileBrowser::resized();
	}
	
private:
	int columnNumber;
	bool active, fileDragEnabled;
	String currentFile;
	
	friend class ColumnFileBrowserContents;
};


//==================================================================================
class ColumnFileBrowserContents	:	public Component,
									public FileBrowserListener,
									public ChangeListener,
									public ComponentListener
{
public:
	ColumnFileBrowserContents(WildcardFileFilter *filesToDisplay_, Viewport* parentViewport);
	
	~ColumnFileBrowserContents();
	
	void resized();
			
	void selectionChanged () {}
	void fileClicked (const File &file, const MouseEvent &e)	{}
	void fileDoubleClicked (const File &file)	{}
	
	void selectedFileChanged(const File &file);
	bool addColumn(const File &rootDirectory);
	void removeColumn(int noColumns=1);
	
	void changeListenerCallback(ChangeBroadcaster* changedComponent)
	{
		BrowserColumn* changedColumn = (BrowserColumn*)changedComponent;
		
		columns[activeColumn]->getLookAndFeel().setColour(DirectoryContentsDisplayComponent::highlightColourId,
														  Colour::greyLevel(0.9f));
		columns[activeColumn]->repaint();
		
		activeColumn = changedColumn->columnNumber;

		File tempFile(changedColumn->getHighlightedFile());
		selectedFileChanged(tempFile);
		columns[activeColumn]->getLookAndFeel().setColour(DirectoryContentsDisplayComponent::highlightColourId, Colours::darkorange);
		columns[activeColumn]->repaint();
	}
		
	void componentMovedOrResized (Component &component, bool wasMoved, bool wasResized)
	{
		if (wasResized)
			resized();
	}
	
	bool keyPressed (const KeyPress& key)
	{
		if (key.isKeyCode (KeyPress::leftKey))
		{
			if (activeColumn != 0)
			{
				columns[activeColumn]->deselectAllRows();
				int newActiveColumn = jmax(0,activeColumn-1);
				columns[newActiveColumn]->selectionChanged();
				columns[newActiveColumn]->grabKeyboardFocus();
			}
			return true;
		}
		else if (key.isKeyCode (KeyPress::rightKey))
		{
			if(columns[activeColumn]->getHighlightedFile().isDirectory())
			{
				int newActiveColumn = activeColumn+1;
				addColumn(columns[newActiveColumn]->getHighlightedFile());
				if (columns[newActiveColumn]->getNumRows() )
				{
					columns[newActiveColumn]->grabKeyboardFocus();
					columns[newActiveColumn]->selectRow(0);
				}
			}
			return true;
		}
		return false;
	}
			
private:
		
	WildcardFileFilter* filesToDisplay;
	Viewport* viewport;
	OwnedArray <BrowserColumn> columns;
	String currentFilePath;
	
	int startingWidth, activeColumn;
	
//	BrowserColumnLookAndFeel* lookAndFeel;
};

//==================================================================================
class ColumnFileBrowser :	public Viewport
{
public:
	ColumnFileBrowser (WildcardFileFilter *filesToDisplay_)
	:	Viewport("ColumnFileBrowser"),
		wildcard(filesToDisplay_)
	{
		addMouseListener(this, true);
		setWantsKeyboardFocus(false);
		
		setScrollBarsShown(false, true);
		
		fileBrowser = new ColumnFileBrowserContents(filesToDisplay_, this);
		setViewedComponent(fileBrowser);
	}
	
	~ColumnFileBrowser()
	{
	}
	
	void resized()
	{
		Viewport::resized();
		fileBrowser->setSize(fileBrowser->getWidth(), getMaximumVisibleHeight());
	}
		
	void visibleAreaChanged(int visibleX, int visibleY,
							int visibleW, int visibleH)
	{
		resized();
	}
	
	void mouseWheelMove (const MouseEvent &e, float wheelIncrementX, float wheelIncrementY)
	{
		if (! (e.mods.isAltDown() || e.mods.isCtrlDown()))
		{
			if (getHorizontalScrollBar()->isVisible())
				Viewport::useMouseWheelMoveIfNeeded(e, wheelIncrementX, 0);
			else
				Viewport::useMouseWheelMoveIfNeeded(e, wheelIncrementX, wheelIncrementY);
		}
	}
	
private:
	
	ScopedPointer<WildcardFileFilter> wildcard;
	ColumnFileBrowserContents *fileBrowser;
};
//==================================================================================

#endif //_DROWAUDIO_COLUMNFILEBROWSER__H_