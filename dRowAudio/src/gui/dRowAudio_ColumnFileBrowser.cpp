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
	viewport(parentViewport)
{
	lookAndFeel = new BrowserColumnLookAndFeel;
	setLookAndFeel(lookAndFeel);
	
	columns.add(new BrowserColumn(filesToDisplay_));
	addAndMakeVisible(columns[0]);
	columns[0]->setSize(300, 50);
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
	int width = 0;
	int height = getHeight();
	
	for (int i = 0; i < columns.size(); i++)
	{
		columns[i]->setBounds(width, 0, columns[i]->getWidth(), height);
		width += columns[i]->getWidth();
	}
	
	setSize(width, height);
	totalWidth = width;
}

void ColumnFileBrowserContents::fileClicked (const File &file, const MouseEvent &e)
{
	DBG( String("File: ")<<file.getFullPathName() );

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
				removeColumn(columns.size()-i-1);
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
		columns.add(new BrowserColumn(filesToDisplay));
		addAndMakeVisible(columns.getLast());
		columns.getLast()->setRoot(file);
		columns.getLast()->setSize(300, 50);
		columns.getLast()->addListener(this);
		columns.getLast()->addChangeListener(this);

		int column = columns.size() - 1;
		columns.getLast()->columnNumber = column;
		
		resized();
		
		return true;
	}
	
	return false;
}

void ColumnFileBrowserContents::removeColumn(int noColumns)
{
	for (int i = noColumns; i < 0; --i) {
		columns[i]->removeListener(this);
		columns[i]->removeChangeListener(this);
	}
	columns.removeLast(noColumns-1);
}