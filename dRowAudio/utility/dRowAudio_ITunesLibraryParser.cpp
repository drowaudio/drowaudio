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



ITunesLibraryParser::ITunesLibraryParser (File& iTunesLibraryFileToUse, ValueTree elementToFill,
                                          CriticalSection& lockToUse)
    : Thread ("iTunesLibraryParser"),
      lock (lockToUse),
      iTunesLibraryFile (iTunesLibraryFileToUse),
      treeToFill (elementToFill),
      numAdded (0),
      finished (false)
{
	startThread (1);
}

ITunesLibraryParser::~ITunesLibraryParser()
{
	stopThread (1000);
}

void ITunesLibraryParser::run()
{
    // parse the iTunes xml first
    iTunesDatabase = XmlDocument::parse (iTunesLibraryFile);
    if (! iTunesDatabase->hasTagName ("plist")
        || iTunesDatabase->getStringAttribute ("version") != "1.0")
    {
        jassertfalse; // not a vlid iTunesLibrary file!
        finished = true;
        return;
    }
	
    // find start of tracks library
    iTunesLibraryTracks = iTunesDatabase->getFirstChildElement()->getChildByName ("dict");
    currentElement = iTunesLibraryTracks->getFirstChildElement();
    
    // find any existing elements
    SortedSet<int> existingIds;
    if (! threadShouldExit())
    {
        if (treeToFill.hasType (MusicColumns::libraryIdentifier))
        {
            for (int i = 0; i < treeToFill.getNumChildren(); ++i)
            {
                int idOfChild = int (treeToFill.getChild (i).getProperty (MusicColumns::columnNames[MusicColumns::ID]));
                existingIds.add (idOfChild);
            }
        }
    }

	while (! threadShouldExit())
	{
        int currentItemId = -1;
        ValueTree newElement;

        bool alreadyExists = false;
        bool needToModify = false;
        bool isAudioFile = false;
        
        while (currentElement != nullptr)
        {
            if (currentElement->getTagName() == "key")
            {
                currentItemId = currentElement->getAllSubText().getIntValue();
                
                ScopedLock sl (lock);
                
                if (existingIds.contains (currentItemId))
                {
                    alreadyExists = true;
                    existingIds.removeValue (currentItemId);
                    newElement = treeToFill.getChildWithProperty (MusicColumns::columnNames[MusicColumns::ID], currentItemId);
                }
                else
                {
                    alreadyExists = false;
                    newElement = ValueTree (MusicColumns::libraryItemIdentifier);
                    newElement.setProperty (MusicColumns::columnNames[MusicColumns::ID], currentItemId, nullptr);
                }
                
                if (alreadyExists)
                {
                    XmlElement *trackDetails = currentElement->getNextElement();
                    
                    forEachXmlChildElement(*trackDetails, e)
                    {
                        if (e->getAllSubText() == MusicColumns::iTunesNames[MusicColumns::Modified])
                        {
                            int64 newModifiedTime = parseITunesDateString (e->getNextElement()->getAllSubText()).toMilliseconds();
                            int64 currentModifiedTime = newElement.getProperty (MusicColumns::columnNames[MusicColumns::Modified]);
                                                        
                            if (newModifiedTime > currentModifiedTime)
                                needToModify = true;
                            
                            break;
                        }
                    }
                }
            }

            currentElement = currentElement->getNextElement();
            
            if (needToModify || (! alreadyExists))
                break;
        }
        
        // check to see if we have reached the end
        if (currentElement == nullptr)
		{
			finished = true;
			signalThreadShouldExit();
            break;
        }            
        
		if (currentElement->getTagName() == "dict")
		{
            // cycle through items of each track
			forEachXmlChildElement (*currentElement, e2)
			{	
                String elementKey (e2->getAllSubText());
                
				if (elementKey == "Kind")
                {
					if (e2->getNextElement()->getAllSubText().contains ("audio file"))
					{
                        isAudioFile = true;
						newElement.setProperty (MusicColumns::columnNames[MusicColumns::LibID], numAdded, nullptr);
						numAdded++;
					}
				}
				
				for(int i = 2; i < MusicColumns::numColumns; i++)
				{					
					if (elementKey == MusicColumns::iTunesNames[i])
					{
						String entry = e2->getNextElement()->getAllSubText();
						
						if (i == MusicColumns::Length
							|| i == MusicColumns::BPM
							|| i == MusicColumns::LibID)
						{
							newElement.setProperty (MusicColumns::columnNames[i], entry.getIntValue(), nullptr);
						}
                        else if (i == MusicColumns::Added
                                 || i == MusicColumns::Modified)
                        {            
                            int64 timeInMilliseconds (parseITunesDateString (entry).toMilliseconds());
                                                        
                            newElement.setProperty (MusicColumns::columnNames[i], timeInMilliseconds, nullptr);
                        }
						else
						{
							if (i == MusicColumns::Location)
								entry = stripFileProtocolForLocal (entry);

							newElement.setProperty (MusicColumns::columnNames[i], entry, nullptr);
						}
					}
				}
			}

			if (isAudioFile == true)
            {
				ScopedLock sl (lock);
				treeToFill.addChild (newElement, -1, nullptr);
			}
            
            currentElement = currentElement->getNextElement();
		}		
	}
}

