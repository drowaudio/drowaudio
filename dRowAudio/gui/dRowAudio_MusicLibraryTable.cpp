/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-9 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

MusicLibraryTable::MusicLibraryTable()
:	font (12.0f),
	currentLibrary (nullptr),
    dataList (Columns::libraryIdentifier),
	filteredNumRows (dataList.getNumChildren()),
	finishedLoading (true)
{
	// Create our table component and add it to this component..
	addAndMakeVisible (&table);
    table.setModel (this);
    table.setMultipleSelectionEnabled (true);
	table.setColour (ListBox::backgroundColourId, Colour::greyLevel(0.2));
	table.setHeaderHeight (18);
	table.setRowHeight (16);
	table.getViewport()->setScrollBarThickness (10);

	// give it a border
	table.setColour (ListBox::outlineColourId, Colours::grey);
	table.setOutlineThickness (1);

	// Add some columns to the table header
    for (int i = 1; i < Columns::numColumns; i++)
    {
        table.getHeader().addColumn (Columns::columnNames[i].toString(),
                                     i,
                                     Columns::columnWidths[i],
                                     50,
                                     800,
                                     TableHeaderComponent::defaultFlags);
    }
        
	// we could now change some initial settings..
	table.getHeader().setSortColumnId (Columns::Artist, true); // sort forwards by the ID column

	table.getHeader().setColumnVisible (Columns::LibID, false);
	table.getHeader().setColumnVisible (Columns::ID, false);
	table.getHeader().setColumnVisible (Columns::Rating, false);
	table.getHeader().setColumnVisible (Columns::Location, false);
	table.getHeader().setColumnVisible (Columns::Modified, false);
		
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
        DBG("dataList.getNumChildren(): "<<dataList.getNumChildren());
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
				if (dataList.getChild (e)[Columns::columnNames[i]].toString().containsIgnoreCase (filterString))
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
		DBG("library changed");
		filteredDataList = dataList = currentLibrary->getLibraryTree();
		filteredNumRows = filteredDataList.getNumChildren();

//        dataList = currentLibrary->getLibraryTree();
//        setFilterText (currentFilterText);
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
		DBG("num finished: " << filteredNumRows);//.getNumChildren());
//        setFilterText (currentFilterText);
//        DBG ("num finished: " << getNumRows());

		table.updateContent();
        table.getHeader().reSortTable();
	}
}

void MusicLibraryTable::libraryFinished (ITunesLibrary* library)
{
	if (library == currentLibrary) 
	{
		filteredNumRows = dataList.getNumChildren();
		finishedLoading = true;
		DBG ("all finished: " << filteredNumRows);
//        setFilterText (currentFilterText);

		table.updateContent();
		table.getHeader().reSortTable();

		DBG ("finished loading MusicLibraryTable");
	}
}

//==============================================================================
int MusicLibraryTable::getNumRows()
{
	return filteredNumRows;
}

void MusicLibraryTable::paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		table.hasKeyboardFocus (true) ? g.fillAll (Colours::darkorange) : g.fillAll (Colour::greyLevel (0.6));
	else
		g.fillAll (Colour::greyLevel (0.2));
}

void MusicLibraryTable::paintCell (Graphics& g,
								   int rowNumber,
								   int columnId,
								   int width, int height,
								   bool rowIsSelected)
{
	rowIsSelected ? g.setColour (Colour::greyLevel (0.2)) : g.setColour (Colour::greyLevel (0.9));
	g.setFont (font);

    {
        ScopedLock sl (currentLibrary->getParserLock());
        const ValueTree& rowElement (filteredDataList.getChild (rowNumber));
//        const ValueTree& rowElement (filteredArray[rowNumber]);
    
        if (rowElement.isValid())
        {
            String text;
            
            if(columnId == Columns::Length)
                text = secondsToTimeLength (rowElement[Columns::columnNames[columnId]].toString().getIntValue());
            else if(columnId == Columns::Added
                    || columnId == Columns::Modified)
                text = Time (int64 (rowElement[Columns::columnNames[columnId]])).formatted ("%d/%m/%Y - %H:%M");
            else
                text = rowElement[Columns::columnNames[columnId]].toString();
            
            g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
        }
    }

	g.setColour (Colour::greyLevel (0.9).withAlpha (0.2f));
	g.fillRect (width - 1, 0, 1, height);
	g.fillRect (0, height - 1, width, 1);
}

void MusicLibraryTable::sortOrderChanged (int newSortColumnId, const bool isForwards)
{
	if (newSortColumnId != 0) // can only sort when not still adding (could lock?)
	{
        ScopedLock sl (currentLibrary->getParserLock());
        DBG ("sortOrderChanged");
        
		if (newSortColumnId == Columns::Length
			|| newSortColumnId == Columns::BPM
			|| newSortColumnId == Columns::LibID
			|| newSortColumnId == Columns::ID
            || newSortColumnId == Columns::Added
            || newSortColumnId == Columns::Modified)
		{
			ValueTreeComparators::Numerical sorter (Columns::columnNames[newSortColumnId], isForwards);
			filteredDataList.sort (sorter, 0, false);
//            dataList.sort (sorter, 0, false);
		}
		else
        {
			ValueTreeComparators::Lexicographic sorter (Columns::columnNames[newSortColumnId], isForwards);
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
//            const ValueTree& rowElement (filteredArray[i]);

            if (rowElement.isValid())
            {
                const String text (rowElement[Columns::columnNames[columnId]].toString());
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

void MusicLibraryTable::focusOfChildComponentChanged (FocusChangeType cause)
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
                itemsArray.append(childTree.getObject());
            }
//            itemsArray.append((int)filteredDataList.getChild(currentlySelectedRows[i]).getProperty(Columns::columnNames[Columns::ID]));
        }
            
        return itemsArray;
            
//		ScopedPointer<XmlElement> tracksToDrag (new XmlElement("ITEMS"));
//
//		for(int i = 0; i < currentlySelectedRows.size(); i++)
//		{
//			tracksToDrag->addChildElement(filteredDataList.getChild(currentlySelectedRows[i]).createXml());
//		}
//		
//		return tracksToDrag->createDocument("", false, false);
	}
	return var::null; //String::empty;
}

//==============================================================================
//void MusicLibraryTable::updateFilteredSortOrder()
//{
//    for (int i = 0; i < dataList.getNumChildren(); i++)
//    {
//        for (int f = 0; f < filteredArray.size(); f++)
//        {
//            if (filteredArray.getReference (f) == dataList.getChild (i))
//            {
//                filteredArray.move (filteredArray.indexOf (filteredArray[f]), filteredArray.size() - 1);
//            }
//        }
//    }
//}

//ValueTree MusicLibraryTable::getRowFromFilteredList (int rowNumber)
//{
//    int idToLookFor = filteredDataList[rowNumber];
//    const int numRows = dataList.getNumChildren();
//
//    for (int i = 0, i < numRows; i++)
//    {
//        ValueTree elm (dataList[i]);
//        if (int (elm[Columns::columnNames[Columns::LibID]]) == idToLookFor)
//        {
//            return elm;
//        }
//    }
//    
//    return ValueTree::invalid;
//}

END_JUCE_NAMESPACE
