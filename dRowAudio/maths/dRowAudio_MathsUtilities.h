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
/** Finds the autocorrelation of a set of given samples.
 
    This will cross-correlate inputSamples with itself and put the result in
    output samples. Note that this uses a shrinking integration window, assuming
    values outside of numSamples are 0. This leads to an exponetially decreasing
    autocorrelation function.
 */
inline void autocorrelate (const float* inputSamples, int numSamples, float* outputSamples) noexcept
{
    for (int i = 0; i < numSamples; i++)
    {
        float sum = 0.0f;
        
        for (int j = 0; j < numSamples - i; j++)
            sum += inputSamples[j] * inputSamples[j + i];
        
        outputSamples[i] = sum * (1.0f / numSamples);
        //outputSamples[i] = sum;
    }
}

/** Finds the autocorrelation of a set of given samples using a
    square-difference function.
 
    This will cross-correlate inputSamples with itself and put the result in
    output samples. Note that this uses a shrinking integration window, assuming
    values outside of numSamples are 0. This leads to an exponetially decreasing
    autocorrelation function.
 */
inline void sdfAutocorrelate (const float* inputSamples, int numSamples, float* outputSamples) noexcept
{
    for (int i = 0; i < numSamples; i++)
    {
        float sum = 0.0f;
        
        for (int j = 0; j < numSamples - i; j++)
            sum += squareNumber (inputSamples[j] - inputSamples[j + i]);
            
        outputSamples[i] = sum;
    }
}

/** Performs a first order differential on the set of given samples.
 
    This is the same as finding the difference between each sample and the previous.
    Note that outputSamples can point to the same location as inputSamples.
 */
inline void differentiate (const float* inputSamples, int numSamples, float* outputSamples) noexcept
{
    float lastSample = 0.0f;
    
    for (int i = 0; i < numSamples; ++i)
    {
        float currentSample = inputSamples[i];
        outputSamples[i] = currentSample - lastSample;
        lastSample = currentSample;
    }
}

/** Finds the mean of a set of samples.
 */
inline float findMean (float* samples, int numSamples) noexcept
{
    float total = 0.0f;
    
    for (int i = 0; i < numSamples; ++i)
        total += samples[i];

    return total / numSamples;
}

/** Returns the median of a set of samples assuming they are sorted.
 */
inline float findMedian (float* samples, int numSamples) noexcept
{
    if ((numSamples % 2) == 0)  // is even
    {
        return samples[numSamples / 2];
    }
    else
    {
        const int lowerIndex = int (numSamples / 2);
        const float lowerSample = samples[lowerIndex];
        const float upperSample = samples[lowerIndex + 1];
        
        return (lowerSample + upperSample) / 2.0f;
    }
}

/** Finds the variance of a set of samples.
 */
inline float findVariance (float* samples, int numSamples) noexcept
{
    const float mean = findMean (samples, numSamples);

    float total = 0.0f;
    for (int i = 0; i < numSamples; ++i)
        total += squareNumber (samples[i] - mean);

    return total / numSamples;
}

/** Finds the corrected variance for a set of samples suitable for a sample standard deviation.
    Note the N - 1 in the formular to correct for small data sets.
 */
inline float findCorrectedVariance (float* samples, int numSamples) noexcept
{
    const float mean = findMean (samples, numSamples);
    
    float total = 0.0f;
    for (int i = 0; i < numSamples; ++i)
        total += squareNumber (samples[i] - mean);
    
    return total / (numSamples - 1);
}

/** Finds the sample standard deviation for a set of samples.
 */
inline float findStandardDeviation (float* samples, int numSamples) noexcept
{
    return sqrtf (findCorrectedVariance (samples, numSamples));
}

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

/** Normalises a value to a range of 0-1 with a given minimum & maximum.
    This is just a quick function to make more readable code and desn't do any error checking.
    If your value is outside the range you will get a normalised value < 0 or > 1.
 */
template <typename Type>
inline Type normalise (const Type valueToNormalise, const Type minimum, const Type maximum) noexcept
{
    return (valueToNormalise - minimum) / (maximum - minimum);
}

/** Scales a value to have a log range between a given minimum & maximum.
    This is just a quick function to make more readable code and desn't do any error checking.
    This is useful when scaling values for meters etc. A good starting point is a normalised
    input value, minimum of 1 and maximum of 40.
 */
template <typename Type>
inline Type logBase10Scale (const Type valueToScale, const Type minimum, const Type maximum) noexcept
{
    return log10 (minimum + ((maximum - minimum) * valueToScale)) / log10 (maximum);
}

/** Checks to see if a number is NaN eg. sqrt (-1).
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

/** Checks to see if a number is Inf eg. 100.0 / 0.0.
 */
template <typename Type>
inline static bool isinf (Type value)
{
#if ! JUCE_WINDOWS
    return std::isinf (value);
#else
    return ! _finite (value);
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

#if JUCE_MSVC || DOXYGEN
/** Log2 function for the MSVC compiler.
 */
inline double log2 (double number)
{
    return log (number) / log (2.0);
}

/** Log2f function for the MSVC compiler.
 */
inline float log2f (float number)
{
    return log (number) / log (2.0f);
}
#endif

#endif //__DROWAUDIO_MATHSUTILITIES_H__