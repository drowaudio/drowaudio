/*
  ==============================================================================

    ITunesLibrary.cpp
    Created: 6 Feb 2011 6:27:15pm
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

juce_ImplementSingleton(ITunesLibrary);

ITunesLibrary::ITunesLibrary()
:	//libraryFile (getDefaultITunesLibraryFile()),
    libraryTree(Columns::libraryIdentifier)
{
	//setLibraryFile(libraryFile);
}

ITunesLibrary::~ITunesLibrary()
{
}

void ITunesLibrary::setLibraryFile (File newFile)
{
	if (newFile.existsAsFile()) 
	{
		listeners.call (&Listener::libraryChanged, this);
		parser = new ITunesLibraryParser (newFile, libraryTree, parserLock);
		startTimer(500);
	}	
}

//==============================================================================
File ITunesLibrary::getDefaultITunesLibraryFile()
{
#ifdef JUCE_MAC
    return File::getSpecialLocation (File::userMusicDirectory).getChildFile ("iTunes/iTunes Music Library.xml");
#else
    return File::nonexistent;
#endif
}

//==============================================================================
void ITunesLibrary::setLibraryTree (ValueTree& newTreeToUse)
{
    if (! newTreeToUse.isValid()) 
    {
        newTreeToUse = ValueTree (Columns::libraryIdentifier);
    }

    libraryTree = newTreeToUse;
}

void ITunesLibrary::timerCallback()
{
	if(parser != nullptr)
	{
		listeners.call (&Listener::libraryUpdated, this);

		if (parser->hasFinished())
        {
			stopTimer();
			parser = nullptr;
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

END_JUCE_NAMESPACE