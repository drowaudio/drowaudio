/*
 *  dRowAudio_Utillity.h
 *  dRowAudio
 *
 *  Created by David Rowland on 03/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWAUDIOUTILITY_H_
#define _DROWAUDIOUTILITY_H_

#include <juce/juce.h>

//==============================================================================
/**
	This file contains some useful utility functions and macros.
 */
//==============================================================================

/**
	Checks to see if two values are equal within a given precision.
 */
inline bool almostEqual(double firstValue, double secondValue, double precision =0.00001)
{
	if ( fabs(firstValue - secondValue) < precision )
		return true;
	else
		return false;
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

#endif //_DROWAUDIOUTILITY_H_