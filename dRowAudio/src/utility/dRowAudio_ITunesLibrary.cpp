/*
  ==============================================================================

    ITunesLibrary.cpp
    Created: 6 Feb 2011 6:27:15pm
    Author:  David Rowland

  ==============================================================================
*/

#include "dRowAudio_ITunesLibrary.h"

BEGIN_DROWAUDIO_NAMESPACE

juce_ImplementSingleton(ITunesLibrary);

ITunesLibrary::ITunesLibrary()
:	libraryTree("DATA")
{
	File libraryFile ("/Users/Dave/Documents/Developement/Juce Projects/Music Library Test/iTunes Music Library_small.xml");
	
	if (libraryFile.existsAsFile()) {
		parser = new ITunesLibraryParser(libraryFile, libraryTree);
		startTimer(500);
	}
	
}

ITunesLibrary::~ITunesLibrary()
{
}

void ITunesLibrary::timerCallback()
{
	if(parser != 0)
	{
		listeners.call (&Listener::libraryUpdated, this);

		if (parser->hasFinished()) {
			stopTimer();
			parser = 0;
			listeners.call (&Listener::libraryFinished, this);
		}
	}
}

//==============================================================================
void ITunesLibrary::addListener (ITunesLibrary::Listener* const listener)
{
    listeners.add (listener);
}

void ITunesLibrary::removeListener (ITunesLibrary::Listener* const listener)
{
    listeners.remove (listener);
}

//==============================================================================

END_DROWAUDIO_NAMESPACE