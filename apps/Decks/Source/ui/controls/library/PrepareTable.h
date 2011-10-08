/*
  ==============================================================================

    PrepareTable.h
    Created: 30 Jan 2011 4:56:49am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __PREPARETABLE_H_482F82F9__
#define __PREPARETABLE_H_482F82F9__

#include "../JuceLibraryCode/JuceHeader.h"


class PrepareTable :	public Component,
						public TableListBoxModel,
						public DragAndDropTarget
{
public:
	PrepareTable();
	
	~PrepareTable();
	
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

	/** Removes a row from the table.
        This is used when dragging rows from the table to players where only one row may need to be removed.
     */
    void removeRow (int rowNumber);
    
    //==============================================================================
	var getDragSourceDescription (const SparseSet< int > &currentlySelectedRows);

    //==============================================================================
	bool isInterestedInDragSource (const SourceDetails& dragSourceDetails);

	void itemDragEnter (const SourceDetails& dragSourceDetails);

    void itemDragExit (const SourceDetails& dragSourceDetails);
	
	void itemDropped (const SourceDetails& dragSourceDetails);
	
    //==============================================================================
    void resized();
	
	void paintOverChildren(Graphics &g);
	
	void focusOfChildComponentChanged (FocusChangeType cause); 
	
private:
    Font font;	

	ScopedPointer<ValueTree> demoData;   // This is the XML document loaded from the embedded file "demo table data.xml"
    ValueTree columnList; // A pointer to the sub-node of demoData that contains the list of columns
    ValueTree dataList;   // A pointer to the sub-node of demoData that contains the list of data rows
    int numRows;            // The number of rows of data we've got
	
    TableListBox* table;    // the table component itself
	bool itemOver;
    
    static const Identifier originalTreeIdentifier;
};


#endif  // __PREPARETABLE_H_482F82F9__
