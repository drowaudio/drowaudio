/*
 *  dRowAudio_Utillity.h
 *  dRowAudio
 *
 *  Created by David Rowland on 03/03/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIOUTILITY_H_
#define _DROWAUDIOUTILITY_H_

#include "../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_XmlHelpers.h"

//==============================================================================
/**
	@file
	This file contains some useful utility functions and macros.
 */
//==============================================================================

/**
	Checks to see if two values are equal within a given precision.
 */
inline static bool almostEqual(double firstValue, double secondValue, double precision =0.00001)
{
	if ( fabs(firstValue - secondValue) < precision )
		return true;
	else
		return false;
}

/**
	Returns the Resources folder in the package contents on a Mac and if an equivalent exists on Windows.
	This will return File::nonexistent if the file does not exist so check for this first.
 */
inline static File getResourcesFolder()
{
	return File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getChildFile("Resources");
}

/**
	If the String passed in is a local path, this will return a string with the file://localhost part
	of the file path stripped as well as the %20 spaces
 */
inline static String stripFileProtocolForLocal(String pathToStrip)
{
	if (pathToStrip.startsWith(T("file://localhost")));
	{
		String temp1(pathToStrip.substring(pathToStrip.indexOf(7, T("/"))));//(T("file://localhost")));
		String temp2(temp1.replace(T("%20"), T(" ")));

		return temp2;
	}
	
	return String::empty;
}

/**
    Converts an iTunes formatted date string (e.g. 2010-12-27T17:44:32Z)
    into a Time object.
 */
inline static Time parseITunesDateString(String dateString)
{
    int year = dateString.substring (0, 4).getIntValue();
    int month = dateString.substring (5, 7).getIntValue();
    int day = dateString.substring (8, 10).getIntValue();
    int hours = dateString.substring (11, 13).getIntValue();
    int minutes = dateString.substring (14, 16).getIntValue();
    int seconds = dateString.substring (17, 19).getIntValue();
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
void reverseArray(Type *array, int length)
{
    int a=0;
    Type swap;
	
    for(a; a < --length; a++) //increment a and decrement b until they meet eachother
    {
        swap = array[a];       //put what's in a into swap space
        array[a] = array[length];    //put what's in b into a
        array[length] = swap;       //put what's in the swap (a) into b
    }
}

/**	Reverses two arrays at once.
	This will be quicker than calling reverseArray twice.
	The arrays must be the same length.
 */
template <class Type>
void reverseTwoArrays(Type *array1, Type *array2, int length)
{
    Type swap;
    for(int a = 0; a < --length; a++) //increment a and decrement b until they meet eachother
    {
        swap = array1[a];       //put what's in a into swap space
        array1[a] = array1[length];    //put what's in b into a
        array1[length] = swap;       //put what's in the swap (a) into b

        swap = array2[a];       //put what's in a into swap space
        array2[a] = array2[length];    //put what's in b into a
        array2[length] = swap;       //put what's in the swap (a) into b
    }
}

static const String dRowReadEntireTextStream (URL *url, const bool usePostCommand)
{
    const ScopedPointer <InputStream> in (url->createInputStream (usePostCommand,
																  0, 0, String::empty,
																  2000));
	
    if (in != 0)
        return in->readEntireStreamAsString();
	
    return String::empty;
}

static XmlElement* dRowReadEntireXmlStream (URL *url, const bool usePostCommand)
{
    return XmlDocument::parse (dRowReadEntireTextStream (url, usePostCommand));
}

/**	Finds the key for a given track from the chemical-records website.
	This will attempt to find the key listed on the chemical website for a given release number
	eg. "31R038" and track title eg. "Wait For Me".
	This is in the Mixed in Key format eg. 11A and will return an empty string if nothing could be found.
	
	@param	releaseNo	The catalogue number to look for.
	@param	trackName	The track name to look for.
	@param	retryLimit	An optional number of retries as sometimes the URL won't load first time.
 */
static String findKeyFromChemicalWebsite(const String &releaseNo, const String &trackName, int retryLimit =10)
{
	URL chemicalURL("http://www.chemical-records.co.uk/sc/servlet/Info?Track="+releaseNo);
	int attemptCount = 0;
	DBG(chemicalURL.toString(true));
	do
	{
		ScopedPointer<XmlElement> pageAsXML(chemicalURL.readEntireXmlStream());
//		ScopedPointer<XmlElement> pageAsXML(dRowReadEntireXmlStream(&chemicalURL, false));

		DBG(releaseNo<<" - "<<trackName<<" attempt: "<<attemptCount);

		if (pageAsXML != 0)
		{
			XmlElement *tracksXml (XmlHelpers::findXmlElementWithAttributeWithValue(pageAsXML, "class", "tracks"));
			
			if (tracksXml)
			{
				attemptCount = retryLimit;
				XmlElement *tracksElem (XmlHelpers::findXmlElementWithSubText(tracksXml, trackName));
				
				if (tracksElem)
				{
					XmlElement *keyElem = 0;
					if (tracksElem->getNextElement() != 0)
						keyElem = (tracksElem->getNextElement()->getFirstChildElement());
					
					if (keyElem != 0) {
						return keyElem->getAllSubText();
					}
					else {
						return String::empty;
					}
				}
			}
		}
	} while (++attemptCount < retryLimit);
	
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
/**
	This handy macro is a platform independent way of stopping compiler
	warnings when paramaters are declared but not used.
 */
#ifndef UNUSED_NOWARN

	#if defined(JUCE_MAC)
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
/**
	This is a platform independent way of aligning variables.
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
//==============================================================================


#endif //_DROWAUDIOUTILITY_H_