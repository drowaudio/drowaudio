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

#include <juce/juce.h>
#include "dRowAudio_BasicFileBrowser.h"

//==================================================================================
class BrowserColumn :	public BasicFileBrowser,
						public DragAndDropContainer
{
public:
	BrowserColumn(WildcardFileFilter *filesToDisplay_, int _columnNumber)
	:	BasicFileBrowser(BasicFileBrowser::loadFileMode,
							 File::getSpecialLocation(File::userMusicDirectory),
							 filesToDisplay_,
							 0, false),
		columnNumber(_columnNumber),
		active(false),
		fileDragEnabled(false)
	{
		addMouseListener(this, true);
		resizer = new ResizableCornerComponent(this, &resizeLimits);
		addAndMakeVisible(resizer);
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
			sendChangeMessage(this);
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
		int width = getWidth();
		int height = getHeight();
		int size = 10;
		
		resizeLimits.setSizeLimits (150, height, 800, height);
		resizer->setBounds(width - size, height - size, size, size);
		
		BasicFileBrowser::resized();
	}

private:
	int columnNumber;
	bool active, fileDragEnabled;
	String currentFile;
	ResizableCornerComponent* resizer;
	ComponentBoundsConstrainer resizeLimits;
	
	friend class ColumnFileBrowserContents;
};

class BrowserColumnLookAndFeel : public LookAndFeel
{
public:
	
	BrowserColumnLookAndFeel()
	{
		setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2));
		setColour(DirectoryContentsDisplayComponent::highlightColourId, Colour::greyLevel(0.9));
		setColour(DirectoryContentsDisplayComponent::textColourId, Colour::greyLevel(0.9));
	}
	
	void layoutFileBrowserComponent (FileBrowserComponent& browserComp,
									 DirectoryContentsDisplayComponent* fileListComponent,
									 FilePreviewComponent* previewComp,
									 ComboBox* currentPathBox,
									 TextEditor* filenameBox,
									 Button* goUpButton)
	{
		int w = browserComp.getWidth();
		int x = 2;
		int y = 2;
				
		Component* const listAsComp = dynamic_cast <Component*> (fileListComponent);
		listAsComp->setBounds (x, y, w, browserComp.getHeight() - y);// - bottomSectionHeight);
		
		y = listAsComp->getBottom();
	}
	
	void drawFileBrowserRow (Graphics& g, int width, int height,
							 const String& filename, Image* icon,
							 const String& fileSizeDescription,
							 const String& fileTimeDescription,
							 const bool isDirectory,
							 const bool isItemSelected,
							 const int itemIndex)
	{
		if (isItemSelected)
			g.fillAll (findColour (DirectoryContentsDisplayComponent::highlightColourId));
//		else if ((itemIndex % 2) != 0)
//			g.fillAll (findColour(ListBox::backgroundColourId).withBrightness(0.95f));
//		else if ((itemIndex % 2) == 0)
//			g.fillAll (browserColumnRow.withAlpha(0.05f));
		
		g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId));
		if (isItemSelected)
			g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId).contrasting(1.0f));
		g.setFont (height * 0.7f);
		
		Image* im = icon;
		Image* toRelease = 0;
		
		if (im == 0)
		{
			toRelease = im = (isDirectory ? getDefaultFolderImage()
							  : getDefaultDocumentFileImage());
		}
		
		const int x = 32;
		
		if (im != 0)
		{
			g.drawImageWithin (im, 2, 2, x - 4, height - 4,
							   RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
							   false);
			
			ImageCache::release (toRelease);
		}
		
		if (width > 450 && ! isDirectory)
		{
			const int sizeX = roundFloatToInt (width * 0.7f);
			const int dateX = roundFloatToInt (width * 0.8f);
			
			g.drawFittedText (filename,
							  x, 0, sizeX - x, height,
							  Justification::centredLeft, 1);
			
			g.setFont (height * 0.5f);
			g.setColour (Colours::darkgrey);
			
			if (! isDirectory)
			{
				g.drawFittedText (fileSizeDescription,
								  sizeX, 0, dateX - sizeX - 8, height,
								  Justification::centredRight, 1);
				
				g.drawFittedText (fileTimeDescription,
								  dateX, 0, width - 8 - dateX, height,
								  Justification::centredRight, 1);
			}
		}
		else
		{
			g.drawFittedText (filename,
							  x, 0, width - x, height,
							  Justification::centredLeft, 1);
			
		}
		
		if (isDirectory)
		{			
			int diameter = height * 0.5f;
			Path p;
			p.addTriangle (width - (height * 0.2f), height * 0.5f,
						   (width - diameter), height * 0.3f,
						   (width - diameter), height * 0.7f);
			g.setColour (findColour (ScrollBar::thumbColourId));
			g.fillPath (p);
			
			g.setColour (Colour (0x80000000));
			g.strokePath (p, PathStrokeType (0.5f));
		}
	}
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
	
	void changeListenerCallback(void* changedComponent)
	{
		BrowserColumn* changedColumn = (BrowserColumn*)changedComponent;
		activeColumn = changedColumn->columnNumber;
		File tempFile(changedColumn->getHighlightedFile());
		selectedFileChanged(tempFile);
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
	
	BrowserColumnLookAndFeel* lookAndFeel;
};

//==================================================================================
class ColumnFileBrowser :	public Viewport
{
public:
	ColumnFileBrowser (WildcardFileFilter *filesToDisplay_)
	{
		addMouseListener(this, true);
		setWantsKeyboardFocus(false);
		
		fileBrowser = new ColumnFileBrowserContents(filesToDisplay_, this);
		setViewedComponent(fileBrowser);
	}
	
	void resized()
	{
		fileBrowser->setSize(fileBrowser->getWidth(), getMaximumVisibleHeight());
	}
	
	void visibleAreaChanged (int visibleX, int visibleY, int visibleW, int visibleH)
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
	
	ColumnFileBrowserContents* fileBrowser;
};
//==================================================================================

#endif //_DROWAUDIO_COLUMNFILEBROWSER__H_