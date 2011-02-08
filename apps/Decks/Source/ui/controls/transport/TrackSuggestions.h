/*
  ==============================================================================

    TrackSuggestions.h
    Created: 6 Feb 2011 2:09:29pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __TRACKSUGGESTIONS_H_89C95328__
#define __TRACKSUGGESTIONS_H_89C95328__

#include <dRowAudio/dRowAudio.h>

class TrackSuggestions : public Component,
						 public TableListBoxModel
{
public:
	TrackSuggestions(ValueTree sourceToBaseSuggestionsOn, ValueTree libraryDatabase);

	~TrackSuggestions();
	
	void resized();
	
	void paint(Graphics &g);

	//==============================================================================
	void setSourceTrack(ValueTree newSource, ValueTree libraryDatabase);
	
	//==============================================================================
    // This is overloaded from TableListBoxModel, and must return the total number of rows in our table
    int getNumRows();
	
    // This is overloaded from TableListBoxModel, and should fill in the background of the whole row
    void paintRowBackground (Graphics& g, int rowNumber, int width, int height, bool rowIsSelected);
	
    // This is overloaded from TableListBoxModel, and must paint any cells that aren't using custom
    // components.
    void paintCell (Graphics& g,
                    int rowNumber,
                    int columnId,
                    int width, int height,
                    bool rowIsSelected);
	
    // This is overloaded from TableListBoxModel, and tells us that the user has clicked a table header
    // to change the sort order.
    void sortOrderChanged (int newSortColumnId, const bool isForwards);
	
    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int getColumnAutoSizeWidth (int columnId);
	
	// Call this to sort the table displaying only the items matched
	void setFilterText (String filterText);
	
	const String getDragSourceDescription (const SparseSet< int > &currentlySelectedRows);

    //==============================================================================
	
private:
	
	Font font;	
	
    ScopedPointer<ValueTree> demoData;   // This is the XML document loaded from the embedded file "demo table data.xml"
    ValueTree columnList; // A pointer to the sub-node of demoData that contains the list of columns
    ValueTree dataList;   // A pointer to the sub-node of demoData that contains the list of data rows
    int numRows;            // The number of rows of data we've got
		
    TableListBox* table;    // the table component itself
    //==============================================================================
	void setUpColumns (ValueTree &elementToSetUp)
	{
		for (int i = 1; i < Columns::numColumns; i++)
		{
			ValueTree tempElement("COLUMN");
			tempElement.setProperty("columnId", i, 0);
			tempElement.setProperty("name", Columns::columnNames[i].toString(), 0);
			tempElement.setProperty("width", Columns::columnWidths[i], 0);
			
			elementToSetUp.addChild(tempElement, -1, 0);
		}
	}
	
	void loadData()
	{
        demoData = new ValueTree("SUGGESTIONS_TABLE");
		
		demoData->addChild(ValueTree("COLUMNS"), -1, 0);
		columnList = demoData->getChildWithName("COLUMNS");
		setUpColumns(columnList);
		
		demoData->addChild(ValueTree("DATA"), -1, 0);
		dataList = demoData->getChildWithName("DATA");
				
		numRows = dataList.getNumChildren();
	}	
};



#endif  // __TRACKSUGGESTIONS_H_89C95328__
