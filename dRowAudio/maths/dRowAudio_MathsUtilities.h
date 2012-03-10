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

#if JUCE_MSVC
    #pragma warning (disable: 4505)
#endif

//==============================================================================
/**	Linear Interpolater.
	Performs a linear interpolation for a fractional buffer position.
	Note: For speed no bounds checking is performed on the buffer position so it is
	up to the caller to make sure it is less than the buffer size or you will be
	reading random memory and probably get an audio blow-up.
 */
inline float linearInterpolate (const float* buffer, int bufferSize, float bufferPosition) noexcept
{
	int iPos1 = (int) bufferPosition;
	int iPos2 = iPos1 + 1;
	if (iPos2 == bufferSize)
		iPos2 = 0;
	float fDiff = bufferPosition - iPos1;		

	float fOutput = (buffer[iPos2] * fDiff) + (buffer[iPos1] * (1.0f - fDiff));
	return fOutput;
}

/** Checks to see if two values are equal within a given precision.
 */
inline static bool almostEqual (double firstValue, double secondValue, double precision = 0.00001)
{
	if (fabs (firstValue - secondValue) < precision)
		return true;
	else
		return false;
}

/** Checks to see if a number is Nan eg. sqrt (-1).
 */
template <typename Type>
inline static bool isnan (Type value)
{
#if JUCE_MAC
    return std::isnan (value);
#else
    volatile Type num = value;
    
    return num != num;
#endif
}

/**	Sinc function.
 */
inline double sinc (const double x) noexcept
{
    if (x == 0)
        return 1;
    
    return sin (x) / x;
}

/**	Sinc function normalised with PI for audio applications.
 */
inline double sincPi (const double x) noexcept
{
    if (x == 0)
        return 1;
    
    return sin (double_Pi * x) / (double_Pi * x);
}

/** Converts a number of degrees to radians
 */
inline double degreesToRadians (const double degrees) noexcept
{
    return (degrees / 180.0) * double_Pi;
}

/**	Returns true if the argument is a power of 2.
    This will return false if 0 is passed.
 */
inline bool isPowerOfTwo (int number) noexcept
{
	return (number) && ! (number & (number - 1));
}

/**	Returns the next power of 2 of the given number.
 */
inline int nextPowerOf2 (int number) noexcept
{
	if (isPowerOfTwo (number))
		return number;
	else
		return (int) pow (2.0, ceil (log ((double) number) / log (2.0)));
}

/**	Returns the previous power of 2.
    This may return 0 if a number < 1 is passed.
 */
inline int prevPowerOf2 (int number) noexcept
{
	if (isPowerOfTwo (number))
		return number;
	else
		return (int) (pow (2.0, ceil (log ((double) number) / log (2.0))) * 0.5);
}

/**	Returns the power which 2 has to be raised to to get the given number.
    If the given number is not an exact power of 2 the next nearest power will be given.
    E.g. 1024 will return 10 as will 1023.
 */
inline int findPowerForBase2 (int number) noexcept
{
	if (isPowerOfTwo (number))
		return (int) (log ((double) number) / log(2.0));
	else
		return (int) (log ((double) nextPowerOf2 (number)) / log(2.0));
}

//==============================================================================
//#if JUCE_UNIT_TESTS
//
//class MathsUnitTests  : public UnitTest
//{
//public:
//    MathsUnitTests() : UnitTest ("Maths Utilities") {}
//    
//    void runTest()
//    {
//        beginTest ("Maths Utilities");
//
//        expectEquals ((int) isnan (1), 0);
//        expectEquals ((int) isnan (sqrt (-1.0)), 1);
//    }
//};
//
//static MathsUnitTests mathsUnitTests;
//
//#endif

#endif //__DROWAUDIO_MATHSUTILITIES_H__