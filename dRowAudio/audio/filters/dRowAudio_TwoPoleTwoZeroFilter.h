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

#ifndef _DROWAUDIO_TWOPOLETWOZEROFILTER__H_
#define _DROWAUDIO_TWOPOLETWOZEROFILTER__H_

#include "../../core/dRowAudio_StandardHeader.h"

#include "../../utility/dRowAudio_Constants.h"

//==============================================================================
/**	Two-Pole, Two-Zero Filter
 
	This is a simple filter that uses one pole and one zero. As such it is very
	computationaly efficient especially if used to process a buffer of samples.
 */
class TwoPoleTwoZeroFilter
{
public:
	
	/**	Create an unititialised filter.
		This will not perform any filtering yet, call a make... method
		to turn it into that particular type of filter.
	 */
	TwoPoleTwoZeroFilter() throw();
	
	/// Destructor
	~TwoPoleTwoZeroFilter() throw();
	
	/**	Process a number of samples in one go.
		This is the most effecient method of filtering.
		Note that the samples passed to it actually get changed.
	 */
	void processSamples(float* const pfSamples,
						const int numSamples) throw();
	
	/**	Process a single sample.
		Less efficient method but leaves the sample unchanged,
		returning a filterd copy of it.
	 */
	inline float processSingleSample(const float sampleToProcess) throw()
	{
		float out = (a0 * sampleToProcess) + (a1 * x1) + (a2 * x2) + (b1 * y1) + (b2 * y2);
		x2 = x1;
		x1 = sampleToProcess;
		y2 = y1;
		y1 = out;
		
		return out;
	}
	
	/**	Turns the filter into a Low-pass Butterworth filter. */
	void makeLowPassButterworth(const double sampleRate, const double frequency) throw();
	
	/**	Turns the filter into a High-pass Butterworth filter. */
	void makeHighPassButterworth(const double sampleRate, const double frequency) throw();

	/**	Turns the filter into a Low-pass Critically-Damped filter. */
	void makeLowPassCriticallyDamped(const double sampleRate, const double frequency) throw();
	
	/**	Turns the filter into a High-pass Critically-Damped filter. */
	void makeHighPassCriticallyDamped(const double sampleRate, const double frequency) throw();

	/**	Turns the filter into a Low-pass Bessel filter. */
	void makeLowPassBessel(const double sampleRate, const double frequency) throw();
	
	/**	Turns the filter into a High-pass Bessel filter. */
	void makeHighPassBessel(const double sampleRate, const double frequency) throw();
	
	
private:

	float x1, x2, y1, y2, a0, a1, a2, b1, b2;
	
	JUCE_LEAK_DETECTOR (TwoPoleTwoZeroFilter);
};

#endif //_DROWAUDIO_TWOPOLETWOZEROFILTER__H_