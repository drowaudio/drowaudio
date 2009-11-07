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


//==================================================================================
class BrowserColumn :	public FileBrowserComponent,
						public DragAndDropContainer
{
public:
	BrowserColumn(WildcardFileFilter *filesToDisplay_)
	:	FileBrowserComponent(FileBrowserComponent::loadFileMode,
							 File::getSpecialLocation(File::userMusicDirectory),
							 filesToDisplay_,
							 0, false),
		active(false)
	{
		resizer = new ResizableCornerComponent(this, &resizeLimits);
		addAndMakeVisible(resizer);
	}
	
	~BrowserColumn()
	{
		deleteAllChildren();
	}
	
	void fileClicked (const File &file, const MouseEvent &e)
	{
		active = true;
		FileBrowserComponent::fileClicked(file, e);
		active = false;
		
		if (file.existsAsFile())
		{
			StringArray files;
			files.add(file.getFullPathName());
			performExternalDragDropOfFiles(files, false);
		}	
	}
		
	void resized()
	{
		int width = getWidth();
		int height = getHeight();
		int size = 10;
		
		resizeLimits.setSizeLimits (150, height, 800, height);
		resizer->setBounds(width - size, height - size, size, size);
		
		FileBrowserComponent::resized();
		sendChangeMessage(this);
	}
		
	int columnNumber;
	bool active;
	ResizableCornerComponent* resizer;
	ComponentBoundsConstrainer resizeLimits;
};

class BrowserColumnLookAndFeel : public LookAndFeel
{
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
		
		g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId));
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
									public ChangeListener
{
public:
	ColumnFileBrowserContents(WildcardFileFilter *filesToDisplay_, Viewport* parentViewport);
	
	~ColumnFileBrowserContents();
	
	void resized();
	
	void selectionChanged () {}
	void fileClicked (const File &file, const MouseEvent &e);
	void fileDoubleClicked (const File &file)	{}
	
	bool addColumn(const File &rootDirectory);
	void removeColumn(int noColumns=1);
	
	void changeListenerCallback(void* changedComponent)
	{
		resized();
	}
	
private:
		
	WildcardFileFilter* filesToDisplay;
	Viewport* viewport;
	OwnedArray <BrowserColumn> columns;
	
	int totalWidth;
	
	BrowserColumnLookAndFeel* lookAndFeel;
};

//==================================================================================
class ColumnFileBrowser :	public Viewport
{
public:
	ColumnFileBrowser (WildcardFileFilter *filesToDisplay_)
	{
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
	
private:
	
	ColumnFileBrowserContents* fileBrowser;
};
//==================================================================================

#endif //_DROWAUDIO_COLUMNFILEBROWSER__H_