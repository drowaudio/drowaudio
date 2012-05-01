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

#ifndef __DROWAUDIO_CONSTANTS_H__
#define __DROWAUDIO_CONSTANTS_H__

//==============================================================================
/** @file 
 
	This file contains some useful constants for calculations such as reciprocals
	to avoid using expensive divides in programs.
 */

static const double oneOver60 = 1.0 / 60.0;
static const double oneOver60Squared = 1.0 / (60.0 * 60.0);
static const double oneOver180 = 1.0 / 180.0;
static const double oneOverPi = 1.0 / double_Pi;
static const double twoTimesPi = 2.0 * double_Pi;
static const double fourTimesPi = 4.0 * double_Pi;
static const double sixTimesPi = 6.0 * double_Pi;
static const double root2 = sqrt (2.0);
static const double oneOverRoot2 = 1.0 / sqrt (2.0);
static const double root3 = sqrt (3.0);
static const double oneOverRoot3 = 1.0 / sqrt (3.0);

template <typename Type>
inline Type squareNumber (Type input)
{
    return input * input;
}

template <typename Type>
inline Type cubeNumber (Type input)
{
    return input * input * input;
}

//#define squareNumber(x)	(x * x)
//#define cubeNumber(x)	(x * x * x)

#if JUCE_WINDOWS
template <class Type>
inline Type log2 (Type input)
{
    return log (input) / log ((Type) 2);
}
#endif
#endif //__DROWAUDIO_CONSTANTS_H__