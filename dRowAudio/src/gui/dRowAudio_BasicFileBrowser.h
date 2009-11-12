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

#include <juce/juce.h>

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
	 @param previewComp              an optional preview component that will be used to
	 show previews of files that the user selects
	 @param useTreeView              if this is false, the files are shown in a list; if true,
	 they are shown in a treeview
	 @param filenameTextBoxIsReadOnly    if true, the user won't be allowed to type their own
	 text into the filename box.
	 */
    BasicFileBrowser (FileChooserMode browserMode,
                          const File& initialFileOrDirectory,
                          const FileFilter* fileFilter,
                          FilePreviewComponent* previewComp,
                          const bool useTreeView = false,
                          const bool filenameTextBoxIsReadOnly = false);
	
    /** Destructor. */
    ~BasicFileBrowser();
	
    //==============================================================================
    /**
	 */
    const File getCurrentFile() const throw();
	
	const File getHighlightedFile(bool onlyInterestedFiles =false) const throw();

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

	//==============================================================================

	int getNumRows()
	{	
		if (!isTree)
			((FileListComponent*)fileListComponent)->getNumRows();
	}
	void selectRow(int rowNumber)
	{	
		if (!isTree)
			((FileListComponent*)fileListComponent)->selectRow(rowNumber);
	}
	void deselectAllRows()
	{	
		if (!isTree)
			((FileListComponent*)fileListComponent)->deselectAllRows();		
	}
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
	
    /** @internal */
    FilePreviewComponent* getPreviewComponent() const throw();
		
protected:
    virtual const BitArray getRoots (StringArray& rootNames, StringArray& rootPaths);
	
private:
    //==============================================================================
    DirectoryContentsList* fileList;
    FileFilter* directoriesOnlyFilter;
	
    FileChooserMode mode;
    File currentRoot;
    SortedSet <void*> listeners;
	
    DirectoryContentsDisplayComponent* fileListComponent;
    FilePreviewComponent* previewComp;
	
    TimeSliceThread thread;
	
    void sendListenerChangeMessage();
	
	bool isTree;
	
    BasicFileBrowser (const BasicFileBrowser&);
    const BasicFileBrowser& operator= (const BasicFileBrowser&);
};


#endif //_DROWAUDIO_BASICFILEBROWSER__H_