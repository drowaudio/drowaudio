/*
  ==============================================================================

    dRowAudio_iTunesLibraryParser.h
    Created: 29 Jan 2011 1:04:16pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_ITUNESLIBRARYPARSER_H__
#define __DROWAUDIO_ITUNESLIBRARYPARSER_H__

#include "dRowAudio_Utility.h"

namespace LoopAndCueHelpers
{
    /** Returns the time from a give cue point index in a cue point tree.
     The index starts at 0 and will return 0.0 if the index is out of range.
     */
    inline double getTimeFromCueTree (ValueTree& cueTree, int index)
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
    inline uint32 getColourFromCueTree (ValueTree& cueTree, int index)
    {
        if (index < cueTree.getNumProperties())
        {
            const String property(cueTree.getProperty(cueTree.getPropertyName(index)).toString());
            return (uint32)property.fromLastOccurrenceOf(",", false, false).getLargeIntValue();
        }
        
        return 0xffffffff;
    }
    
    /** Returns the start time, end time and Colour of a give loop point in a loop tree.
     The index starts at 0 and will return 0.0's if the index is out of range.
     */
    inline void getTimeAndColourFromLoopTree (ValueTree& loopTree, int index, double &startTime, double &endTime, uint32& colour)
    {
        if (index < loopTree.getNumProperties())
        {
            const String property(loopTree.getProperty(loopTree.getPropertyName(index)).toString());
            startTime = property.upToFirstOccurrenceOf(",", false, false).getDoubleValue();
            endTime = property.fromFirstOccurrenceOf(",", false, false).upToLastOccurrenceOf(",", false, false).getDoubleValue();
            colour = (uint32)property.fromLastOccurrenceOf(",", false, false).getLargeIntValue();
            return;
        }
        
        startTime = endTime = 0.0;
        colour = 0xffffffff;
        
        return;
    }
}

/**	Details the colums of the table.
 */
namespace Columns {
    
    static const Identifier libraryIdentifier ("DATA");
    static const Identifier libraryItemIdentifier ("ITEM");
    static const Identifier libraryCuePointIdentifier ("CUE");
    static const Identifier libraryLoopIdentifier ("LOOP");

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
	
	static const UNUSED_NOWARN char *iTunesNames[] = {
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

//==============================================================================
/** Parses an iTunes Xml library into a ValueTree using a background thread.
 
    If the tree passed in already contains a generated library this will merge
    any new data from the file into it preserving any sub-trees or attributes
    that may have been added.
 */
class ITunesLibraryParser : public Thread
{
public:
    //==============================================================================
    /** Creates a parser with a given valid library file and a ValueTree with which
        to put the parsed data.
     */
	ITunesLibraryParser(File &iTunesLibraryFileToUse, ValueTree elementToFill,
                        CriticalSection& lockToUse);
	
    /** Destructor.
     */
	~ITunesLibraryParser();

	/** Returns true if the parser has finished.
     */
	bool hasFinished()          {	return finished;	}

    /** @internal */
	void run();

    CriticalSection& getLock () {   return lock;    }
    
private:
    //==============================================================================
    CriticalSection& lock;
	
    File iTunesLibraryFile;
	ValueTree treeToFill, partialTree;
	ScopedPointer<XmlElement> iTunesDatabase;
	XmlElement *iTunesLibraryTracks, *currentElement;

    int numAdded;
	bool finished;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ITunesLibraryParser);
};

#endif  // __DROWAUDIO_ITUNESLIBRARYPARSER_H__
