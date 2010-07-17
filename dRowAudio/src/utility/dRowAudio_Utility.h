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

#include <juce/juce.h>

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