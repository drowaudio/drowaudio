/*
  ==============================================================================

    dRowAudio_iTunesLibaryParser.cpp
    Created: 29 Jan 2011 1:04:16pm
    Author:  David Rowland

  ==============================================================================
*/

#include "dRowAudio_ITunesLibaryParser.h"

BEGIN_DROWAUDIO_NAMESPACE

ITunesLibraryParser::ITunesLibraryParser(File &iTunesLibraryFile, ValueTree elementToFill)
:	Thread("iTunesLibraryParser"),
	finished(false),
	treeToFill(elementToFill)
{
	iTunesDatabase = XmlDocument::parse(iTunesLibraryFile);
	
	// find start of tracks library
	iTunesLibraryTracks = iTunesDatabase->getFirstChildElement()->getChildByName("dict");
	currentElement = iTunesLibraryTracks->getFirstChildElement();
	
	startThread(1);
}

ITunesLibraryParser::~ITunesLibraryParser()
{
	stopThread(1000);
}

void ITunesLibraryParser::run()
{
	static int num = 0;
	while (!threadShouldExit())
	{
		if (currentElement->getTagName() == "dict")
		{
			ValueTree newElement("ITEM");
			
			bool add = false;
			// cycle through items of each track
			forEachXmlChildElement(*currentElement, e2)
			{	
				if (e2->getAllSubText() == "Kind") {
					if (e2->getNextElement()->getAllSubText().contains("audio file"))
					{
						newElement.setProperty(Columns::columnNames[1], num, 0);
						num++;
						add = true;
					}
				}
				
				for(int i = 2; i < Columns::numColumns; i++)
				{					
					if (e2->getAllSubText() == Columns::iTunesNames[i])
					{
						String entry = e2->getNextElement()->getAllSubText();
						
						if (i == Columns::Length
							|| i == Columns::BPM
							|| i == Columns::LibID
							|| i == Columns::ID)
						{
							newElement.setProperty(Columns::columnNames[i], entry.getDoubleValue(), 0);
						}
						else
						{
							if (i == Columns::Location) {
								entry = stripFileProtocolForLocal(entry);
							}
							newElement.setProperty(Columns::columnNames[i], entry, 0);
						}
					}
				}
			}

			if (add == true) {
				ScopedLock sl(lock);
				treeToFill.addChild(newElement, -1, 0);
			}
		}		

		currentElement = currentElement->getNextElement();
		
		if (currentElement == 0)
		{
			finished = true;
			signalThreadShouldExit();
			DBG("total added now: "<<num);
			DBG("from tree: "<<treeToFill.getNumChildren());
			
			File output(File::getSpecialLocation(File::userDesktopDirectory).getChildFile("library.xml"));
			ScopedPointer<XmlElement> xmlOutput(treeToFill.createXml());
			xmlOutput->writeToFile(output, "", "", 800);
		}
	}
}

END_DROWAUDIO_NAMESPACE

/*
static void fillITunesData (File &iTunesLibraryFile, XmlElement* elementToFill)
{	
	// cycle through each track
	forEachXmlChildElement(*iTunesLibraryTracks, e)
	{
		if (e->getTagName() == T("dict"))
		{
			XmlElement* tempElement = new XmlElement("ITEM");
			
			// cycle through items of each track
			forEachXmlChildElement(*e, e2)
			{				
				for(int i = 0; i < Columns::numColumns; i++)
				{
					if (e2->getAllSubText() == String(Columns::iTunesNames[i])) {
						String entry = e2->getNextElement()->getAllSubText();
						if (i == Columns::Location) {
							entry = stripFileProtocolForLocal(entry);
						}
						tempElement->setAttribute(String(Columns::columnNames[i]), entry);
					}
				}
			}
			elementToFill->addChildElement(tempElement);
		}
	}		
}*/
