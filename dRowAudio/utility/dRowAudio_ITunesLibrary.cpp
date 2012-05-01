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



juce_ImplementSingleton(ITunesLibrary);

ITunesLibrary::ITunesLibrary()
    : libraryTree (MusicColumns::libraryIdentifier)
{
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
    return File::getSpecialLocation (File::userMusicDirectory).getChildFile ("iTunes/iTunes Music Library.xml");
}

//==============================================================================
void ITunesLibrary::setLibraryTree (ValueTree& newTreeToUse)
{
    if (! newTreeToUse.isValid()) 
    {
        newTreeToUse = ValueTree (MusicColumns::libraryIdentifier);
    }

    libraryTree = newTreeToUse;
}

void ITunesLibrary::timerCallback()
{
	if (parser != nullptr)
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

