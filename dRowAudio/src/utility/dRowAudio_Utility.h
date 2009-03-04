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

/**
 This file contains some useful utility functions.
 */

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

#endif //_DROWAUDIOUTILITY_H_