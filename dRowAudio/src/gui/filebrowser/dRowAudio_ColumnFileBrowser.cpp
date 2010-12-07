/*
 *  dRowAudio_ColumnFileBrowser.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 27/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "../../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_ColumnFileBrowser.h"

ColumnFileBrowserContents::ColumnFileBrowserContents(WildcardFileFilter *filesToDisplay_, Viewport* parentViewport)
:	filesToDisplay(filesToDisplay_),
	viewport(parentViewport),
	startingWidth(300)
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

void ColumnFileBrowserContents::selectedFileChanged (const File &file)
{
	currentFilePath = file.getFullPathName();
	
	// if last column clicked add new column
	if (columns[activeColumn] == columns.getLast())
	{
		addColumn(file);
	}
	// otherwise remove uneeded columns and change last
	else
	{
		for (int i = 0; i < columns.size(); i++)
		{
			if (columns[activeColumn] == columns[i]) {
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
	else if ( file.exists())//.isDirectory() )
		viewport->setViewPosition(viewport->getViewPositionX() - (viewport->getRight() - getRight()), 0);
	else if (getRight() < viewport->getRight())
	{
		int ammountToSubtract = viewport->getRight() - getRight();
//		DBG(String("need to resize by: ")<<ammountToSubtract);
		viewport->setViewPosition( viewport->getViewPositionX() - ammountToSubtract, 0);
	}
	
}

bool ColumnFileBrowserContents::addColumn(const File &file)
{
	if (file.isDirectory() && file.exists())
	{
		columns.add(new BrowserColumn(filesToDisplay, columns.size()));
		addAndMakeVisible(columns.getLast());
		columns.getLast()->setRoot(file);
		columns.getLast()->setSize(startingWidth, 50);
		columns.getLast()->addListener(this);
		columns.getLast()->addChangeListener(this);
		columns.getLast()->addComponentListener(this);
		
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
		columns[i]->removeComponentListener(this);
	}
	columns.removeLast(noColumns-1);
}

END_DROWAUDIO_NAMESPACE