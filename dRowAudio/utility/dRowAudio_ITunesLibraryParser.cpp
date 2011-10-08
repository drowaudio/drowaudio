/*
  ==============================================================================

    dRowAudio_iTunesLibraryParser.cpp
    Created: 29 Jan 2011 1:04:16pm
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

ITunesLibraryParser::ITunesLibraryParser (File &iTunesLibraryFileToUse, ValueTree elementToFill, CriticalSection& lockToUse)
    : Thread ("iTunesLibraryParser"),
      lock (lockToUse),
      iTunesLibraryFile (iTunesLibraryFileToUse),
      treeToFill (elementToFill),
      numAdded (0),
      finished (false)
{
	startThread(1);
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
    }
	
    // find start of tracks library
    iTunesLibraryTracks = iTunesDatabase->getFirstChildElement()->getChildByName ("dict");
    currentElement = iTunesLibraryTracks->getFirstChildElement();
    
    // find any existing elements
    Array<int> existingIds;
    if (! threadShouldExit())
    {
        if (treeToFill.hasType(Columns::libraryIdentifier))
        {
            for (int i = 0; i < treeToFill.getNumChildren(); ++i)
            {
                int idOfChild = int (treeToFill.getChild (i).getProperty (Columns::columnNames[Columns::ID]));
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
                    newElement = treeToFill.getChildWithProperty (Columns::columnNames[Columns::ID], currentItemId);
                }
                else
                {
                    alreadyExists = false;
                    newElement = ValueTree (Columns::libraryItemIdentifier);
                    newElement.setProperty (Columns::columnNames[Columns::ID], currentItemId, nullptr);
                }
                
                if (alreadyExists)
                {
                    XmlElement *trackDetails = currentElement->getNextElement();
                    
                    forEachXmlChildElement(*trackDetails, e)
                    {
                        if (e->getAllSubText() == Columns::iTunesNames[Columns::Modified])
                        {
                            int64 newModifiedTime = parseITunesDateString (e->getNextElement()->getAllSubText()).toMilliseconds();
                            int64 currentModifiedTime = newElement.getProperty (Columns::columnNames[Columns::Modified]);
                                                        
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
						newElement.setProperty (Columns::columnNames[Columns::LibID], numAdded, nullptr);
						numAdded++;
					}
				}
				
				for(int i = 2; i < Columns::numColumns; i++)
				{					
					if (elementKey == Columns::iTunesNames[i])
					{
						String entry = e2->getNextElement()->getAllSubText();
						
						if (i == Columns::Length
							|| i == Columns::BPM
							|| i == Columns::LibID)
						{
							newElement.setProperty (Columns::columnNames[i], entry.getIntValue(), nullptr);
						}
                        else if (i == Columns::Added
                                 || i == Columns::Modified)
                        {            
                            int64 timeInMilliseconds (parseITunesDateString (entry).toMilliseconds());
                                                        
                            newElement.setProperty (Columns::columnNames[i], timeInMilliseconds, nullptr);
                        }
						else
						{
							if (i == Columns::Location)
								entry = stripFileProtocolForLocal (entry);

							newElement.setProperty (Columns::columnNames[i], entry, nullptr);
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

END_JUCE_NAMESPACE
