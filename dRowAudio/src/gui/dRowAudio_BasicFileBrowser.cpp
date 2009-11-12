/*
 *  dRowAudio_BasicFileBrowser.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 05/11/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_BasicFileBrowser.h"

//==============================================================================
class DirectoriesOnlyFilter    : public FileFilter
{
public:
    DirectoriesOnlyFilter() : FileFilter (String::empty) {}
	
    bool isFileSuitable (const File&) const         { return false; }
    bool isDirectorySuitable (const File&) const    { return true; }
};


//==============================================================================
BasicFileBrowser::BasicFileBrowser (FileChooserMode mode_,
                                    const File& initialFileOrDirectory,
                                    const FileFilter* fileFilter,
                                    FilePreviewComponent* previewComp_,
                                    const bool useTreeView,
                                    const bool filenameTextBoxIsReadOnly)
: directoriesOnlyFilter (0),
mode (mode_),
listeners (2),
previewComp (previewComp_),
thread ("Juce FileBrowser"),
isTree(useTreeView)
{
    String filename;
	
    if (initialFileOrDirectory == File::nonexistent)
    {
        currentRoot = File::getCurrentWorkingDirectory();
    }
    else if (initialFileOrDirectory.isDirectory())
    {
        currentRoot = initialFileOrDirectory;
    }
    else
    {
        currentRoot = initialFileOrDirectory.getParentDirectory();
        filename = initialFileOrDirectory.getFileName();
    }
	
    if (mode_ == chooseDirectoryMode)
        fileFilter = directoriesOnlyFilter = new DirectoriesOnlyFilter();
	
    fileList = new DirectoryContentsList (fileFilter, thread);
	
    if (useTreeView)
    {
        FileTreeComponent* const tree = new FileTreeComponent (*fileList);
        addAndMakeVisible (tree);
        fileListComponent = tree;
    }
    else
    {
        FileListComponent* const list = new FileListComponent (*fileList);
        list->setOutlineThickness (1);
        addAndMakeVisible (list);
        fileListComponent = list;
    }
	
    fileListComponent->addListener (this);
	((FileListComponent*)fileListComponent)->getViewport()->getVerticalScrollBar()->setAutoHide(false);
	
    StringArray rootNames, rootPaths;
    const BitArray separators (getRoots (rootNames, rootPaths));
	
	
    if (previewComp != 0)
        addAndMakeVisible (previewComp);
	
    setRoot (currentRoot);
		
    thread.startThread (4);
}

BasicFileBrowser::~BasicFileBrowser()
{
    if (previewComp != 0)
        removeChildComponent (previewComp);
	
    deleteAllChildren();
	
    deleteAndZero (fileList);
    delete directoriesOnlyFilter;
	
    thread.stopThread (10000);
}

//==============================================================================
void BasicFileBrowser::addListener (FileBrowserListener* const newListener) throw()
{
    jassert (newListener != 0)
	
    if (newListener != 0)
        listeners.add (newListener);
}

void BasicFileBrowser::removeListener (FileBrowserListener* const listener) throw()
{
    listeners.removeValue (listener);
}

//==============================================================================
const File BasicFileBrowser::getCurrentFile() const throw()
{
    return File::nonexistent;
}

const File BasicFileBrowser::getHighlightedFile(bool onlyInterestedFiles) const throw()
{
    const File selected (fileListComponent->getSelectedFile());

    if (!onlyInterestedFiles)
		return fileListComponent->getSelectedFile();
    else
    {
        if ((mode == chooseDirectoryMode && selected.isDirectory())
			|| selected.existsAsFile())
        {
			return fileListComponent->getSelectedFile();
        }
    }
	
    return File::nonexistent;
} 

bool BasicFileBrowser::currentFileIsValid() const
{
    if (mode == saveFileMode)
        return ! getCurrentFile().isDirectory();
    else if (mode == loadFileMode)
        return getCurrentFile().existsAsFile();
    else if (mode == chooseDirectoryMode)
        return getCurrentFile().isDirectory();
	
    jassertfalse
    return false;
}

//==============================================================================
const File BasicFileBrowser::getRoot() const
{
    return currentRoot;
}

void BasicFileBrowser::setRoot (const File& newRootDirectory)
{
    if (currentRoot != newRootDirectory)
    {
        fileListComponent->scrollToTop();
		
		
        String path (newRootDirectory.getFullPathName());
		
        if (path.isEmpty())
            path += File::separator;
		
        StringArray rootNames, rootPaths;
        getRoots (rootNames, rootPaths);
    }
	
    currentRoot = newRootDirectory;
    fileList->setDirectory (currentRoot, true, true);
	
    String currentRootName (currentRoot.getFullPathName());
    if (currentRootName.isEmpty())
        currentRootName += File::separator;
}

void BasicFileBrowser::refresh()
{
    fileList->refresh();
}

const String BasicFileBrowser::getActionVerb() const
{
    return (mode == chooseDirectoryMode) ? TRANS("Choose")
	: ((mode == saveFileMode) ? TRANS("Save") : TRANS("Open"));
}

FilePreviewComponent* BasicFileBrowser::getPreviewComponent() const throw()
{
    return previewComp;
}

//==============================================================================
void BasicFileBrowser::resized()
{
//    getLookAndFeel()
//	.layoutFileBrowserComponent (*this, fileListComponent,
//								 previewComp, currentPathBox,
//								 filenameBox, goUpButton);
//	fileListComponent->setBounds(0, 0, getWidth(), getHeight());
	Component* const listAsComp = dynamic_cast <Component*> (fileListComponent);
    listAsComp->setBounds (0, 0, getWidth(), getHeight());
}

//==============================================================================
void BasicFileBrowser::sendListenerChangeMessage()
{
    ComponentDeletionWatcher deletionWatcher (this);
	
    if (previewComp != 0)
        previewComp->selectedFileChanged (getCurrentFile());
	
    jassert (! deletionWatcher.hasBeenDeleted());
	
    for (int i = listeners.size(); --i >= 0;)
    {
        ((FileBrowserListener*) listeners.getUnchecked (i))->selectionChanged();
		
        if (deletionWatcher.hasBeenDeleted())
            return;
		
        i = jmin (i, listeners.size() - 1);
    }
}

void BasicFileBrowser::selectionChanged()
{
    sendListenerChangeMessage();
}

void BasicFileBrowser::fileClicked (const File& f, const MouseEvent& e)
{
    ComponentDeletionWatcher deletionWatcher (this);
	
    for (int i = listeners.size(); --i >= 0;)
    {
        ((FileBrowserListener*) listeners.getUnchecked (i))->fileClicked (f, e);
		
        if (deletionWatcher.hasBeenDeleted())
            return;
		
        i = jmin (i, listeners.size() - 1);
    }
}

void BasicFileBrowser::fileDoubleClicked (const File& f)
{
    if (f.isDirectory())
    {
        setRoot (f);
    }
    else
    {
        ComponentDeletionWatcher deletionWatcher (this);
		
        for (int i = listeners.size(); --i >= 0;)
        {
            ((FileBrowserListener*) listeners.getUnchecked (i))->fileDoubleClicked (f);
			
            if (deletionWatcher.hasBeenDeleted())
                return;
			
            i = jmin (i, listeners.size() - 1);
        }
    }
}

bool BasicFileBrowser::keyPressed (const KeyPress& key)
{
#if JUCE_LINUX || JUCE_WINDOWS
    if (key.getModifiers().isCommandDown()
		&& (key.getKeyCode() == 'H' || key.getKeyCode() == 'h'))
    {
        fileList->setIgnoresHiddenFiles (! fileList->ignoresHiddenFiles());
        fileList->refresh();
        return true;
    }
#endif
	
    return false;
}

//==============================================================================

const BitArray BasicFileBrowser::getRoots (StringArray& rootNames, StringArray& rootPaths)
{
    BitArray separators;
	
#if JUCE_WINDOWS
    OwnedArray<File> roots;
    File::findFileSystemRoots (roots);
    rootPaths.clear();
	
    for (int i = 0; i < roots.size(); ++i)
    {
        const File* const drive = roots.getUnchecked(i);
		
        String name (drive->getFullPathName());
        rootPaths.add (name);
		
        if (drive->isOnHardDisk())
        {
            String volume (drive->getVolumeLabel());
			
            if (volume.isEmpty())
                volume = TRANS("Hard Drive");
			
            name << " [" << drive->getVolumeLabel() << ']';
        }
        else if (drive->isOnCDRomDrive())
        {
            name << TRANS(" [CD/DVD drive]");
        }
		
        rootNames.add (name);
    }
	
    separators.setBit (rootPaths.size());
	
    rootPaths.add (File::getSpecialLocation (File::userDocumentsDirectory).getFullPathName());
    rootNames.add ("Documents");
    rootPaths.add (File::getSpecialLocation (File::userDesktopDirectory).getFullPathName());
    rootNames.add ("Desktop");
#endif
	
#if JUCE_MAC
    rootPaths.add (File::getSpecialLocation (File::userHomeDirectory).getFullPathName());
    rootNames.add ("Home folder");
    rootPaths.add (File::getSpecialLocation (File::userDocumentsDirectory).getFullPathName());
    rootNames.add ("Documents");
    rootPaths.add (File::getSpecialLocation (File::userDesktopDirectory).getFullPathName());
    rootNames.add ("Desktop");
	
    separators.setBit (rootPaths.size());
	
    OwnedArray <File> volumes;
    File vol ("/Volumes");
    vol.findChildFiles (volumes, File::findDirectories, false);
	
    for (int i = 0; i < volumes.size(); ++i)
    {
        const File* const volume = volumes.getUnchecked(i);
		
        if (volume->isDirectory() && ! volume->getFileName().startsWithChar (T('.')))
        {
            rootPaths.add (volume->getFullPathName());
            rootNames.add (volume->getFileName());
        }
    }
#endif
	
#if JUCE_LINUX
    rootPaths.add ("/");
    rootNames.add ("/");
    rootPaths.add (File::getSpecialLocation (File::userHomeDirectory).getFullPathName());
    rootNames.add ("Home folder");
    rootPaths.add (File::getSpecialLocation (File::userDesktopDirectory).getFullPathName());
    rootNames.add ("Desktop");
#endif
	
    return separators;
}