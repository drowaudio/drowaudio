/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/



MusicLibraryTable::MusicLibraryTable()
:	font (12.0f),
	currentLibrary (nullptr),
    dataList (MusicColumns::libraryIdentifier),
	filteredNumRows (dataList.getNumChildren()),
	finishedLoading (true)
{
	// Create our table component and add it to this component..
	addAndMakeVisible (&table);
    table.setModel (this);
    table.setMultipleSelectionEnabled (true);
	table.setColour (ListBox::backgroundColourId, Colour::greyLevel (0.2f));
	table.setHeaderHeight (18);
	table.setRowHeight (16);
	table.getViewport()->setScrollBarThickness (10);

	// give it a border
	table.setColour (ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness (1);

	// Add some MusicColumns to the table header
    for (int i = 1; i < MusicColumns::numColumns; i++)
    {
        table.getHeader().addColumn (MusicColumns::columnNames[i].toString(),
                                     i,
                                     MusicColumns::columnWidths[i],
                                     50,
                                     800,
                                     TableHeaderComponent::defaultFlags);
    }
        
	// we could now change some initial settings..
	table.getHeader().setSortColumnId (MusicColumns::Artist, true); // sort forwards by the ID column

	table.getHeader().setColumnVisible (MusicColumns::LibID, false);
	table.getHeader().setColumnVisible (MusicColumns::ID, false);
	table.getHeader().setColumnVisible (MusicColumns::Rating, false);
	table.getHeader().setColumnVisible (MusicColumns::Location, false);
	table.getHeader().setColumnVisible (MusicColumns::Modified, false);
		
	setFilterText (String::empty);
}

MusicLibraryTable::~MusicLibraryTable()
{
	if (currentLibrary != nullptr)
		currentLibrary->removeListener(this);
}

void MusicLibraryTable::setLibraryToUse (ITunesLibrary* library)
{
	currentLibrary = library;
	
	DBG("ITunesLibrary to use changed");
	filteredDataList = dataList = library->getLibraryTree();
	dataList = library->getLibraryTree();
	library->addListener(this);
}

void MusicLibraryTable::setFilterText (String filterString)
{
    currentFilterText = filterString;
    if (currentLibrary != nullptr)
        currentLibrary->getParserLock().enter();

//    filteredArray.clear();
    
	if (filterString == String::empty)
	{
		filteredDataList = dataList;
		filteredNumRows = filteredDataList.getNumChildren();

//        for (int e = 0; e < dataList.getNumChildren(); e++)
//        {
//            filteredArray.add (dataList.getChild (e));
//        }
	}
	else
	{
		filteredDataList = ValueTree (dataList.getType());
		
		for (int e = 0; e < dataList.getNumChildren(); e++)
		{
			for (int i = 0; i < dataList.getChild (e).getNumProperties(); i++)
			{
				if (dataList.getChild (e)[MusicColumns::columnNames[i]].toString().containsIgnoreCase (filterString))
				{
//                    filteredArray.add (dataList.getChild (e));
					filteredDataList.addChild (dataList.getChild(e).createCopy(), -1, 0);
					
					break;
				}
				
			}
		}
		
		filteredNumRows = filteredDataList.getNumChildren();
	}
	
    if (currentLibrary != nullptr)
        currentLibrary->getParserLock().exit();

	table.getHeader().reSortTable();
	table.updateContent();
}

//==============================================================================
void MusicLibraryTable::libraryChanged (ITunesLibrary* library)
{
	if (library == currentLibrary) 
	{
		filteredDataList = dataList = currentLibrary->getLibraryTree();
		filteredNumRows = filteredDataList.getNumChildren();

		finishedLoading = false;
		
		table.updateContent();
        table.getHeader().reSortTable();
    }
}

void MusicLibraryTable::libraryUpdated (ITunesLibrary* library)
{
	if (library == currentLibrary) 
	{
		filteredNumRows = filteredDataList.getNumChildren();

		table.updateContent();
        table.getHeader().reSortTable();
	}
}

void MusicLibraryTable::libraryFinished (ITunesLibrary* library)
{
	if (library == currentLibrary) 
	{
		filteredNumRows = filteredDataList.getNumChildren();
		finishedLoading = true;

		table.updateContent();
		table.getHeader().reSortTable();
	}
}

//==============================================================================
int MusicLibraryTable::getNumRows()
{
	return filteredNumRows;
}

void MusicLibraryTable::paintRowBackground (Graphics& g, int /*rowNumber*/,
                                            int /*width*/, int /*height*/, bool rowIsSelected)
{
	if (rowIsSelected)
		table.hasKeyboardFocus (true) ? g.fillAll (Colours::darkorange) : g.fillAll (Colour::greyLevel (0.6f));
	else
		g.fillAll (Colour::greyLevel (0.2f));
}

void MusicLibraryTable::paintCell (Graphics& g,
								   int rowNumber,
								   int columnId,
								   int width, int height,
								   bool rowIsSelected)
{
	rowIsSelected ? g.setColour (Colour::greyLevel (0.2f)) : g.setColour (Colour::greyLevel (0.9f));
	g.setFont (font);

    {
        ScopedLock sl (currentLibrary->getParserLock());
        const ValueTree& rowElement (filteredDataList.getChild (rowNumber));
    
        if (rowElement.isValid())
        {
            String text;
            
            if(columnId == MusicColumns::Length)
                text = secondsToTimeLength (rowElement[MusicColumns::columnNames[columnId]].toString().getIntValue());
            else if(columnId == MusicColumns::Added
                    || columnId == MusicColumns::Modified)
                text = Time (int64 (rowElement[MusicColumns::columnNames[columnId]])).formatted ("%d/%m/%Y - %H:%M");
            else
                text = rowElement[MusicColumns::columnNames[columnId]].toString();
            
            g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
        }
    }

	g.setColour (Colour::greyLevel (0.9f).withAlpha (0.2f));
	g.fillRect (width - 1, 0, 1, height);
	g.fillRect (0, height - 1, width, 1);
}

void MusicLibraryTable::sortOrderChanged (int newSortColumnId, bool isForwards)
{
	if (newSortColumnId != 0)
	{
        ScopedLock sl (currentLibrary->getParserLock());
        
		if (newSortColumnId == MusicColumns::Length
			|| newSortColumnId == MusicColumns::BPM
			|| newSortColumnId == MusicColumns::LibID
			|| newSortColumnId == MusicColumns::ID
            || newSortColumnId == MusicColumns::Added
            || newSortColumnId == MusicColumns::Modified)
		{
			ValueTreeComparators::Numerical sorter (MusicColumns::columnNames[newSortColumnId], isForwards);
			filteredDataList.sort (sorter, 0, false);
//            dataList.sort (sorter, 0, false);
		}
		else
        {
//			ValueTreeComparators::Lexicographic sorter (MusicColumns::columnNames[newSortColumnId], isForwards);
			ValueTreeComparators::LexicographicWithBackup sorter (MusicColumns::columnNames[newSortColumnId],
                                                                  MusicColumns::columnNames[MusicColumns::LibID],
                                                                  isForwards);
            
			filteredDataList.sort (sorter, 0, false);
//            dataList.sort (sorter, 0, false);
		}

//        updateFilteredSortOrder();
		table.updateContent();
	}
}

//==========================================================================================
int MusicLibraryTable::getColumnAutoSizeWidth (int columnId)
{
	int widest = 32;

	// find the widest bit of text in this column..
	for (int i = getNumRows(); --i >= 0;)
	{
        {
            ScopedLock sl (currentLibrary->getParserLock());
            const ValueTree& rowElement (filteredDataList.getChild (i));

            if (rowElement.isValid())
            {
                const String text (rowElement[MusicColumns::columnNames[columnId]].toString());
                widest = jmax (widest, font.getStringWidth (text));
            }
        }
	}

	return widest + 8;
}

//==============================================================================
void MusicLibraryTable::resized()
{
	table.setBounds (getLocalBounds());
}

void MusicLibraryTable::focusOfChildComponentChanged (FocusChangeType /*cause*/)
{
	repaint();
}

var MusicLibraryTable::getDragSourceDescription (const SparseSet< int > &currentlySelectedRows)
{
	if(! currentlySelectedRows.isEmpty())
	{
        var itemsArray;
                          
        for (int i = 0; i < currentlySelectedRows.size(); ++i)
        {
            {
                ScopedLock sl (currentLibrary->getParserLock());
                // get child from main tree with same LibID
                const ValueTree& tree (filteredDataList.getChild (currentlySelectedRows[i]));
//                const ValueTree& tree (filteredArray[currentlySelectedRows[i]]);

                ReferenceCountedValueTree::Ptr childTree = new ReferenceCountedValueTree (tree);
                itemsArray.append (childTree.getObject());
            }
        }
            
        return itemsArray;
            
	}
    
	return var::null;
}



