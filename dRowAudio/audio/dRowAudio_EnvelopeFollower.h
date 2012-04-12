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

#ifndef __DROWAUDIO_ENVELOPEFOLLOWER_H__
#define __DROWAUDIO_ENVELOPEFOLLOWER_H__

#include "filters/dRowAudio_OnePoleFilter.h"

//==============================================================================
/**
    EnvelopeFollower.
    
    Envelope follower class that gives an overall amplitude response of a set of
    samples.
 */
class EnvelopeFollower
{
public:
    //==============================================================================
	/** Constructor. */
	EnvelopeFollower();

	/** Destructor. */
	~EnvelopeFollower();
	
    //==============================================================================
	/** Uses different exponential attack and release coefficients.
		Call setTimes to setup this method, ignoring the hold time.
	 */
	void processEnvelope (const float* inputBuffer, float* outputBuffer, int numSamples) noexcept;

	/** Sets the times for the vaious stages of the envelope.
        1 is an instant attack/release, 0 ill never change the value.
     */
	void setCoefficients (float attack, float release) noexcept;
	
private:
    //==============================================================================
	float envelope;
	float envAttack, envRelease;

    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeFollower);
};

#endif // __DROWAUDIO_ENVELOPEFOLLOWER_H__