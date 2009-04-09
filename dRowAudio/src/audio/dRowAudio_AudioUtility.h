/*
 *  dRowAudio_AudioUtility.h
 *  dRowAudio
 *
 *  Created by David Rowland on 25/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWAUDIOAUDIOUTILITY_H_
#define _DROWAUDIOAUDIOUTILITY_H_

#include <juce/juce.h>
#include "../utility/dRowAudio_Constants.h"

//==============================================================================
/**
 This file contains some useful utility functions and macros.
 */
//==============================================================================

/**
	Converts a time in seconds to minutes.
 */
forcedinline static double minsToSeconds(double minutes)
{	return minutes / 60.0; }

/**
	Converts a time in seconds to a timecode string.
 */
static const String timeToTimecodeString (const double seconds)
{
    const double absSecs = fabs (seconds);
    const tchar* const sign = (seconds < 0) ? T("-") : T("");
	
    const int hours = (int) (absSecs / (60.0 * 60.0));
    const int mins  = ((int) (absSecs / 60.0)) % 60;
    const int secs  = ((int) absSecs) % 60;
	
    return String::formatted (T("%s%02d:%02d:%02d:%03d"),
                              sign, hours, mins, secs,
                              roundDoubleToInt (absSecs * 1000) % 1000);
}

/**
 Converts a time in seconds to a timecode string displaying mins, secs and 1/10th secs.
 */
static const String timeToTimecodeStringLowRes (const double seconds)
{
    const double absSecs = fabs (seconds);
    const tchar* const sign = (seconds < 0) ? T("-") : T("");
	
    const int hours = (int) (absSecs * oneOver60Squared);
    const int mins  = ((int) (absSecs * oneOver60)) % 60;
    const int secs  = ((int) absSecs) % 60;
	const int tenthSecs  = (int) ((absSecs - (int)absSecs) * 10);
	
    return String::formatted (T("%s%02d:%02d.%i"),
                              sign, mins, secs, tenthSecs);
}

/**
	Formats a CurretPositionInfo to a bars/beats string.
 */
static const String ppqToBarsBeatsString (const double ppq,
                                          const double lastBarPPQ,
                                          const int numerator,
                                          const int denominator)
{
    if (numerator == 0 || denominator == 0)
        return T("1|1|0");
	
    const int ppqPerBar = (numerator * 4 / denominator);
    const double beats  = (fmod (ppq, ppqPerBar) / ppqPerBar) * numerator;
	
    const int bar       = ((int) ppq) / ppqPerBar + 1;
    const int beat      = ((int) beats) + 1;
    const int ticks     = ((int) (fmod (beats, 1.0) * 960.0));
	
    String s;
    s << bar << T('|') << beat << T('|') << ticks;
    return s;
}

/**
	Compares a filename extension with a wildcard string.
 */
static bool matchesAudioWildcard (const String &extensionToTest, const String &wildcard, const bool ignoreCase)
{
	if (ignoreCase	?	wildcard.containsIgnoreCase(extensionToTest)
					:	wildcard.contains(extensionToTest))
		return true;
	else
		return false;
}

#endif //_DROWAUDIOAUDIOUTILITY_H_