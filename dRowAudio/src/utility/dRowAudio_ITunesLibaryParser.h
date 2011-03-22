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

/**	Details the colums of the table.
 */
namespace Columns {
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
		Added,
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
		"Added",
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
		"Date Added",
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
		125,
		300,
		60
	};
}

class ITunesLibraryParser : public Thread
{
public:
	ITunesLibraryParser(File &iTunesLibraryFile, ValueTree elementToFill);
	
	~ITunesLibraryParser();
	
	void run();
	
	bool hasFinished()	{	return finished;	}
	
private:
	CriticalSection lock;
	bool finished;
	
	ValueTree treeToFill, partialTree;
	ScopedPointer<XmlElement> iTunesDatabase;
	XmlElement *iTunesLibraryTracks, *currentElement;

	JUCE_LEAK_DETECTOR (ITunesLibraryParser);
};



#endif  // __DROWAUDIO_ITUNESLIBARYPARSER_H_79B40639__
