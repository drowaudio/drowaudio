/*
 *  dRowAudio_BasicFileBrowser.h
 *  dRowAudio
 *
 *  Created by David Rowland on 05/11/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_BASICFILEBROWSER_H__
#define __DROWAUDIO_BASICFILEBROWSER_H__

class BasicFileBrowserLookAndFeel : public LookAndFeel,
									public DeletedAtShutdown
{
public:
	//juce_DeclareSingleton (BasicFileBrowserLookAndFeel, false)
	
	BasicFileBrowserLookAndFeel()
	{
		setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2));
		setColour(DirectoryContentsDisplayComponent::highlightColourId, Colour::greyLevel(0.9));
		setColour(DirectoryContentsDisplayComponent::textColourId, Colour::greyLevel(0.9));		
		
		// scrollbars
		setColour(ScrollBar::backgroundColourId, Colour::greyLevel(0.5));
		setColour(ScrollBar::thumbColourId, Colour::greyLevel(0.8));
		setColour(ScrollBar::trackColourId, Colour::greyLevel(0.3));		
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
	
	//============================================================
	void drawScrollbarButton (Graphics& g,
                              ScrollBar& scrollbar,
                              int width, int height,
                              int buttonDirection,
                              bool /*isScrollbarVertical*/,
                              bool /*isMouseOverButton*/,
                              bool isButtonDown)
	{
		Path p;
		
		if (buttonDirection == 0)
			p.addTriangle (width * 0.5f, height * 0.2f,
						   width * 0.1f, height * 0.7f,
						   width * 0.9f, height * 0.7f);
		else if (buttonDirection == 1)
			p.addTriangle (width * 0.8f, height * 0.5f,
						   width * 0.3f, height * 0.1f,
						   width * 0.3f, height * 0.9f);
		else if (buttonDirection == 2)
			p.addTriangle (width * 0.5f, height * 0.8f,
						   width * 0.1f, height * 0.3f,
						   width * 0.9f, height * 0.3f);
		else if (buttonDirection == 3)
			p.addTriangle (width * 0.2f, height * 0.5f,
						   width * 0.7f, height * 0.1f,
						   width * 0.7f, height * 0.9f);
		
		if (isButtonDown)
			g.setColour (scrollbar.findColour (ScrollBar::thumbColourId).contrasting (0.2f));
		else
			g.setColour (scrollbar.findColour (ScrollBar::thumbColourId));
		
		g.fillPath (p);
		
		g.setColour (Colour (0x80000000));
		g.strokePath (p, PathStrokeType (0.5f));
	}
	
	void drawScrollbar (Graphics& g,
                        ScrollBar& scrollbar,
                        int x, int y,
                        int width, int height,
                        bool isScrollbarVertical,
                        int thumbStartPosition,
                        int thumbSize,
                        bool /*isMouseOver*/,
                        bool /*isMouseDown*/)
	{
		g.fillAll (scrollbar.findColour (ScrollBar::backgroundColourId));
		
		Path slotPath, thumbPath;
		
		float gx1 = 0.0f, gy1 = 0.0f, gx2 = 0.0f, gy2 = 0.0f;
		
		if (isScrollbarVertical)
		{
			slotPath.addRoundedRectangle (x,
										  y,
										  width,
										  height,
										  width * 0.5f);
			
			if (thumbSize > 0)
				thumbPath.addRoundedRectangle (x,
											   thumbStartPosition,
											   width,
											   thumbSize,
											   (width) * 0.5f);
			gx1 = (float) x;
			gx2 = x + width * 0.7f;
		}
		else
		{
			slotPath.addRoundedRectangle (x,
										  y,
										  width,
										  height,
										  (height) * 0.5f);
			
			if (thumbSize > 0)
				thumbPath.addRoundedRectangle (thumbStartPosition,
											   y,
											   thumbSize,
											   height,
											   (height) * 0.5f);
			gy1 = (float) y;
			gy2 = y + height * 0.7f;
		}
		
		const Colour thumbColour (scrollbar.findColour (ScrollBar::thumbColourId));
		Colour trackColour1, trackColour2;
		
		trackColour1 = scrollbar.findColour (ScrollBar::trackColourId);
		trackColour2 = trackColour1.overlaidWith (Colour (0x19000000));
		
		g.setGradientFill (ColourGradient (trackColour1, gx1, gy1,
										   trackColour2, gx2, gy2, false));
		g.fillPath (slotPath);
		
		if (isScrollbarVertical)
		{
			gx1 = x + width * 0.6f;
			gx2 = (float) x + width;
		}
		else
		{
			gy1 = y + height * 0.6f;
			gy2 = (float) y + height;
		}
		
		g.setGradientFill (ColourGradient (Colours::transparentBlack,gx1, gy1,
										   Colour (0x19000000), gx2, gy2, false));
		g.fillPath (slotPath);
		
		g.setColour (thumbColour);
		g.fillPath (thumbPath);
		
		//    g.setGradientFill (ColourGradient (Colour (0x10000000), gx1, gy1,
		//									   Colours::transparentBlack, gx2, gy2, false));
		//	
		//    g.saveState();
		//	
		//    if (isScrollbarVertical)
		//        g.reduceClipRegion (x + width / 2, y, width, height);
		//    else
		//        g.reduceClipRegion (x, y + height / 2, width, height);
		//	
		//    g.fillPath (thumbPath);
		//    g.restoreState();
		
		//    g.setColour (Colour (0x4c000000));
		//    g.setColour (thumbColour.brighter(0.5));
		//    g.strokePath (thumbPath, PathStrokeType (1.0f));
	}
	
	//============================================================
    void drawCornerResizer (Graphics& g,
                            int w, int h,
                            bool isMouseOver,
                            bool isMouseDragging)
	{
		const float lineThickness = 1.0f;//jmin (w, h) * 0.075f;
		const float xGap = w / 3.0f;
		const float yGap = h * 0.25;
		
		g.setColour (findColour (ScrollBar::backgroundColourId));
		g.fillAll();
		
		g.setColour (findColour (ScrollBar::thumbColourId));
		g.drawLine (xGap + lineThickness * 0.5, yGap, xGap + lineThickness * 0.5, h - yGap, lineThickness);
		g.drawLine ((2 * xGap) - lineThickness * 0.5, yGap, (2 * xGap) - lineThickness * 0.5, h - yGap, lineThickness);
	}
};

//==================================================================================
class  BasicFileBrowser		:	public Component,
								private FileBrowserListener,
                                private FileFilter
{
public:
    //==============================================================================
    /** Various options for the browser.
     
        A combination of these is passed into the FileBrowserComponent constructor.
     */
    enum FileChooserFlags
    {
        openMode                = 1,    /**< specifies that the component should allow the user to
                                         choose an existing file with the intention of opening it. */
        saveMode                = 2,    /**< specifies that the component should allow the user to specify
                                         the name of a file that will be used to save something. */
        canSelectFiles          = 4,    /**< specifies that the user can select files (can be used in
                                         conjunction with canSelectDirectories). */
        canSelectDirectories    = 8,    /**< specifies that the user can select directories (can be used in
                                         conjuction with canSelectFiles). */
        canSelectMultipleItems  = 16,   /**< specifies that the user can select multiple items. */
        useTreeView             = 32,   /**< specifies that a tree-view should be shown instead of a file list. */
        filenameBoxIsReadOnly   = 64    /**< specifies that the user can't type directly into the filename box. */
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
    BasicFileBrowser (int flags,
                      const File& initialFileOrDirectory,
                      const FileFilter* fileFilter);
	
    /** Destructor. */
    ~BasicFileBrowser();
	
    //==============================================================================
    /** Returns the number of files that the user has got selected.
     If multiple select isn't active, this will only be 0 or 1. To get the complete
     list of files they've chosen, pass an index to getCurrentFile().
     */
    int getNumSelectedFiles() const noexcept;
    
    /** Returns one of the files that the user has chosen.
     If the box has multi-select enabled, the index lets you specify which of the files
     to get - see getNumSelectedFiles() to find out how many files were chosen.
     @see getHighlightedFile
     */
    File getSelectedFile (int index) const noexcept;
    
    /** Deselects any files that are currently selected.
     */
    void deselectAllFiles();
    
    /** Returns true if the currently selected file(s) are usable.
     
     This can be used to decide whether the user can press "ok" for the
     current file. What it does depends on the mode, so for example in an "open"
     mode, this only returns true if a file has been selected and if it exists.
     In a "save" mode, a non-existent file would also be valid.
     */
    bool currentFileIsValid() const;
    
    /** This returns the last item in the view that the user has highlighted.
     This may be different from getCurrentFile(), which returns the value
     that is shown in the filename box, and if there are multiple selections,
     this will only return one of them.
     @see getSelectedFile
     */
    File getHighlightedFile() const noexcept;
    
    //==============================================================================
    /** Returns the directory whose contents are currently being shown in the listbox. */
    const File& getRoot() const;
    
    /** Changes the directory that's being shown in the listbox. */
    void setRoot (const File& newRootDirectory);
    
    /** Refreshes the directory that's currently being listed. */
    void refresh();
    
    /** Changes the filter that's being used to sift the files. */
    void setFileFilter (const FileFilter* newFileFilter);
    
    /** Returns a verb to describe what should happen when the file is accepted.
     
     E.g. if browsing in "load file" mode, this will be "Open", if in "save file"
     mode, it'll be "Save", etc.
     */
    virtual String getActionVerb() const;
    
    /** Returns true if the saveMode flag was set when this component was created.
     */
    bool isSaveMode() const noexcept;
    
    //==============================================================================
    /** Adds a listener to be told when the user selects and clicks on files.
     @see removeListener
     */
    void addListener (FileBrowserListener* listener);
    
    /** Removes a listener.
     @see addListener
     */
    void removeListener (FileBrowserListener* listener);
        
	//==============================================================================
	/** Enables the column resizer.
     */
	void setResizeEnable(bool enableResize)
    {	
		showResizer = enableResize;
	}
    
    /** Returns true if the resizer is enabled.
     */
	bool getResizeEnabled()					{	return showResizer;			}
	
    /** Returns the width for the longest item in the list.
     */
	int getLongestWidth();
    
    //==============================================================================
    /** @internal */
    void resized();
    /** @internal */
    bool keyPressed (const KeyPress& key);
    /** @internal */
    void mouseDoubleClick (const MouseEvent &e);
    /** @internal */
    void selectionChanged();
    /** @internal */
    void fileClicked (const File& f, const MouseEvent& e);
    /** @internal */
    void fileDoubleClicked (const File& f);
    /** @internal */
    void browserRootChanged (const File&);
    /** @internal */
    bool isFileSuitable (const File&) const;
    /** @internal */
    bool isDirectorySuitable (const File&) const;

    /** @internal */
    DirectoryContentsDisplayComponent* getDisplayComponent() const noexcept;
    
private:
    //==============================================================================
    ScopedPointer <DirectoryContentsList> fileList;
    const FileFilter* fileFilter;
    
    int flags;
    File currentRoot;
    Array<File> chosenFiles;
    ListenerList <FileBrowserListener> listeners;
    
    ScopedPointer<DirectoryContentsDisplayComponent> fileListComponent;
    
    TimeSliceThread thread;
    
    void sendListenerChangeMessage();
    bool isFileOrDirSuitable (const File& f) const;
	
	bool showResizer;
	ScopedPointer<ResizableCornerComponent> resizer;
	ComponentBoundsConstrainer resizeLimits;
	ScopedPointer<BasicFileBrowserLookAndFeel> lookAndFeel;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicFileBrowser);
};

#endif //__DROWAUDIO_BASICFILEBROWSER_H__