/*
  ==============================================================================

    PrepareTable.cpp
    Created: 30 Jan 2011 4:56:49am
    Author:  David Rowland

  ==============================================================================
*/

#include "PrepareTable.h"

PrepareTable::PrepareTable()
:	font(12),
	numRows(0),
	itemOver(false)
{
	demoData = new ValueTree("DEMO_TABLE_DATA");
	
	demoData->addChild(ValueTree("COLUMNS"), -1, 0);
	columnList = demoData->getChildWithName("COLUMNS");

	for (int i = 1; i < Columns::numColumns; i++)
	{
		ValueTree tempElement("COLUMN");
		tempElement.setProperty("columnId", i, 0);
		if (i == Columns::LibID)
			tempElement.setProperty("name", "#", 0);
		else
			tempElement.setProperty("name", Columns::columnNames[i].toString(), 0);
		tempElement.setProperty("width", Columns::columnWidths[i], 0);
		
		columnList.addChild(tempElement, -1, 0);
	}
	
	demoData->addChild(ValueTree("DATA"), -1, 0);
	dataList = demoData->getChildWithName("DATA");

	// set up table
	addAndMakeVisible (table = new TableListBox (T("prepare table"), this));
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
	table->getHeader().setSortColumnId (Columns::LibID, true); // sort forwards by the ID column
	
	table->getHeader().setColumnVisible (Columns::ID, false);
	table->getHeader().setColumnVisible (Columns::Rating, false);
	table->getHeader().setColumnVisible (Columns::Location, false);	
}

PrepareTable::~PrepareTable()
{
	deleteAllChildren();
}

//==============================================================================
int PrepareTable::getNumRows()
{
	return numRows;
}

void PrepareTable::paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		table->hasKeyboardFocus(true) ? g.fillAll (Colours::darkorange) : g.fillAll (Colour::greyLevel(0.6));
	else
		g.fillAll (Colour::greyLevel(0.2));
}

void PrepareTable::paintCell (Graphics& g,
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
//		else if(columnId == Columns::Added)
//			text = parseDateString(rowElement[Columns::columnNames[columnId]].toString());
		else
			text = (rowElement[Columns::columnNames[columnId]].toString());
		
		g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
	}
	
	g.setColour (Colour::greyLevel(0.9).withAlpha (0.2f));
	g.fillRect (width - 1, 0, 1, height);
	g.fillRect (0, height - 1, width, 1);
}

void PrepareTable::sortOrderChanged (int newSortColumnId, const bool isForwards)
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

//==========================================================================================
int PrepareTable::getColumnAutoSizeWidth (int columnId)
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
//==============================================================================
const String PrepareTable::getDragSourceDescription (const SparseSet< int > &currentlySelectedRows)
{
	if(!currentlySelectedRows.isEmpty())
	{
		ScopedPointer<XmlElement> tracksToDrag (new XmlElement("ITEMS"));
		XmlElement *elem = dataList.getChild(currentlySelectedRows[0]).createXml();
		tracksToDrag->addChildElement(new XmlElement(*elem));
		numRows--;
		
		int removedNum = elem->getIntAttribute(Columns::columnNames[Columns::LibID].toString());
		dataList.removeChild(dataList.getChildWithProperty(Columns::columnNames[Columns::LibID], removedNum), 0);
		
		for(int i = 0; i < dataList.getNumChildren(); i++)
		{
			int currentNum = dataList.getChild(i).getProperty(Columns::columnNames[Columns::LibID]);
			if (currentNum > removedNum) {
				dataList.getChild(i).setProperty(Columns::columnNames[Columns::LibID], currentNum-1, 0);
			}
		}
		
		table->updateContent();
		
		return tracksToDrag->createDocument("", false, false);
	}
	
	return String::empty;
}

//==============================================================================
bool PrepareTable::isInterestedInDragSource (const String &sourceDescription, Component *sourceComponent)
{
	if (sourceDescription.startsWith("<ITEMS>")) {
		itemOver = true;
		repaint();
		return true;
	}
	
	return false;
}

void PrepareTable::itemDragExit (const String &sourceDescription, Component *sourceComponent)
{
	if (itemOver) {
		itemOver = false;
		repaint();
	}
}

void PrepareTable::itemDropped (const String &sourceDescription, Component *sourceComponent, int x, int y)
{
	itemOver = false;
	
	ScopedPointer<XmlElement> newTracks (XmlDocument::parse(sourceDescription));

	forEachXmlChildElement(*newTracks, e)
	{
		e->setAttribute(Columns::columnNames[Columns::LibID].toString(), ++numRows);
		ScopedPointer<XmlElement> newElem (new XmlElement(*e));
		dataList.addChild(ValueTree::fromXml(*newElem), -1, 0);
	}

	table->getHeader().reSortTable();
	table->updateContent();
}

//==========================================================================================
void PrepareTable::resized()
{
	table->setBounds(getLocalBounds());
}

void PrepareTable::paintOverChildren(Graphics &g)
{
	if (itemOver) {
		g.setColour(Colours::darkorange);
		g.drawRect(0, 0, getWidth(), getHeight(), 2);
	}
}

void PrepareTable::focusOfChildComponentChanged (FocusChangeType cause)
{
	repaint();
}