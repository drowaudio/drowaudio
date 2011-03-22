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

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_MusicLibraryTable.h"

String parseDateString(String dateString)
{
	dateString = dateString.replace("T", " ", true);
	dateString = dateString.replace("Z", String::empty, true);
	
	String year(dateString.substring(0, 4));
	String month(dateString.substring(5, 7));
	String day(dateString.substring(8, 10));
	dateString = dateString.replace(dateString.upToFirstOccurrenceOf(" ", false, true), String::empty);

	String returnString;
	returnString <<day<<"/"<<month<<"/"<<year<<" -"<<dateString;
	return returnString;
}

MusicLibraryTable::MusicLibraryTable()
:	font (12.0f),
	numRows(0),
	filteredNumRows(0),
	finishedLoading(false)
{
	// Load some data from an embedded XML file..
	loadData();

	// Create our table component and add it to this component..
	addAndMakeVisible (table = new TableListBox (T("demo table"), this));
	table->setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2));
	table->setHeaderHeight(18);
	table->setRowHeight(16);
	table->getViewport()->setScrollBarThickness(10);
	
	// give it a border
	table->setColour (ListBox::outlineColourId, Colours::grey);
	table->setOutlineThickness (1);

	// Add some columns to the table header, based on the column list in our database..
	for (int i = 0; i < columnList.getNumChildren(); i++)
	{
		table->getHeader().addColumn (columnList.getChild(i)["name"],
									  columnList.getChild(i)["columnId"],
									  columnList.getChild(i)["width"],
									  50, 800,
									  TableHeaderComponent::defaultFlags);
	}

	// we could now change some initial settings..
	table->getHeader().setSortColumnId (Columns::Artist, true); // sort forwards by the ID column

	table->getHeader().setColumnVisible (Columns::LibID, false);
	table->getHeader().setColumnVisible (Columns::ID, false);
	table->getHeader().setColumnVisible (Columns::Rating, false);
	table->getHeader().setColumnVisible (Columns::Location, false);
	
	// un-comment this line to have a go of stretch-to-fit mode
	// table->getHeader()->setStretchToFitActive (true);

	table->setMultipleSelectionEnabled (true);
	
	setFilterText(String::empty);
}

MusicLibraryTable::~MusicLibraryTable()
{
	deleteAllChildren();

	if (*filteredDataList == dataList)
		filteredDataList.release();
}

//==============================================================================
void MusicLibraryTable::setLibraryToUse(ITunesLibrary *library)
{
	dataList = library->getLibraryTree();
	library->addListener(this);
}

void MusicLibraryTable::libraryUpdated (ITunesLibrary *library)
{
	filteredNumRows = numRows = dataList.getNumChildren();
	DBG("not finished: "<<filteredNumRows);
	
	table->updateContent();
}

void MusicLibraryTable::libraryFinished (ITunesLibrary *library)
{
	filteredNumRows = numRows = dataList.getNumChildren();
	finishedLoading = true;
	DBG("parser finished: "<<filteredNumRows);

	table->updateContent();
	table->getHeader().reSortTable();

	library->removeListener(this);
	DBG("finished loading MusicLibraryTable");
}

//==============================================================================
int MusicLibraryTable::getNumRows()
{
	return filteredNumRows;
}

void MusicLibraryTable::paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		table->hasKeyboardFocus(true) ? g.fillAll (Colours::darkorange) : g.fillAll (Colour::greyLevel(0.6));
	else
		g.fillAll (Colour::greyLevel(0.2));
}

void MusicLibraryTable::paintCell (Graphics& g,
								   int rowNumber,
								   int columnId,
								   int width, int height,
								   bool rowIsSelected)
{
	rowIsSelected ? g.setColour(Colour::greyLevel(0.2)) : g.setColour(Colour::greyLevel(0.9));
	g.setFont (font);

	ValueTree rowElement (filteredDataList->getChild(rowNumber));

	if (rowElement.isValid())
	{
		String text;
		if(columnId == Columns::Length)
			text = secondsToTimeLength(rowElement[(Columns::columnNames[columnId])].toString().getIntValue());
		else if(columnId == Columns::Added)
			text = parseDateString(rowElement[Columns::columnNames[columnId]].toString());
		else
			text = (rowElement[Columns::columnNames[columnId]].toString());

		g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
	}

	g.setColour (Colour::greyLevel(0.9).withAlpha (0.2f));
	g.fillRect (width - 1, 0, 1, height);
	g.fillRect (0, height - 1, width, 1);
}

void MusicLibraryTable::sortOrderChanged (int newSortColumnId, const bool isForwards)
{
	if (newSortColumnId != 0 && finishedLoading == true)
	{
		if (newSortColumnId == Columns::Length
			|| newSortColumnId == Columns::BPM
			|| newSortColumnId == Columns::LibID
			|| newSortColumnId == Columns::ID)
		{
			ValueTreeComparators::Numerical sorter (Columns::columnNames[newSortColumnId], isForwards);
			filteredDataList->sort (sorter, 0, false);
		}
		else {
			ValueTreeComparators::Lexicographic sorter (Columns::columnNames[newSortColumnId], isForwards);
			filteredDataList->sort (sorter, 0, false);
		}

		table->updateContent();
	}
}

//==========================================================================================
int MusicLibraryTable::getColumnAutoSizeWidth (int columnId)
{
	int widest = 32;

	// find the widest bit of text in this column..
	for (int i = getNumRows(); --i >= 0;)
	{
		const ValueTree rowElement = filteredDataList->getChild (i);

		if (rowElement.isValid())
		{
			const String text (rowElement[Columns::columnNames[columnId]].toString());

			widest = jmax (widest, font.getStringWidth (text));
		}
	}

	return widest + 8;
}

//==========================================================================================
void MusicLibraryTable::setFilterText (String filterString)
{
	DBG(filterString);
	
	if (filterString == String::empty)
	{
		filteredDataList = &dataList;
		filteredNumRows = numRows;
	}
	else
	{
		if (*filteredDataList == dataList)
			filteredDataList.release();
		
		filteredDataList = new ValueTree(dataList.getType());
		
		for (int e = 0; e < dataList.getNumChildren(); e++)
		{
			for (int i = 0; i < dataList.getChild(e).getNumProperties(); i++)
			{
				if (dataList.getChild(e)[Columns::columnNames[i]/*dataList.getChild(e).getPropertyName(i)*/].toString().containsIgnoreCase(filterString))
				{
					filteredDataList->addChild(dataList.getChild(e).createCopy(), -1, 0);
					
					break;
				}
				
			}
		}
		
		filteredNumRows = filteredDataList->getNumChildren();
	}
	
	table->getHeader().reSortTable();
	table->updateContent();
}

//==============================================================================
void MusicLibraryTable::resized()
{
	table->setBounds(getLocalBounds());
}

void MusicLibraryTable::focusOfChildComponentChanged (FocusChangeType cause)
{
	repaint();
}
void MusicLibraryTable::timerCallback()
{
	if (!parser->hasFinished()) {
		filteredNumRows = numRows = dataList.getNumChildren();
		DBG("not finished: "<<filteredNumRows);
	}
	else {
		filteredNumRows = numRows = dataList.getNumChildren();
		DBG("parser finished: "<<filteredNumRows);
		parser = 0;
		stopTimer();
		table->getHeader().reSortTable();
	}
	
	table->updateContent();
}

const String MusicLibraryTable::getDragSourceDescription (const SparseSet< int > &currentlySelectedRows)
{
	if(!currentlySelectedRows.isEmpty())
	{
		ScopedPointer<XmlElement> tracksToDrag (new XmlElement("ITEMS"));

		for(int i = 0; i < currentlySelectedRows.size(); i++)
		{
			tracksToDrag->addChildElement(filteredDataList->getChild(currentlySelectedRows[i]).createXml());
		}
		
		return tracksToDrag->createDocument("", false, false);
	}
	return String::empty;
}

END_DROWAUDIO_NAMESPACE
