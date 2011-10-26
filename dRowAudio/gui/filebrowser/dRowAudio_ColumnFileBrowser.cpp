/*
 *  dRowAudio_ColumnFileBrowser.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 27/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

ColumnFileBrowserContents::ColumnFileBrowserContents(WildcardFileFilter *filesToDisplay_, Viewport* parentViewport)
    : filesToDisplay (filesToDisplay_),
	  viewport (parentViewport),
	  startingWidth (300)
{
	addMouseListener(this, true);
	
//	lookAndFeel = new BrowserColumnLookAndFeel;
//	setLookAndFeel(lookAndFeel);

	columns.add(new BrowserColumn(filesToDisplay_, 0));
	addAndMakeVisible(columns[0]);
	columns[0]->setSize(startingWidth, 50);
	columns[0]->columnNumber = columns.size()-1;
	columns[0]->addListener(this);
	columns[0]->addChangeListener(this);
	columns[0]->addComponentListener(this);
	activeColumn = 0;
}

ColumnFileBrowserContents::~ColumnFileBrowserContents()
{
	columns.clear();
//	delete lookAndFeel;
}

void ColumnFileBrowserContents::resized()
{
	startingWidth = columns.getLast()->getWidth();

	int width = 0;
	int height = getHeight();
	
	for (int i = 0; i < columns.size(); i++)
	{
		columns[i]->setBounds(width, 0, columns[i]->getWidth(), height);
		width += columns[i]->getWidth();
	}
	
	setSize(width, height);
}

void ColumnFileBrowserContents::selectedFileChanged (const File& file)
{
	currentFilePath = file.getFullPathName();
	
	// if last column clicked add new column
	if (columns[activeColumn] == columns.getLast())
	{
		addColumn (file);
	}
	else        // otherwise remove uneeded columns and change last
	{
		for (int i = 0; i < columns.size(); i++)
		{
			if (columns[activeColumn] == columns[i])
            {
                const int numColumnsToRemove = columns.size() - i - (file.isDirectory() ? 1 : 0);
				removeColumn (numColumnsToRemove);
				
				if (file.isDirectory())
					columns.getLast()->setRoot (file);
			}
		}
		
		resized();
	}
	
	// stick to edges of viewport
	if (getWidth() < viewport->getWidth())
		viewport->setViewPosition (0, 0);
	else if ( file.exists())//.isDirectory() )
		viewport->setViewPosition (viewport->getViewPositionX() - (viewport->getRight() - getRight()), 0);
	else if (getRight() < viewport->getRight())
	{
		int ammountToSubtract = viewport->getRight() - getRight();
//		DBG(String("need to resize by: ")<<ammountToSubtract);
		viewport->setViewPosition( viewport->getViewPositionX() - ammountToSubtract, 0);
	}
	
}

bool ColumnFileBrowserContents::addColumn (const File& rootDirectory)
{
	if (rootDirectory.isDirectory() && rootDirectory.exists())
	{
		columns.add (new BrowserColumn (filesToDisplay, columns.size()));
		addAndMakeVisible (columns.getLast());
		columns.getLast()->setRoot (rootDirectory);
		columns.getLast()->setSize (startingWidth, 50);
		columns.getLast()->addListener (this);
		columns.getLast()->addChangeListener (this);
		columns.getLast()->addComponentListener (this);
		
		resized();
		
		return true;
	}
	
	return false;
}

void ColumnFileBrowserContents::removeColumn(int numColumns)
{
	for (int i = numColumns; i <= 0; --i) 
    {
		columns[i]->removeListener (this);
		columns[i]->removeChangeListener (this);
		columns[i]->removeComponentListener (this);
	}
    
	columns.removeLast (numColumns - 1);
}

void ColumnFileBrowserContents::changeListenerCallback (ChangeBroadcaster* changedComponent)
{
    BrowserColumn* changedColumn = static_cast<BrowserColumn*> (changedComponent);
    
    columns[activeColumn]->getLookAndFeel().setColour (DirectoryContentsDisplayComponent::highlightColourId,
                                                       Colour::greyLevel(0.9f));
    columns[activeColumn]->repaint();
    
    activeColumn = changedColumn->columnNumber;
    
    File tempFile (changedColumn->getHighlightedFile());
    selectedFileChanged (tempFile);
    columns[activeColumn]->getLookAndFeel().setColour (DirectoryContentsDisplayComponent::highlightColourId, 
                                                       Colours::darkorange);
    columns[activeColumn]->repaint();
}

void ColumnFileBrowserContents::componentMovedOrResized (Component &component, bool wasMoved, bool wasResized)
{
    if (wasResized)
        resized();
}

bool ColumnFileBrowserContents::keyPressed (const KeyPress& key)
{
    if (key.isKeyCode (KeyPress::leftKey))
    {
        if (activeColumn != 0)
        {
            FileListComponent* list = dynamic_cast<FileListComponent*> (columns[activeColumn]->getDisplayComponent());
            list->deselectAllRows();
            int newActiveColumn = jmax (0, activeColumn - 1);
            columns[newActiveColumn]->selectionChanged();
            columns[newActiveColumn]->grabKeyboardFocus();
        }
        
        return true;
    }
    else if (key.isKeyCode (KeyPress::rightKey))
    {
        if (columns[activeColumn]->getNumSelectedFiles() == 1
            && columns[activeColumn]->getSelectedFile (0).isDirectory())
        {
            int newActiveColumn = activeColumn + 1;
            addColumn (columns[activeColumn]->getSelectedFile (0));
            
            FileListComponent* list = dynamic_cast<FileListComponent*> (columns[newActiveColumn]->getDisplayComponent());

            if (list->getNumRows() > 0)
            {
                columns[newActiveColumn]->grabKeyboardFocus();
                list->selectRow (0);
            }
        }
        
        return true;
    }
    
    return false;
}

END_JUCE_NAMESPACE