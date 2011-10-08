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
	
	demoData->addChild(ValueTree(Columns::libraryIdentifier), -1, 0);
	dataList = demoData->getChildWithName(Columns::libraryIdentifier);

	// set up table
	addAndMakeVisible (table = new TableListBox ("prepare table", this));
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
	return dataList.getNumChildren();
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

void PrepareTable::removeRow (int rowNumber)
{
    // first remove removeObject
//    ReferenceCountedValueTree::Ptr treeToEdit (dynamic_cast<ReferenceCountedValueTree*> (dataList.getChild (int(rowNumber)).getProperty (originalTreeIdentifier).getObject()));
//    if (treeToEdit != nullptr && treeToEdit->getValueTree().hasType (Columns::libraryItemIdentifier))
//        treeToEdit->getValueTree().removeProperty ("removeObject", nullptr);
        
    // then remove item from list
    dataList.removeChild(int(rowNumber), nullptr);
    
    for(int i = 0; i < dataList.getNumChildren(); i++)
        dataList.getChild (i).setProperty(Columns::columnNames[Columns::LibID], i + 1, nullptr);
    
    table->updateContent();
}

//==============================================================================
var PrepareTable::getDragSourceDescription (const SparseSet< int > &currentlySelectedRows)
{
	if(! currentlySelectedRows.isEmpty())
	{
        var itemsArray;
        
        for (int i = 0; i < currentlySelectedRows.size(); ++i)
        {
            itemsArray.append (dataList.getChild (currentlySelectedRows[i]).getProperty (originalTreeIdentifier));
            removeRow (currentlySelectedRows[i]);
        }
        
        return itemsArray;
	}
	
	return var::null;
}

//==============================================================================
bool PrepareTable::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    ReferenceCountedValueTree::Ptr libraryTree (dynamic_cast<ReferenceCountedValueTree*> (dragSourceDetails.description[0].getObject()));
    
    if (libraryTree != nullptr && libraryTree->getValueTree().hasType (Columns::libraryItemIdentifier))
        return true;
	
	return false;	
}

void PrepareTable::itemDragEnter (const SourceDetails& dragSourceDetails)
{
	itemOver = true;
    repaint();
}

void PrepareTable::itemDragExit (const SourceDetails& dragSourceDetails)
{
	itemOver = false;
    repaint();
}

void PrepareTable::itemDropped (const SourceDetails& dragSourceDetails)
{
    if (dragSourceDetails.description.isArray()) 
    {
        for (int i = 0; i < dragSourceDetails.description.size(); ++i) 
        {
            ReferenceCountedValueTree::Ptr childTree (dynamic_cast<ReferenceCountedValueTree*> (dragSourceDetails.description[i].getObject()));
            
            if (childTree != nullptr) 
            {
                ValueTree itemTree (childTree->getValueTree().createCopy());
                dataList.addChild(itemTree, -1, 0);
                itemTree.setProperty(originalTreeIdentifier, childTree.getObject(), nullptr);
                itemTree.setProperty(Columns::columnNames[Columns::LibID], getNumRows(), nullptr);
            }
        }
    }
    
    table->getHeader().reSortTable();
    table->updateContent();
    
	itemOver = false;
    repaint();
}

//==========================================================================================
void PrepareTable::resized()
{
	table->setBounds(getLocalBounds());
}

void PrepareTable::paintOverChildren(Graphics &g)
{
	if (itemOver) 
    {
		g.setColour(Colours::darkorange);
		g.drawRect(0, 0, getWidth(), getHeight(), 2);
	}
}

void PrepareTable::focusOfChildComponentChanged (FocusChangeType cause)
{
	repaint();
}

const Identifier PrepareTable::originalTreeIdentifier ("originalTree");