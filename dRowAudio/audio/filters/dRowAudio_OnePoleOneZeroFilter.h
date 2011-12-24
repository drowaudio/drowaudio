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

#ifndef _ONEPOLEONEZEROFILTER_H_
#define _ONEPOLEONEZEROFILTER_H_

#include "../../core/dRowAudio_StandardHeader.h"

#include "../../utility/dRowAudio_Constants.h"

//==============================================================================
/**	One-Pole, One-Zero Filter
 
	This is a simple filter that uses one pole and one zero. As such it is very
	computationaly efficient especially if used to process a buffer of samples.
 */
class OnePoleOneZeroFilter
{
public:
	
	/**	Create an unititialised filter.
		This will not perform any filtering yet, call a make... method
		to turn it into that particular type of filter.
	 */
	OnePoleOneZeroFilter() throw();
	
	/// Destructor
	~OnePoleOneZeroFilter() throw();
	
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
	forcedinline float processSingleSample(const float sampleToProcess) throw()
	{
		y1 = (sampleToProcess * a0) + (x1 * a1) + (y1 * b1);
		x1 = sampleToProcess;
		
		return y1;
	}
	
	/**	Turns the filter into a Low-pass.
	 */
	void makeLowPass(const double sampleRate,
					 const double frequency) throw();
	/**	Turns the filter into a High-pass.
	 */
	void makeHighPass(const double sampleRate,
					  const double frequency) throw();

private:
	CriticalSection lock;
	
	float x1, y1, a0, a1, b1;
	
	JUCE_LEAK_DETECTOR (OnePoleOneZeroFilter);
};

#endif // _ONEPOLEONEZEROFILTER_H_