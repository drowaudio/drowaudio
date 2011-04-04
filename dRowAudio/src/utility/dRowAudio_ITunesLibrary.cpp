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
	File libraryFile (File::getSpecialLocation(File::userMusicDirectory).getChildFile("iTunes/iTunes Music Library.xml"));
	setLibraryFile(libraryFile);
}

ITunesLibrary::~ITunesLibrary()
{
}

void ITunesLibrary::setLibraryFile(File newFile)
{
	if (newFile.existsAsFile()) 
	{
		libraryTree.removeAllChildren(0);
		listeners.call (&Listener::libraryChanged, this);
		parser = new ITunesLibraryParser(newFile, libraryTree);
		startTimer(500);
	}	
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