/*
  ==============================================================================

    ITunesLibrary.h
    Created: 6 Feb 2011 6:27:15pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __ITUNESLIBRARY_H_CF4E7153__
#define __ITUNESLIBRARY_H_CF4E7153__

#include "../core/dRowAudio_StandardHeader.h"
#include "dRowAudio_ITunesLibaryParser.h"

class ITunesLibrary : public Timer
{
public:
	
	juce_DeclareSingleton(ITunesLibrary, false);
	
	ITunesLibrary();
	
	~ITunesLibrary();
	
	ValueTree getLibraryTree()	{	return libraryTree;	}
	
	void timerCallback();
	
	//==============================================================================
    /** A class for receiving callbacks from an ITunesLibrary.
	 
		This will repeatedly call libraryUpdated so you can respond to any changes
		that may have happened until a single call to libraryFinished() where you
		may want to do some additional set-up.
	 
		@see ITunesLibrary::addListener, ITunesLibrary::removeListener
	 */
    class  Listener
    {
    public:
        //==============================================================================
        /** Destructor. */
        virtual ~Listener() {}
		
        //==============================================================================
        /** Called when the library has been updated.
		 */
        virtual void libraryUpdated (ITunesLibrary *library) = 0;

        virtual void libraryFinished (ITunesLibrary *library) {}
    };
	
    /** Adds a listener to be called when this slider's value changes. */
    void addListener (Listener* listener);
	
    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);
	
    //==============================================================================
	
private:
	
	ListenerList <Listener> listeners;

	ScopedPointer<ITunesLibraryParser> parser;
	ValueTree libraryTree;
};



#endif  // __ITUNESLIBRARY_H_CF4E7153__
