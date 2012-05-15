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

#ifndef __DROWAUDIO_UTILITY_H__
#define __DROWAUDIO_UTILITY_H__

#if JUCE_MSVC
    #pragma warning (disable: 4505)
#endif

#include "dRowAudio_XmlHelpers.h"

//==============================================================================
/** @file
 
	This file contains some useful utility functions and macros.
 */
//==============================================================================

/** Returns the Resources folder in the package contents on a Mac and if an equivalent exists on Windows.
	This will return File::nonexistent if the file does not exist so check for this first.
 */
inline static File getResourcesFolder()
{
	return File::getSpecialLocation (File::currentExecutableFile).getParentDirectory().getParentDirectory().getChildFile ("Resources");
}

/** If the String passed in is a local path, this will return a string with the file://localhost part
	of the file path stripped and any escaped characters (e.g. %20) converted to ascii
 */
inline static String stripFileProtocolForLocal (String pathToStrip)
{
	if (pathToStrip.startsWith ("file://localhost"))
	{
#if JUCE_WINDOWS
		String temp (pathToStrip.substring (pathToStrip.indexOf (7, "/") + 1));
#else
		String temp (pathToStrip.substring (pathToStrip.indexOf (7, "/")));
#endif   
		return temp.replace ("%20", " ").replace ("%5B", "[").replace ("%5D", "]").replace ("%23", "#");
	}
	
	return String::empty;
}

/** Converts an iTunes formatted date string (e.g. 2010-12-27T17:44:32Z)
    into a Time object.
 */
inline static Time parseITunesDateString (String dateString)
{
    int year =      dateString.substring (0, 4).getIntValue();
    int month =     dateString.substring (5, 7).getIntValue();
    int day =       dateString.substring (8, 10).getIntValue();
    int hours =     dateString.substring (11, 13).getIntValue();
    int minutes =   dateString.substring (14, 16).getIntValue();
    int seconds =   dateString.substring (17, 19).getIntValue();
    int milliseconds = 0;
    bool useLocalTime = true;                     
    
    return Time(year,
                month,
                day,
                hours,
                minutes,
                seconds,
                milliseconds,
                useLocalTime);
}

/**	Reverses an array.
 */
template <class Type>
void reverseArray (Type* array, int length)
{
    Type swap;
	
    for (int a = 0; a < --length; a++)  //increment a and decrement b until they meet eachother
    {
        swap = array[a];                //put what's in a into swap space
        array[a] = array[length];       //put what's in b into a
        array[length] = swap;           //put what's in the swap (a) into b
    }
}

/**	Reverses two arrays at once.
	This will be quicker than calling reverseArray twice.
	The arrays must be the same length.
 */
template <class Type>
void reverseTwoArrays (Type* array1, Type* array2, int length)
{
    Type swap;
    for (int a = 0; a < --length; a++)  //increment a and decrement b until they meet eachother
    {
        swap = array1[a];               //put what's in a into swap space
        array1[a] = array1[length];     //put what's in b into a
        array1[length] = swap;          //put what's in the swap (a) into b

        swap = array2[a];               //put what's in a into swap space
        array2[a] = array2[length];     //put what's in b into a
        array2[length] = swap;          //put what's in the swap (a) into b
    }
}

/**	Finds the key for a given track from the chemical-records website.
	This will attempt to find the key listed on the chemical website for a given release number
	eg. "31R038" and track title eg. "Wait For Me".
	This is in the Mixed in Key format eg. 11A and will return an empty string if nothing could be found.
	
	@param	releaseNo	The catalogue number to look for.
	@param	trackName	The track name to look for.
	@param	retryLimit	An optional number of retries as sometimes the URL won't load first time.
 */
static String findKeyFromChemicalWebsite (const String& releaseNo, const String& trackName)
{
    URL chemicalURL ("http://www.chemical-records.co.uk/sc/servlet/Info");
    chemicalURL = chemicalURL.withParameter ("Track", releaseNo);
    
    String pageAsString (chemicalURL.readEntireTextStream());
    String trackInfo (pageAsString.fromFirstOccurrenceOf ("<table class=\"tracks\" cellspacing=\"0\" cellpadding=\"4\">", true, false));
    trackInfo = trackInfo.upToFirstOccurrenceOf("</table>", true, false);
    
    ScopedPointer<XmlElement> tracksXml (XmlDocument::parse (trackInfo));
    
    if (tracksXml != nullptr)
    {
        XmlElement* tracksElem (XmlHelpers::findXmlElementContainingSubText (tracksXml, trackName));
        
        if (tracksElem != nullptr)
        {
            XmlElement* nextElem = tracksElem->getNextElement();
            
            if (nextElem != nullptr)
            {
                XmlElement* keyElem = nextElem->getFirstChildElement();
                
                if (keyElem != nullptr) 
                    return keyElem->getAllSubText();
            }
        }
    }
    
    return String::empty;
}

/** Holds a ValueTree as a ReferenceCountedObject.
    This is somewhat obfuscated but makes it easy to transfer ValueTrees as var objects 
    such as when using them as DragAndDropTarget::SourceDetails::description members.
 */
class ReferenceCountedValueTree : public ReferenceCountedObject
{
public:
    
    ReferenceCountedValueTree (ValueTree treeToReference)
    :   tree (treeToReference)
    {
    }
    
    ~ReferenceCountedValueTree()
    {
    }
    
    void setValueTree (ValueTree newTree)
    {
        tree = newTree;
    }
    
    ValueTree getValueTree()
    {
        return tree;
    }
    
    typedef ReferenceCountedObjectPtr<ReferenceCountedValueTree> Ptr;
    
private:
    
    ValueTree tree;
};

//==============================================================================
/** Writes a ValueTree to a specified file.
    This is a helper method to conveniently write a ValueTree to a File,
    optionally storing it as Xml.
 */
static bool writeValueTreeToFile (ValueTree& treeToWrite, File& fileToWriteTo, bool asXml = true)
{
    if (fileToWriteTo.hasWriteAccess())
    {
        if (asXml) 
        {
            ScopedPointer<XmlElement> treeAsXml (treeToWrite.createXml());
            
            if (treeAsXml != nullptr)
                return treeAsXml->writeToFile (fileToWriteTo, String::empty, "UTF-8", 200);
            
            return false;
        }
        else 
        {
            TemporaryFile tempFile (fileToWriteTo);
            ScopedPointer <FileOutputStream> outputStream (tempFile.getFile().createOutputStream());
            
            if (outputStream != nullptr)
            {
                treeToWrite.writeToStream (*outputStream);
                outputStream = nullptr;
                
                return tempFile.overwriteTargetFileWithTemporary();
            }
        }
    }
    
    return false;
}

/** Reads a ValueTree from a stored file.
 
    This will first attempt to parse the file as Xml, if this fails it will
    attempt to read it as binary. If this also fails it will return an invalid
    ValueTree.
 */
static ValueTree readValueTreeFromFile (const File& fileToReadFrom)
{
    ScopedPointer<XmlElement> treeAsXml (XmlDocument::parse (fileToReadFrom));
    if (treeAsXml != nullptr)
    {
        return ValueTree::fromXml (*treeAsXml);
    }

    ScopedPointer<FileInputStream> fileInputStream (fileToReadFrom.createInputStream());
    if (fileInputStream != nullptr
        && fileInputStream->openedOk())
    {
        return ValueTree::readFromStream (*fileInputStream);
    }
    
    return ValueTree::invalid;
}

//==============================================================================
/** This handy macro is a platform independent way of stopping compiler
	warnings when paramaters are declared but not used.
 */
#ifndef UNUSED_NOWARN

	#if defined(JUCE_MAC) || defined(JUCE_IOS)
		// enable supression of unused variable is GCC
		#define UNUSED_NOWARN __attribute__((unused))

	#elif defined(JUCE_MSVC)
	#define UNUSED_NOWARN

		// disable unused variable warnings in MSVC (Windows)
		#pragma warning( push )
		#pragma warning( disable : 4705 )

	#else

	#define UNUSED_NOWARN

	#endif

#endif // #ifndef UNUSED_NOWARN

//==============================================================================
/** This is a platform independent way of aligning variables.
 */
#ifndef ALIGN_DATA

	#if defined(JUCE_MAC)		// gcc
		#define ALIGN_DATA(x) __attribute__ ((aligned (x)))

	#elif defined(JUCE_MSVC)	// MSVC
		#define ALIGN_DATA(x) __declspec (align(x))

	#else
		#define ALIGN_DATA(x)

	#endif

#endif // ALIGN_DATA

#endif //__DROWAUDIO_UTILITY_H__