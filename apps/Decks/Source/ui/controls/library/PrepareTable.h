/*
  ==============================================================================

    PrepareTable.h
    Created: 30 Jan 2011 4:56:49am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __PREPARETABLE_H_482F82F9__
#define __PREPARETABLE_H_482F82F9__

#include <dRowAudio/dRowAudio.h>

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
	
    //==============================================================================
	const String getDragSourceDescription (const SparseSet< int > &currentlySelectedRows);

    //==============================================================================
	bool isInterestedInDragSource (const String &sourceDescription, Component *sourceComponent);

	void itemDragExit (const String &sourceDescription, Component *sourceComponent);
	
	void itemDropped (const String &sourceDescription, Component *sourceComponent, int x, int y);
	
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
};


#endif  // __PREPARETABLE_H_482F82F9__
