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

#ifndef __DROWAUDIO_AUDIOUTILITY_H__
#define __DROWAUDIO_AUDIOUTILITY_H__

#include "../utility/dRowAudio_Constants.h"

//==============================================================================
/** This file contains some useful utility functions and macros for audio
    applications.
 */
//==============================================================================
/** Converts an absolute value to decibels.
 */
forcedinline static double toDecibels (double absoluteValue)
{
    return 20.0 * log10 (absoluteValue);
}

/** Converts a value in decibels to an absolute value.
 */
forcedinline static double decibelsToAbsolute (double decibelsValue)
{
    return pow (10, (decibelsValue * 0.05));
}

/** Converts a time in seconds to minutes.
 */
forcedinline static double secondsToMins (double seconds)
{
    return seconds * oneOver60;
}

/** Converts a time in milliseconds to a number of samples for a given sample rate.
 */
forcedinline static int msToSamples (float timeMs, double sampleRate)
{
    return (int) (timeMs * 0.001 * sampleRate);
}

/** Converts a number of samples to a time in ms for a given sample rate.
 */
forcedinline static double samplesToMs (int64 numSamples, double sampleRate)
{
    return (1000 * (numSamples / sampleRate));
}

/** Converts a number of samples to a time in seconds for a given sample rate.
 */
forcedinline static double samplesToSeconds (int64 numSamples, double sampleRate)
{
    return (numSamples / sampleRate);
}

/** Converts a number of semitones to a given pitch ratio.
 */
static inline double semitonesToPitchRatio (double numSemitones)  
{
    return pow (10.0, numSemitones * (log10 (2.0) / 12.0));
}

/** Converts pitch ratio to a number of semitones.
 */
static inline double pitchRatioToSemitones (double pitchRatio)    
{
    return (12.0 / log10 (2.0)) * log10 (pitchRatio);
}

/** Converts a frequency to MIDI note number.
 */
static inline double frequencyToMidi (double frequency)
{
    return 69.0 + (12.0 * log2 (frequency / 440.0));
}

/** Converts a MIDI note number to a frequency.
 */
static inline double midiToFrequency (double midiNoteNumber)
{
    return 440.0 * pow (2.0, (midiNoteNumber - 69.0) / 12.0);
}

/** Converts a time in seconds to a timecode string.
 */
static const String timeToTimecodeString (const double seconds)
{
    const double absSecs = fabs (seconds);
    const String sign ((seconds < 0) ? "-" : "");
	
    const int hours = (int) (absSecs / (60.0 * 60.0));
    const int mins  = ((int) (absSecs / 60.0)) % 60;
    const int secs  = ((int) absSecs) % 60;
	
	String t (sign);
	
    t	<< String (hours).paddedLeft ('0', 2) << ":"
		<< String (mins).paddedLeft ('0', 2) << ":"
		<< String (secs).paddedLeft ('0', 2) << ":"
		<< String (roundToInt (absSecs * 1000) % 1000).paddedLeft ('0', 2);
	return t;
}

/** Converts a time in seconds to a timecode string displaying mins, secs and 1/10th secs.
 */
static const String timeToTimecodeStringLowRes (const double seconds)
{
    const double absSecs = fabs (seconds);
    const String sign ((seconds < 0) ? "-" : "");
	
//    const int hours = (int) (absSecs * oneOver60Squared);
    const int mins  = ((uint32) (absSecs * oneOver60)) % 60u;
    const int secs  = ((uint32) absSecs) % 60u;
	const int tenthSecs  = (int) ((absSecs - (int) absSecs) * 10);
	
	String t (sign);
	
    t	<< String (mins).paddedLeft ('0', 2) << ":"
		<< String (secs).paddedLeft ('0', 2) << "."
		<< String (tenthSecs).paddedLeft ('0', 0);
	return t;
}

/**	Converts a number of seconds to a time string.
	This is useful when displaying times as hrs, mins secs etc.
	as it will only display the units needed.
 */
static const String secondsToTimeLength (double numSeconds)
{
	double decimalTime = numSeconds / 60000.0;
	
	int hrs = 0;
	int mins = (int) decimalTime;
	int secs = roundToInt ((decimalTime - mins) * 60.0);
	
	String timeString;
	
	if (mins > 59)
	{
		hrs = mins / 60;
		mins -= hrs * 60;
		
		timeString << String (hrs) << ":"
		<< String (mins).paddedLeft ('0', 2) << ":";
	}
	else
		timeString << String (mins) << ":";
	
	timeString << String (secs).paddedLeft ('0', 2);
	
	return timeString;
}

/** Formats a CurretPositionInfo to a bars/beats string.
 */
static const String ppqToBarsBeatsString (const double ppq,
                                          const double /*lastBarPPQ*/,
                                          const int numerator,
                                          const int denominator)
{
    if (numerator == 0 || denominator == 0)
        return "1|1|0";
	
    const int ppqPerBar = (numerator * 4 / denominator);
    const double beats  = (fmod (ppq, ppqPerBar) / ppqPerBar) * numerator;
	
    const int bar       = ((int) ppq) / ppqPerBar + 1;
    const int beat      = ((int) beats) + 1;
    const int ticks     = ((int) (fmod (beats, 1.0) * 960.0));
	
    String s;
    s << bar << '|' << beat << '|' << ticks;
    return s;
}

/** Compares a filename extension with a wildcard string.
 */
static bool matchesAudioWildcard (const String& extensionToTest, const String& wildcard, const bool ignoreCase=true)
{
	if (ignoreCase ? wildcard.containsIgnoreCase (extensionToTest)
                   : wildcard.contains (extensionToTest))
		return true;
	else
		return false;
}

/** Converts a block of audio sample to floating point samples if the reader
    used an integer format.
 */
static void convertToFloat (AudioFormatReader* reader, void* sourceBuffer, float* destBuffer, int numSamples)
{
	if (reader != nullptr)
	{
		if (! reader->usesFloatingPointData)
		{
#if JUCE_BIG_ENDIAN
			AudioDataConverters::convertInt32BEToFloat ((void*) sourceBuffer, destBuffer,
                                                        numSamples, sizeof (int));
#else
			AudioDataConverters::convertInt32LEToFloat ((void*) sourceBuffer, destBuffer,
                                                        numSamples, sizeof (int));
#endif
		}
		else
        {
			memcpy (destBuffer, sourceBuffer, sizeof (float) * numSamples);
		}
	}
}

#endif //__DROWAUDIO_AUDIOUTILITY_H__