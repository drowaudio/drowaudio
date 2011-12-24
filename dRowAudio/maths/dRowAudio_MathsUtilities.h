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

#ifndef __DROWAUDIO_MATHSUTILITIES_H__
#define __DROWAUDIO_MATHSUTILITIES_H__

/**	Linear Interpolater.
	Performs a linear interpolation for a fractional buffer position.
	Note: For speed no bounds checking is performed on the buffer position so it is
	up to the caller to mae sure it is less than the buffer size or you will be
	overwritting random memory.
 */
inline float linearInterpolate (float* buffer, int bufferSize, float bufferPosition)
{
	int iPos1 = (int)bufferPosition;
	int iPos2 = iPos1 + 1;
	if (iPos2 == bufferSize)
		iPos2 = 0;
	float fDiff = bufferPosition - iPos1;		

	float fOutput = (buffer[iPos2] * fDiff) + (buffer[iPos1] * (1.0f - fDiff));
	return fOutput;
}

/**	Sinc function.
 */
inline double sinc(const double x)
{
    if (x==0)
        return 1;
    
    return sin (x) / x;
}

/**	Sinc function normalised with PI for audio applications.
 */
inline double sincPi(const double x)
{
    if (x==0)
        return 1;
    
    return sin (double_Pi * x) / (double_Pi * x);
}

/** Converts a number of degrees to radians
 */
inline double degreesToRadians (const double degrees)
{
    return (degrees / 180.0) * double_Pi;
}

/**	Returns true if the argument is a power of 2.
    This will return false if 0 is passed.
 */
inline bool isPowerOfTwo (int number)
{
	return (number) && ! (number & (number - 1));
}

/**	Returns the next power of 2 of the given number.
 */
inline int nextPowerOf2 (int number)
{
	if (isPowerOfTwo (number))
		return number;
	else
		return pow (2, ceil (log ((double) number) / log(2.0)));
}

/**	Returns the previous power of 2.
    This may return 0 if a number < 1 is passed.
 */
inline int prevPowerOf2(int number)
{
	if (isPowerOfTwo (number))
		return number;
	else
		return pow (2, ceil (log ((double) number) / log (2.0))) * 0.5;
}

/**	Returns the power which 2 has to be raised to to get the given number.
    If the given number is not an exact power of 2 the next nearest power will be given.
    E.g. 1024 will return 10 as will 1023.
 */
inline int findPowerForBase2(int number)
{
	if (isPowerOfTwo (number))
		return log ((double) number) / log(2.0);
	else
		return log ((double) nextPowerOf2 ( number)) / log(2.0);
}

#endif //__DROWAUDIO_MATHSUTILITIES_H__