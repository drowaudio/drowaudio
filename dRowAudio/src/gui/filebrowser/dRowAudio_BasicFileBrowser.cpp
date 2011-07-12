/*
 *  dRowAudio_BasicFileBrowser.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 05/11/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "../../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

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
                                    const FileFilter* fileFilter)
:	directoriesOnlyFilter (0),
	mode (mode_),
	thread ("Juce BasicFileBrowser"),
	showResizer(true)
{
	lookAndFeel = new BasicFileBrowserLookAndFeel;
	setLookAndFeel(lookAndFeel);
//	setLookAndFeel(BasicFileBrowserLookAndFeel::getInstance());
		
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
	
	fileListComponent = new FileListComponent (*fileList);
	fileListComponent->setOutlineThickness (0);
	fileListComponent->getViewport()->setScrollBarThickness(10);
	fileListComponent->setRowHeight(18);
	addAndMakeVisible (fileListComponent);
	
    fileListComponent->addListener (this);
	fileListComponent->getViewport()->getVerticalScrollBar()->setAutoHide(false);
		
	resizer = new ResizableCornerComponent(this, &resizeLimits);
	addAndMakeVisible(resizer);
	resizer->setMouseCursor (MouseCursor::LeftRightResizeCursor);
	
//	resizer->addMouseListener(this, true);

    setRoot (currentRoot);
		
    thread.startThread (4);
}

BasicFileBrowser::~BasicFileBrowser()
{
    deleteAllChildren();
	
    deleteAndZero (fileList);
    delete directoriesOnlyFilter;
	delete lookAndFeel;
	
    thread.stopThread (10000);
}

//==============================================================================
void BasicFileBrowser::addListener (FileBrowserListener* const newListener) throw()
{
    listeners.add (newListener);
}

void BasicFileBrowser::removeListener (FileBrowserListener* const listener) throw()
{
    listeners.remove (listener);
}

void BasicFileBrowser::mouseDoubleClick (const MouseEvent &e)
{
	if (resizer->contains(Point<int>(e.x, e.y)))	{
		
		setSize(jmax(getLongestWidth(), resizeLimits.getMinimumWidth()), 800);
	}
}

//==============================================================================
const File BasicFileBrowser::getCurrentFile() const throw()
{
    return File::nonexistent;
}

const File BasicFileBrowser::getHighlightedFile() const throw()
{
	return fileListComponent->getSelectedFile();
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

int BasicFileBrowser::getLongestWidth()
{
	const int noFiles = fileList->getNumFiles();
	int stringWidth = 0;
	
	Font temp( fileListComponent->getRowHeight() * 0.7f);
	for (int i = 0; i < noFiles; i++)
	{
		int itemWidth = temp.getStringWidth(fileList->getFile(i).getFileName());
		if (itemWidth >	stringWidth)
			stringWidth = itemWidth;
	}
	stringWidth += (2 * fileListComponent->getRowHeight()) + 30;
	
	return stringWidth;
}
//==============================================================================
void BasicFileBrowser::resized()
{
	const int height = getHeight();
	const int width = getWidth();
		
    fileListComponent->setBounds (0, 0, width, height);
	ScrollBar *bar = fileListComponent->getVerticalScrollBar();
	const int size = bar->getWidth() * 1.5;

	if (showResizer)
	{
		bar->setTransform(AffineTransform::scale(1, (height - (float)size) / height));
		
		resizeLimits.setSizeLimits (150, height, 800, height);
		resizer->setBounds(width - size*(2.0/3.0), height - size, size*(2.0/3.0), size);
	}
	else {
		bar->setTransform(AffineTransform::identity);
	}

}

//==============================================================================
void BasicFileBrowser::sendListenerChangeMessage()
{
    Component::BailOutChecker checker (this);
	
    jassert (! checker.shouldBailOut());
	
    listeners.callChecked (checker, &FileBrowserListener::selectionChanged);
}

void BasicFileBrowser::selectionChanged()
{
    sendListenerChangeMessage();
}

void BasicFileBrowser::fileClicked (const File& f, const MouseEvent& e)
{
    Component::BailOutChecker checker (this);
    listeners.callChecked (checker, &FileBrowserListener::fileClicked, f, e);
}

void BasicFileBrowser::fileDoubleClicked (const File& f)
{
    if (f.isDirectory())
    {
        setRoot (f);
    }
    else
    {
        Component::BailOutChecker checker (this);
        listeners.callChecked (checker, &FileBrowserListener::fileDoubleClicked, f);
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

END_DROWAUDIO_NAMESPACE