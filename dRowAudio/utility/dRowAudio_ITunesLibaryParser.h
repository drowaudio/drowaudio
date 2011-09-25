/*
  ==============================================================================

    dRowAudio_iTunesLibaryParser.h
    Created: 29 Jan 2011 1:04:16pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_ITUNESLIBARYPARSER_H_79B40639__
#define __DROWAUDIO_ITUNESLIBARYPARSER_H_79B40639__

#include "../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_Utility.h"

namespace LoopAndCueHelpers
{

    /** Returns the time from a give cue point index in a cue point tree.
        The index starts at 0 and will return 0.0 if the index is out of range.
     */
    inline double getTimeFromCueString (ValueTree& cueTree, int index)
    {
        if (index < cueTree.getNumProperties())
        {
            const String property(cueTree.getProperty(cueTree.getPropertyName(index)).toString());
            return property.upToFirstOccurrenceOf(",", false, false).getDoubleValue();
        }
        
        return 0.0;
    }

    /** Returns the time from a give cue point index in a cue point tree.
        The index starts at 0 and will return white if the index is out of range.
     */
    inline uint32 getColourFromCueString (ValueTree& cueTree, int index)
    {
        if (index < cueTree.getNumProperties())
        {
            const String property(cueTree.getProperty(cueTree.getPropertyName(index)).toString());
            return (uint32)property.fromLastOccurrenceOf(",", false, false).getLargeIntValue();
        }
        
        return 0xffffffff;
    }    
}

/**	Details the colums of the table.
 */
namespace Columns {
    
    static const Identifier libraryIdentifier ("DATA");
    static const Identifier libraryItemIdentifier ("ITEM");
    static const Identifier libraryCuePointIdentifier ("CUE");
    
	enum colums {
		Dummy,
		LibID,
		ID,
		Artist,
		Song,
		Album,
		Rating,
		BPM,
		Genre,
		SubGenre,
		Label,
		Key,
		Length,
		Kind,
		Added,
        Modified,
		Location,
		Score,
		numColumns
	};
	
	static const Identifier columnNames[] = { 
		"Dummy",
		"LibID",
		"ID",						
		"Artist",
		"Song",
		"Album",
		"Rating",
		"BPM",
		"Genre",
		"Sub_Genre",
		"Label",
		"Key",
		"Length",
		"Kind",
		"Added",
        "Modified",
		"Location",
		"Score"
	};
	
	static const char *iTunesNames[] = {
		"",
		"",
		"Track ID",
		"Artist",
		"Name",
		"Album",
		"Rating",
		"BPM",
		"Genre",
		"Grouping",
		"Comments",
		"Series",
		"Total Time",
		"Kind",
		"Date Added",
        "Date Modified",
		"Location",
		"Score"
	};
	
	static const int columnWidths[] = {
		1,
		30,
		50,
		150,
		200,
		150,
		60,
		60,
		80,
		100,
		100,
		30,
		80,
		60,
		125,
		125,
		300,
		60
	};
}

/** Represents an iTunes Xml library into a value tree.
 
    If the tree passed in already contains a generated library this will merge
    any new data from the file into it preserving and sub-trees or attributes
    that may have been added.
 */
class ITunesLibraryParser : public Thread
{
public:
	ITunesLibraryParser(File &iTunesLibraryFile, ValueTree elementToFill);
	
	~ITunesLibraryParser();
	
	void run();
	
	bool hasFinished()	{	return finished;	}
	
private:
	CriticalSection lock;
	int numAdded;
	bool finished;
	
	ValueTree treeToFill, partialTree;
	ScopedPointer<XmlElement> iTunesDatabase;
	XmlElement *iTunesLibraryTracks, *currentElement;

	JUCE_LEAK_DETECTOR (ITunesLibraryParser);
};



#endif  // __DROWAUDIO_ITUNESLIBARYPARSER_H_79B40639__
