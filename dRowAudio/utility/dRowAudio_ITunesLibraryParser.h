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

#ifndef __DROWAUDIO_ITUNESLIBRARYPARSER_H__
#define __DROWAUDIO_ITUNESLIBRARYPARSER_H__

#include "dRowAudio_Utility.h"

//==============================================================================
/** Parses an iTunes Xml library into a ValueTree using a background thread.
 
    If the tree passed in already contains a generated library this will merge
    any new data from the file into it preserving any sub-trees or attributes
    that may have been added.
 
    You shouldn't need to use this directly, use the higher-level iTunesLibrary
    instead.
 */
class ITunesLibraryParser : public Thread
{
public:
    //==============================================================================
    /** Creates a parser with a given valid library file and a ValueTree with which
        to put the parsed data.
     */
	ITunesLibraryParser (File& iTunesLibraryFileToUse, ValueTree elementToFill,
                         CriticalSection& lockToUse);
	
    /** Destructor.
     */
	~ITunesLibraryParser();

	/** Returns true if the parser has finished.
     */
	bool hasFinished()          {	return finished;	}

    /** @internal */
	void run();

    /** Returns the lock being used.
     */
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

    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ITunesLibraryParser);
};

#endif  // __DROWAUDIO_ITUNESLIBRARYPARSER_H__
