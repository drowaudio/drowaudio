/*
 *  dRowAudio_ColumnFileBrowser.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 27/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_ColumnFileBrowser.h"

ColumnFileBrowserContents::ColumnFileBrowserContents(WildcardFileFilter *filesToDisplay_, Viewport* parentViewport)
:	filesToDisplay(filesToDisplay_),
	viewport(parentViewport),
	startingWidth(300)
{
	lookAndFeel = new BrowserColumnLookAndFeel;
	setLookAndFeel(lookAndFeel);

	addMouseListener(this, true);

	columns.add(new BrowserColumn(this, filesToDisplay_, 0));
	addAndMakeVisible(columns[0]);
	columns[0]->setSize(startingWidth, 50);
	columns[0]->columnNumber = 0;
	columns[0]->addListener(this);
	columns[0]->addChangeListener(this);
}

ColumnFileBrowserContents::~ColumnFileBrowserContents()
{
	columns.clear();
	delete lookAndFeel;
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

void ColumnFileBrowserContents::selectionChanged()
{
//	columns[1]->setRoot(columns[0]->getHighlightedFile());
}

void ColumnFileBrowserContents::fileClicked (const File &file, const MouseEvent &e)
{
	currentFilePath = file.getFullPathName();
	DBG( String("File: ")<<currentFilePath );

	// if last column clicked add new column
	if (columns.getLast()->active)
	{
		addColumn(file);
	}
	// otherwise remove uneeded columns and change last
	else
	{
		for (int i = 0; i < columns.size(); i++)
		{
			if (columns[i]->active) {
				removeColumn(columns.size()-i-(file.isDirectory()?1:0));

				if (file.isDirectory())
					columns.getLast()->setRoot(file);
			}
		}
		
		resized();
	}

	// stick to edges of viewport
	if (getWidth() < viewport->getWidth())
		viewport->setViewPosition(0, 0);
	else if ( file.isDirectory() )
		viewport->setViewPosition(viewport->getViewPositionX() - (viewport->getRight() - getRight()), 0);
	else if (getRight() < viewport->getRight())
	{
		int ammountToSubtract = viewport->getRight() - getRight();
		DBG(String("need to resize by: ")<<ammountToSubtract);
		viewport->setViewPosition( viewport->getViewPositionX() - ammountToSubtract, 0);
	}
	
}

bool ColumnFileBrowserContents::addColumn(const File &file)
{
	if (file.isDirectory())
	{
		columns.add(new BrowserColumn(this, filesToDisplay, columns.size() - 1));
		addAndMakeVisible(columns.getLast());
		columns.getLast()->setRoot(file);
		columns.getLast()->setSize(startingWidth, 50);
		columns.getLast()->addListener(this);
		columns.getLast()->addChangeListener(this);
		
		resized();
		
		return true;
	}
	
	return false;
}

void ColumnFileBrowserContents::removeColumn(int noColumns)
{
	for (int i = noColumns; i <= 0; --i) {
		columns[i]->removeListener(this);
		columns[i]->removeChangeListener(this);
	}
	columns.removeLast(noColumns-1);
}