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

#ifndef __DROWAUDIO_ONEPOLEFILTER_H__
#define __DROWAUDIO_ONEPOLEFILTER_H__

//==============================================================================
/**
    One-Pole Filter.
 
	This is a simple filter that uses only one pole. As such it is very
	computationaly efficient especially if used to process a buffer of samples.
	It has a slope of -6dB/octave.
 */
class OnePoleFilter
{
public:
    //==============================================================================
	/**	Create an unititialised filter.
		This will not perform any filtering yet, call a make... method
		to turn it into that particular type of filter.
	 */
	OnePoleFilter() noexcept;
	
	/** Destructor.
     */
	~OnePoleFilter() noexcept;
	
	/**	Process a number of samples in one go.
		This is the most effecient method of filtering.
		Note that the samples passed to it actually get changed.
	 */
	void processSamples (float* const samples,
						 const int numSamples) noexcept;
	
	/**	Process a single sample.
		Less efficient method but leaves the sample unchanged,
		returning a filtered copy of it.
	 */
	inline float processSingleSample (const float sampleToProcess) noexcept
	{
		return y1 = (b0 * sampleToProcess) + (a1 * y1);
	}
	
	/**	Turns the filter into a Low-pass.
	 */
	void makeLowPass (const double sampleRate,
					  const double frequency) noexcept;
	/**	Turns the filter into a High-pass.
	 */
	void makeHighPass (const double sampleRate,
					   const double frequency) noexcept;

private:
    //==============================================================================
	CriticalSection lock;
	float y1, b0, a1;
	
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnePoleFilter);
};

#endif // __DROWAUDIO_ONEPOLEFILTER_H__