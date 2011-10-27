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
	ITunesLibraryParser (File &iTunesLibraryFileToUse, ValueTree elementToFill,
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
