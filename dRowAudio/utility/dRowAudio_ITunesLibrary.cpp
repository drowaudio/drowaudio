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

void ITunesLibrary::setLibraryFile(File newFile)
{
	if (newFile.existsAsFile()) 
	{
		//libraryTree.removeAllChildren(0);
		listeners.call (&Listener::libraryChanged, this);
		parser = new ITunesLibraryParser (newFile, libraryTree);
		startTimer(500);
	}	
}

bool ITunesLibrary::loadSavedLibraryIfNewer(File savedLibraryFile)
{
    ScopedPointer<XmlElement> elm (XmlDocument::parse (savedLibraryFile));
    if (elm != nullptr)
    {
        ValueTree savedTree (ValueTree::fromXml(*elm));
        
        if (savedLibraryFile.getLastModificationTime() > getDefaultITunesLibraryFile().getLastModificationTime()) 
        {
            setLibraryTree (savedTree);
            
            return true;
        }
        else if (savedTree.hasType (Columns::libraryIdentifier)) //attempt to update existing
        {
            libraryTree = savedTree;
        }
    }
    
    setLibraryFile (ITunesLibrary::getDefaultITunesLibraryFile());
    
    return false;
}

const File& ITunesLibrary::getLibraryFile()
{
    return libraryFile;
}

void ITunesLibrary::setLibraryTree(ValueTree newTreeToUse)
{
    if (newTreeToUse.hasType (Columns::libraryIdentifier))
    {
        libraryTree = newTreeToUse;
        listeners.call (&Listener::libraryChanged, this);
		listeners.call (&Listener::libraryUpdated, this);
        listeners.call (&Listener::libraryFinished, this);
    }
}

void ITunesLibrary::timerCallback()
{
	if(parser != 0)
	{
		listeners.call (&Listener::libraryUpdated, this);

		if (parser->hasFinished())
        {
			stopTimer();
			parser = 0;
			listeners.call (&Listener::libraryFinished, this);
		}
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