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

#ifndef __DROWAUDIO_MUSICLIBRARYTABLE_H__
#define __DROWAUDIO_MUSICLIBRARYTABLE_H__

#include "../audio/dRowAudio_AudioUtility.h"
#include "../utility/dRowAudio_ITunesLibrary.h"
#include "../utility/dRowAudio_Comparators.h"

//==============================================================================
/**
    Table to display and interact with a music library.
    The easiest way to use this is to load a default or saved iTunes library like so:
    
    @code
        MusicLibraryTable table;
        table.setLibraryToUse (ITunesLibrary::getInstance());

        File parsedLibraryFile (File::getSpecialLocation (File::userDesktopDirectory).getChildFile ("saved_library_file.xml"));
        ValueTree libraryTree (readValueTreeFromFile (parsedLibraryFile));
 
        ITunesLibrary::getInstance()->setLibraryTree (libraryTree);
        ITunesLibrary::getInstance()->setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
    @endcode
*/
class MusicLibraryTable	: public Component,
                          public TableListBoxModel,
						  public ITunesLibrary::Listener
{
public:
    //==============================================================================
    /** Create the MusicLibraryTable.
     
        This will initially be blank, set an ITunesLibrary to use first with
        setLibraryToUse(). The table will then be continually updated as the library
        is parsed.
     */
    MusicLibraryTable();

    /** Destructor.
     */
    ~MusicLibraryTable();

    /** Sets the ITunesLibrary to use.
     */
	void setLibraryToUse (ITunesLibrary* library);

    /** Filters the table to only rows containing the given text.
     */
	void setFilterText (String filterText);
    
	/**	Returns the table list box component.
     */
	TableListBox& getTableListBox()	{	return table;	};
    
    //==============================================================================
    /** @internal */
	void libraryChanged (ITunesLibrary* library);
	
    /** @internal */
	void libraryUpdated (ITunesLibrary* library);
	
    /** @internal */
	void libraryFinished (ITunesLibrary* library);
	
    //==============================================================================
    /** Returns the number of rows currently bein displayed in the table.
     */
    int getNumRows();

    /** @internal */
    void paintRowBackground (Graphics& g, int rowNumber,
                             int width, int height, bool rowIsSelected);

    /** @internal */
    void paintCell (Graphics& g,
                    int rowNumber,
                    int columnId,
                    int width, int height,
                    bool rowIsSelected);

    /** @internal */
    void sortOrderChanged (int newSortColumnId, bool isForwards);

    /** @internal */
    int getColumnAutoSizeWidth (int columnId);
	
    //==============================================================================
    /** @internal */
    void resized();

    /** @internal */
	void focusOfChildComponentChanged (FocusChangeType cause); 

    /** @internal */
	var getDragSourceDescription (const SparseSet<int>& currentlySelectedRows);
	
private:
    //==============================================================================
    Font font;	
	ITunesLibrary* currentLibrary;
    TableListBox table;
    String currentFilterText;
    
    ValueTree dataList;
	ValueTree filteredDataList;
        
    int filteredNumRows;
	bool finishedLoading;
	
    void updateFilteredSortOrder();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MusicLibraryTable);
};

#endif // __DROWAUDIO_MUSICLIBRARYTABLE_H__