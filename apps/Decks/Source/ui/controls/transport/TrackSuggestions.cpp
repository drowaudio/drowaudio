/*
  ==============================================================================

    TrackSuggestions.cpp
    Created: 6 Feb 2011 2:09:29pm
    Author:  David Rowland

  ==============================================================================
*/

#include "TrackSuggestions.h"

TrackSuggestions::TrackSuggestions(ValueTree sourceToBaseSuggestionsOn, ValueTree libraryDatabase)
:	font (12.0f),
	numRows(0)
{
	loadData();
	
	// Create our table component and add it to this component..
	addAndMakeVisible (table = new TableListBox ("suggestion table", this));
	table->setColour(ListBox::backgroundColourId, Colour::greyLevel(0.2));
	table->setHeaderHeight(18);
	table->setRowHeight(16);
	table->getViewport()->setScrollBarThickness(10);
	
	// give it a border
	table->setColour (ListBox::outlineColourId, Colours::grey);
	table->setOutlineThickness (0);
	
	// Add some columns to the table header, based on the column list in our database..
	for (int i = 0; i < columnList.getNumChildren(); i++)
	{
		table->getHeader().addColumn (columnList.getChild(i)["name"],
									  columnList.getChild(i)["columnId"],
									  columnList.getChild(i)["width"],
									  50, 800,
									  TableHeaderComponent::defaultFlags);
	}	
	
	table->getHeader().setColumnWidth(Columns::Artist, 100);
	table->getHeader().setColumnWidth(Columns::Song, 150);
	
	table->getHeader().setColumnVisible (Columns::LibID, false);
	table->getHeader().setColumnVisible (Columns::Album, false);
	table->getHeader().setColumnVisible (Columns::ID, false);
	table->getHeader().setColumnVisible (Columns::Rating, false);
	table->getHeader().setColumnVisible (Columns::Length, false);
	table->getHeader().setColumnVisible (Columns::Added, false);
	table->getHeader().setColumnVisible (Columns::Location, false);
	
	setSourceTrack(sourceToBaseSuggestionsOn, libraryDatabase);
}

TrackSuggestions::~TrackSuggestions()
{
	deleteAllChildren();
}

void TrackSuggestions::resized()
{
	table->setBounds(getLocalBounds());
}

void TrackSuggestions::paint(Graphics &g)
{
}

//==============================================================================
void TrackSuggestions::setSourceTrack(ValueTree newSource, ValueTree libraryDatabase)
{
	double minBpm = double(newSource.getProperty(Columns::columnNames[Columns::BPM], 0)) * 0.99;
	double maxBpm = double(newSource.getProperty(Columns::columnNames[Columns::BPM], 0)) * 1.01;
	for(int i = 0; i < libraryDatabase.getNumChildren(); i++)
	{
		double bpm = libraryDatabase.getChild(i).getProperty(Columns::columnNames[Columns::BPM]);
		if (bpm >= minBpm && bpm <= maxBpm) {
			dataList.addChild(libraryDatabase.getChild(i).createCopy(), -1, 0);
		}
	}
	
	numRows = dataList.getNumChildren();

	table->updateContent();
}

//==============================================================================
int TrackSuggestions::getNumRows()
{
	return numRows;
}

void TrackSuggestions::paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		table->hasKeyboardFocus(true) ? g.fillAll (Colours::darkorange) : g.fillAll (Colour::greyLevel(0.6));
	else
		g.fillAll (Colour::greyLevel(0.2));
}

void TrackSuggestions::paintCell (Graphics& g,
								   int rowNumber,
								   int columnId,
								   int width, int height,
								   bool rowIsSelected)
{
	rowIsSelected ? g.setColour(Colour::greyLevel(0.2)) : g.setColour(Colour::greyLevel(0.9));
	g.setFont (font);
	
	ValueTree rowElement (dataList.getChild(rowNumber));
	
	if (rowElement.isValid())
	{
		String text;
		if(columnId == Columns::Length)
			text = secondsToTimeLength(rowElement[(Columns::columnNames[columnId])].toString().getIntValue());
		else
			text = (rowElement[Columns::columnNames[columnId]].toString());
		
		g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
	}
	
	g.setColour (Colour::greyLevel(0.9).withAlpha (0.2f));
	g.fillRect (width - 1, 0, 1, height);
	g.fillRect (0, height - 1, width, 1);
}

void TrackSuggestions::sortOrderChanged (int newSortColumnId, const bool isForwards)
{
	if (newSortColumnId != 0)
	{
		if (newSortColumnId == Columns::Length
			|| newSortColumnId == Columns::BPM
			|| newSortColumnId == Columns::LibID
			|| newSortColumnId == Columns::ID)
		{
			ValueTreeComparators::Numerical sorter (Columns::columnNames[newSortColumnId], isForwards);
			dataList.sort (sorter, 0, false);
		}
		else {
			ValueTreeComparators::Lexicographic sorter (Columns::columnNames[newSortColumnId], isForwards);
			dataList.sort (sorter, 0, false);
		}
		
		table->updateContent();
	}
}

int TrackSuggestions::getColumnAutoSizeWidth (int columnId)
{
	int widest = 32;
	
	// find the widest bit of text in this column..
	for (int i = getNumRows(); --i >= 0;)
	{
		const ValueTree rowElement = dataList.getChild (i);
		
		if (rowElement.isValid())
		{
			const String text (rowElement[Columns::columnNames[columnId]].toString());
			
			widest = jmax (widest, font.getStringWidth (text));
		}
	}
	
	return widest + 8;
}

const String TrackSuggestions::getDragSourceDescription (const SparseSet< int > &currentlySelectedRows)
{
	if(!currentlySelectedRows.isEmpty())
	{
		ScopedPointer<XmlElement> tracksToDrag (new XmlElement("ITEMS"));
		for(int i = 0; i < currentlySelectedRows.size(); i++)
		{
			XmlElement *item (new XmlElement(*dataList.getChild(currentlySelectedRows[i]).createXml()));
			tracksToDrag->addChildElement(item);
		}
		
		return tracksToDrag->createDocument("", false, false);
	}
	return String::empty;
}

//==============================================================================
