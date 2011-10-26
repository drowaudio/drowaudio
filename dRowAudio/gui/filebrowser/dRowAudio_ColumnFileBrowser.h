/*
 *  dRowAudio_ColumnFileBrowser.h
 *  dRowAudio
 *
 *  Created by David Rowland on 27/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_COLUMNFILEBROWSER_H__
#define __DROWAUDIO_COLUMNFILEBROWSER_H__

#include "dRowAudio_BasicFileBrowser.h"

//==================================================================================
class BrowserColumn :	public BasicFileBrowser,
						public DragAndDropContainer,
                        public ChangeBroadcaster
{
public:
    //==================================================================================
	BrowserColumn (WildcardFileFilter* filesToDisplay_, int columnNumber_)
        : BasicFileBrowser (BasicFileBrowser::openMode
                            + BasicFileBrowser::canSelectFiles
                            + BasicFileBrowser::canSelectDirectories
                            + BasicFileBrowser::canSelectMultipleItems,
                            File::getSpecialLocation (File::userMusicDirectory),
                            filesToDisplay_),
		  columnNumber (columnNumber_),
		  active (false),
		  fileDragEnabled (false)
	{
		addMouseListener(this, true);
	}
	
	~BrowserColumn()
	{
	}
			
	void mouseDrag (const MouseEvent& e)
	{
		if (fileDragEnabled)
		{
			StringArray files;

            for (int i = 0; i < getNumSelectedFiles(); i++) 
            {
                File file (getSelectedFile (i));

                if (file.existsAsFile()) 
                    files.add (file.getFullPathName());
            }

            fileDragEnabled = false;
            performExternalDragDropOfFiles (files, false);
		}
	}
	
	void mouseUp (const MouseEvent& e)
	{
		fileDragEnabled = false;
	}
	
	void selectionChanged()
	{
        BasicFileBrowser::selectionChanged();
        
        sendChangeMessage();
	}
		
	void fileClicked (const File& file, const MouseEvent& e)
	{
		fileDragEnabled = true;
	}
	
	void fileDoubleClicked (const File& f) {}
			
private:
    //==================================================================================
	int columnNumber;
	bool active, fileDragEnabled;
	
	friend class ColumnFileBrowserContents;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BrowserColumn);
};


//==================================================================================
class ColumnFileBrowserContents	:	public Component,
									public FileBrowserListener,
									public ChangeListener,
									public ComponentListener
{
public:
    //==================================================================================
	ColumnFileBrowserContents (WildcardFileFilter* filesToDisplay_, Viewport* parentViewport);
	
	~ColumnFileBrowserContents();
	
	void resized();
			
	void selectionChanged () {}
	void fileClicked (const File& file, const MouseEvent& e) {}
	void fileDoubleClicked (const File& file) {}
	void browserRootChanged (const File& newRoot) {}
    
	void selectedFileChanged (const File& file);
	bool addColumn (const File& rootDirectory);
	void removeColumn (int numColumns = 1);
	
	void changeListenerCallback (ChangeBroadcaster* changedComponent);
	void componentMovedOrResized (Component &component, bool wasMoved, bool wasResized);
	bool keyPressed (const KeyPress& key);
			
private:
    //==================================================================================
	WildcardFileFilter* filesToDisplay;
	Viewport* viewport;
	OwnedArray <BrowserColumn> columns;
	String currentFilePath;
	
	int startingWidth, activeColumn;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColumnFileBrowserContents);
};

//==================================================================================
class ColumnFileBrowser :	public Viewport
{
public:
    //==================================================================================
	ColumnFileBrowser (WildcardFileFilter* filesToDisplay_)
        : Viewport("ColumnFileBrowser"),
		  wildcard(filesToDisplay_)
	{
		addMouseListener (this, true);
		setWantsKeyboardFocus (false);
		
		setScrollBarsShown (false, true);
		setScrollBarThickness (10);
        
		fileBrowser = new ColumnFileBrowserContents (filesToDisplay_, this);
		setViewedComponent (fileBrowser);
	}
	
	~ColumnFileBrowser()
	{
	}
	
	void resized()
	{
		Viewport::resized();
		fileBrowser->setSize (fileBrowser->getWidth(), getMaximumVisibleHeight());
	}
		
	void visibleAreaChanged (const Rectangle<int>& newVisibleArea)
	{
		resized();
	}
	
	void mouseWheelMove (const MouseEvent& e, float wheelIncrementX, float wheelIncrementY)
	{
		if (! (e.mods.isAltDown() || e.mods.isCtrlDown()))
		{
			if (getHorizontalScrollBar()->isVisible())
				Viewport::useMouseWheelMoveIfNeeded (e, wheelIncrementX, 0);
			else
				Viewport::useMouseWheelMoveIfNeeded (e, wheelIncrementX, wheelIncrementY);
		}
	}
	
private:
    //==================================================================================
	ScopedPointer<WildcardFileFilter> wildcard;
	ColumnFileBrowserContents* fileBrowser;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColumnFileBrowser);
};

#endif //__DROWAUDIO_COLUMNFILEBROWSER_H__