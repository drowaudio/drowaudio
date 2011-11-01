/*
  ==============================================================================

    TrackSuggestionTable.h
    Created: 15 Mar 2011 1:24:14pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __TRACKSUGGESTIONTABLE_H_1F8623__
#define __TRACKSUGGESTIONTABLE_H_1F8623__

#include "../JuceLibraryCode/JuceHeader.h"

#include "TrackSuggestions.h"

class TrackSuggestionTable : public Component,
							 public TableListBoxModel,
							 public ComboBox::Listener
{
public:
	TrackSuggestionTable(ValueTree sourceToBaseSuggestionsOn, ValueTree libraryDatabase);

	~TrackSuggestionTable();
	
	void resized();
	
	void paint(Graphics &g);

	//==============================================================================
	void setMixTypeMask(int newMaskValue);
	
	void setSourceTrack(ValueTree newSource, ValueTree libraryDatabase, int mixType);
	
	void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
	
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
	
	var getDragSourceDescription (const SparseSet< int > &currentlySelectedRows);

    //==============================================================================
	
private:
	
	Font font;	
	
    ScopedPointer<ValueTree> demoData;   // This is the XML document loaded from the embedded file "demo table data.xml"
    ValueTree columnList; // A pointer to the sub-node of demoData that contains the list of columns
    ValueTree dataList;   // A pointer to the sub-node of demoData that contains the list of data rows
    int numRows;            // The number of rows of data we've got
		
	//ComboBox *mixTypeBox;
    TableListBox* table;    // the table component itself
	ValueTree currentSource, currentLibrary;
	
    //==============================================================================
	void setUpColumns (ValueTree &elementToSetUp)
	{
		for (int i = 1; i < MusicColumns::numColumns; i++)
		{
			ValueTree tempElement("COLUMN");
			tempElement.setProperty("columnId", i, 0);
			tempElement.setProperty("name", MusicColumns::columnNames[i].toString(), 0);
			tempElement.setProperty("width", MusicColumns::columnWidths[i], 0);
			
			elementToSetUp.addChild(tempElement, -1, 0);
		}
	}
	
	void loadData()
	{
        demoData = new ValueTree("SUGGESTIONS_TABLE");
		
		demoData->addChild(ValueTree("COLUMNS"), -1, 0);
		columnList = demoData->getChildWithName("COLUMNS");
		setUpColumns(columnList);
		
		demoData->addChild(ValueTree(MusicColumns::libraryIdentifier), -1, 0);
		dataList = demoData->getChildWithName(MusicColumns::libraryIdentifier);
				
		numRows = dataList.getNumChildren();
	}	
	//==============================================================================

//	enum mixTypes {
//		any,
//		plusOne,
//		minusOne,
//		moodChange,
//		plusTwoBoost,
//		numMixTypes
//	};
	
//	static const char* mixTypeNames[];
};

#endif  // __TRACKSUGGESTIONTABLE_H_1F8623__
