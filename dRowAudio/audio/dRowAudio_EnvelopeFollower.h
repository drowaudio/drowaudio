/*
 *  dRowAudio_EnvelopeFollower.h
 *  dRowAudio
 *
 *  Created by David Rowland on 16/09/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_ENVELOPEFOLLOWER__H_
#define _DROWAUDIO_ENVELOPEFOLLOWER__H_

#include "../core/dRowAudio_StandardHeader.h"

#include "filters/dRowAudio_OnePoleFilter.h"

/**
	Envelope follower class that gives an overall amplitude response of a set of samples.
 
	This can be used in three different modes for slightly different envelope results. Depending
	on which one is used, a different set-up method will need to be called.
	For each process method you will need to supply the input sample data in inputBuffer,
	a buffer of the same size to store the envelope data, outputBuffer, and the number of samples.
 */
class EnvelopeFollower
{
public:
	
	/** Constructor. */
	EnvelopeFollower();

	/** Destructor. */
	~EnvelopeFollower();
	
	/** The simplest and most efficient method.
		This uses a second order lowpass filter. Call setCutoffFrequency to setup this method.
	 */
	void processEnvelope (float *inputBuffer, float *outputBuffer, int numSamples) throw();

	/** Uses different exponential attack and release coefficients.
		Call setTimes to setup this method, ignoring the hold time.
	 */
	void processEnvelopeExp (float *inputBuffer, float *outputBuffer, int numSamples) throw();
	
	/**	Uses different exponential attack and release coefficients along with a hold time.
		This is similar to the exponential method except an optional hold time can be used to
		keep the envelope at its current level between attack and release stages.
		Call setTimes to setup this method.
	 */
	void processEnvelopeExpHold (float *inputBuffer, float *outputBuffer, int numSamples) throw();

	/** Sets the cutoff frequency for the processEnvelope() method.
		Lower frequencies will give smoother but less accurate envelopes.
	 */
	void setCutoffFrequency (float sampleRate, float frequency) throw();
	
	/** Returns the cuttof frequency currently being used. */
	float getCutoffFrequency() throw() {	return cutoffFrequency;	}
	
	/** Sets the times for the vaious stages of the exponential methods. */
	void setTimes(float sampleRate, float attack, float release, float hold =0.0f) throw();
	
	 
private:
	
	OnePoleFilter filter0, filter1;
	double cutoffFrequency;
	
	float envelope;
	float envAttack, envRelease;
	int holdCount, holdLimit;
	bool hold;
	
	JUCE_LEAK_DETECTOR (EnvelopeFollower);
};

#endif //_DROWAUDIO_ENVELOPEFOLLOWER__H_