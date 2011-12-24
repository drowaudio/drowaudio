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

#ifndef _DROWAUDIO_INTERPOLATINGALLPASSFILTER_H_
#define _DROWAUDIO_INTERPOLATINGALLPASSFILTER_H_

#include "../../core/dRowAudio_StandardHeader.h"

#define BUFFERSIZE 4096

/**
	 Interpolating, Allpass Comb Filter.
	 
	 This allpass filter is a feedback and feedforward comb filter in series.
	 It has a unity frequency response but a complex phase response,typically
	 delaying sharp transients.
	 
	 This filter also uses linear interolation to allow fractional buffer positions.
	 This means the delay time can be swept to give modulating effects such as flangers.
 */
class InterpolatingAllpassFilter
{
public:
	
	/** Creates a default filter ready to be used. 
	 */
	InterpolatingAllpassFilter() throw();
	
	/// Destructor
	~InterpolatingAllpassFilter() throw();
	
	/** Sets the feedback coefficient.
		This needs to be kept below 1 for stability.
		The higher the value, the longer the delay will last.
	 */	
	void setGain(float newGain) throw();

	///	Sets the time the samples are delayed for.
	void setDelayTime(double sampleRate, float newDelayTime) throw();
	
	/// Processes a single sample return a new, filtered value.
	float processSingleSample(float newSample) throw();
	
	/// Processes an array of samples which are modified.
	void processSamples (float* const samples,
						 const int numSamples) throw();
	
private:
	
	CriticalSection processLock;
	
	float* delayRegister;
	int registerSize, registerSizeMask;
	float delayTime, delaySamples, gain;
	int bufferWritePos;
	float bufferReadPos;
	
	JUCE_LEAK_DETECTOR (InterpolatingAllpassFilter);
};

#endif //_DROWAUDIO_INTERPOLATINGALLPASSFILTER_H_