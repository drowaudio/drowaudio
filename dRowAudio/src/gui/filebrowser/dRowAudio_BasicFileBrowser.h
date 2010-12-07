/*
 *  dRowAudio_BasicFileBrowser.h
 *  dRowAudio
 *
 *  Created by David Rowland on 05/11/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_BASICFILEBROWSER__H_
#define _DROWAUDIO_BASICFILEBROWSER__H_

#include "../../core/dRowAudio_StandardHeader.h"

class BasicFileBrowserLookAndFeel : public LookAndFeel,
									public DeletedAtShutdown
{
public:
//	juce_DeclareSingleton (BasicFileBrowserLookAndFeel, false)
	
	BasicFileBrowserLookAndFeel()
	{
		setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2));
		setColour(DirectoryContentsDisplayComponent::highlightColourId, Colour::greyLevel(0.9));
		setColour(DirectoryContentsDisplayComponent::textColourId, Colour::greyLevel(0.9));		
	}
	
	~BasicFileBrowserLookAndFeel()
	{
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
							 bool isDirectory,
							 bool isItemSelected,
							 int itemIndex,
							 DirectoryContentsDisplayComponent& component)
	{
		if (isItemSelected)
			g.fillAll (findColour (DirectoryContentsDisplayComponent::highlightColourId));
		//		else if ((itemIndex % 2) != 0)
		//			g.fillAll (findColour(ListBox::backgroundColourId).withBrightness(0.95f));
		//		else if ((itemIndex % 2) == 0)
		//			g.fillAll (browserColumnRow.withAlpha(0.05f));
		
//		g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId));
//		if (isItemSelected)
//			g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId).contrasting(1.0f));
//		g.setFont (height * 0.7f);
//		
//		Image im;
//		if (icon != 0 && icon->isValid())
//			im = *icon;
//		
//		if (im.isNull())
//			im = isDirectory ? getDefaultFolderImage()
//			: getDefaultDocumentFileImage();
//		
//		const int x = 32;
//		
//		if (im.isValid())
//		{
//			g.drawImageWithin (im, 2, 2, x - 4, height - 4,
//							   RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
//							   false);
//		}
		const int x = 32;
		g.setColour (Colours::black);
		
		if (icon != 0 && icon->isValid())
		{
			g.drawImageWithin (*icon, 2, 2, x - 4, height - 4,
							   RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
							   false);
		}
		else
		{
			const Drawable* d = isDirectory ? getDefaultFolderImage()
			: getDefaultDocumentFileImage();
			
			if (d != 0)
				d->drawWithin (g, Rectangle<float> (2.0f, 2.0f, x - 4.0f, height - 4.0f),
							   RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
		}
		
		g.setColour (findColour (DirectoryContentsDisplayComponent::textColourId));
		if (isItemSelected)
			g.setColour (findColour (DirectoryContentsDisplayComponent::highlightColourId).contrasting(1.0f));

		g.setFont (height * 0.7f);
		
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
							  x, 0, width - x - (height * 0.7f), height,
							  Justification::centredLeft, 1);
			
		}
		
		// draw directory triangles
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
	
    void drawCornerResizer (Graphics& g,
                            int w, int h,
                            bool isMouseOver,
                            bool isMouseDragging)
	{
		const float lineThickness = 1.0f;//jmin (w, h) * 0.075f;
		const float xGap = w / 3.0f;
		const float yGap = h * 0.25;
		
		g.setColour(Colours::lightgrey);
		g.drawLine(xGap + lineThickness*0.5, yGap, xGap + lineThickness*0.5, h - yGap, lineThickness);
		g.drawLine((2 * xGap) - lineThickness*0.5, yGap, (2 * xGap) - lineThickness*0.5, h - yGap, lineThickness);
	}
};
//juce_ImplementSingleton (BasicFileBrowserLookAndFeel)

//==================================================================================
class  BasicFileBrowser		:	public Component,
								public ChangeBroadcaster,
								private FileBrowserListener
{
public:
    //==============================================================================
    /** Various modes that the browser can be used in.
	 
	 One of these is passed into the constructor.
	 */
    enum FileChooserMode
    {
        loadFileMode,           /**< the component should allow the user to choose an existing
								 file with the intention of opening it. */
        saveFileMode,           /**< the component should allow the user to specify the name of
								 a file that will be used to save something. */
        chooseDirectoryMode     /**< the component should allow the user to select an existing
								 directory. */
    };
	
    //==============================================================================
    /** Creates a BasicFileBrowser.
	 
	 @param browserMode              The intended purpose for the browser - see the
	 FileChooserMode enum for the various options
	 @param initialFileOrDirectory   The file or directory that should be selected when
	 the component begins. If this is File::nonexistent,
	 a default directory will be chosen.
	 @param fileFilter               an optional filter to use to determine which files
	 are shown. If this is 0 then all files are displayed. Note
	 that a pointer is kept internally to this object, so
	 make sure that it is not deleted before the browser object
	 is deleted.
	 */
    BasicFileBrowser (FileChooserMode browserMode,
					  const File& initialFileOrDirectory,
                      const FileFilter* fileFilter);
	
    /** Destructor. */
    ~BasicFileBrowser();
	
    //==============================================================================
    /**
	 */
    const File getCurrentFile() const throw();
	
	const File getHighlightedFile() const throw();

    /** Returns true if the current file is usable.
	 
	 This can be used to decide whether the user can press "ok" for the
	 current file. What it does depends on the mode, so for example in an "open"
	 mode, the current file is only valid if one has been selected and if the file
	 exists. In a "save" mode, a non-existent file would also be valid.
	 */
    bool currentFileIsValid() const;
	
    //==============================================================================
    /** Returns the directory whose contents are currently being shown in the listbox. */
    const File getRoot() const;
	
    /** Changes the directory that's being shown in the listbox. */
    void setRoot (const File& newRootDirectory);
	
    /** Refreshes the directory that's currently being listed. */
    void refresh();
	
    /** Returns the browser's current mode. */
    FileChooserMode getMode() const throw()                     { return mode; }
	
    /** Returns a verb to describe what should happen when the file is accepted.
	 
	 E.g. if browsing in "load file" mode, this will be "Open", if in "save file"
	 mode, it'll be "Save", etc.
	 */
    virtual const String getActionVerb() const;
	
    //==============================================================================
    /** Adds a listener to be told when the user selects and clicks on files.
	 
	 @see removeListener
	 */
    void addListener (FileBrowserListener* const listener) throw();
	
    /** Removes a listener.
	 
	 @see addListener
	 */
    void removeListener (FileBrowserListener* const listener) throw();

	void mouseDoubleClick (const MouseEvent &e);
	//==============================================================================
	int getNumRows()				{	return fileListComponent->getNumRows();		}
	void selectRow(int rowNumber)	{	fileListComponent->selectRow(rowNumber);	}
	void deselectAllRows()			{	fileListComponent->deselectAllRows();		}
	
	void setResizeEnable(bool enableResize)	{	
		showResizer = enableResize;
		resizer->setVisible(enableResize);	
	}
	bool getResizeEnable()					{	return showResizer;			}
	
	int getLongestWidth();
    //==============================================================================
    /** @internal */
    void resized();
    /** @internal */
    bool keyPressed (const KeyPress& key);
    /** @internal */
    void selectionChanged();
    /** @internal */
    void fileClicked (const File& f, const MouseEvent& e);
    /** @internal */
    void fileDoubleClicked (const File& f);
				
private:
    //==============================================================================
    DirectoryContentsList* fileList;
    FileFilter* directoriesOnlyFilter;
	
    FileChooserMode mode;
    File currentRoot;
    ListenerList <FileBrowserListener> listeners;
	
    FileListComponent* fileListComponent;
	
    TimeSliceThread thread;
	
    void sendListenerChangeMessage();
	
	bool showResizer;
	ResizableCornerComponent* resizer;
	ComponentBoundsConstrainer resizeLimits;
	BasicFileBrowserLookAndFeel *lookAndFeel;

    BasicFileBrowser (const BasicFileBrowser&);
    const BasicFileBrowser& operator= (const BasicFileBrowser&);
};

#endif //_DROWAUDIO_BASICFILEBROWSER__H_