/*
 *  dRowAudio_BiquadFilter.cpp
 *
 *  Created by David Rowland on 11/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "dRowAudio_BiquadFilter.h"

void BiquadFilter::makeLowPass(const double sampleRate,
								 const double frequency) throw()
{
	double oneOverCurrentSampleRate = 1/sampleRate; 
	float w0 = 2.0f * float_Pi * frequency * oneOverCurrentSampleRate;
	float cos_w0 = cos(w0);
	float sin_w0 = sin(w0);
	float alpha = sin_w0 / (2.0f * 0.5f);
	setCoefficients((1.0f - cos_w0) * 0.5f, 
					(1.0f - cos_w0), 
					(1.0f - cos_w0) * 0.5f, 
					(1.0f + alpha), 
					-2.0f * cos_w0, 
					(1.0f - alpha));
}

void BiquadFilter::makeHighPass(const double sampleRate,
								  const double frequency) throw()
{
	double oneOverCurrentSampleRate = 1/sampleRate; 
	float w0 = 2.0f * float_Pi * frequency * oneOverCurrentSampleRate;
	float cos_w0 = cos(w0);
	float sin_w0 = sin(w0);
	float alpha = sin_w0 / (2.0f * 0.5f);
	setCoefficients((1.0f + cos_w0) * 0.5f, 
					-(1.0f + cos_w0), 
					(1.0f + cos_w0) * 0.5f, 
					(1.0f + alpha), 
					-2.0f * cos_w0, 
					(1.0f - alpha));	
}

void BiquadFilter::makeBandPass(const double sampleRate,
								  const double frequency,
								  const double Q) throw()
{
	const double qFactor = jlimit(0.00001, 1000.0, Q);
	const double oneOverCurrentSampleRate = 1/sampleRate;
		
	
	float w0 = 2.0f * float_Pi * frequency * oneOverCurrentSampleRate;
	float cos_w0 = cos(w0);
	float sin_w0 = sin(w0);
	float alpha = sin_w0/(2*qFactor);
//	float alpha = sin_w0 * sinh( (log(2.0)/2.0) * bandwidth * w0/sin_w0 );
	
	setCoefficients(alpha, 
					0.0f, 
					-alpha, 
					1.0f + alpha, 
					-2.0f * cos_w0, 
					1.0f - alpha);
}

void BiquadFilter::makeBandStop(const double sampleRate,
								  const double frequency,
								  const double Q) throw()
{
	const double qFactor = jlimit(0.00001, 1000.0, Q);
	const double oneOverCurrentSampleRate = 1/sampleRate;
	
	
	float w0 = 2.0f * float_Pi * frequency * oneOverCurrentSampleRate;
	float cos_w0 = cos(w0);
	float sin_w0 = sin(w0);
	float alpha = sin_w0/(2*qFactor);
	
	setCoefficients(1.0f, 
					-2*cos_w0, 
					1.0f, 
					1.0f + alpha, 
					-2.0f * cos_w0, 
					1.0f - alpha);
}

void BiquadFilter::makePeakNotch (const double sampleRate,
								  const double centreFrequency,
								  const double Q,
								  const float gainFactor) throw()
{
    jassert (sampleRate > 0);
    jassert (Q > 0);
	
    const double A = jmax (0.0f, gainFactor);
    const double omega = (double_Pi * 2.0 * jmax (centreFrequency, 2.0)) / sampleRate;
    const double alpha = 0.5 * sin (omega) / Q;
    const double c2 = -2.0 * cos (omega);
    const double alphaTimesA = alpha * A;
    const double alphaOverA = alpha / A;
	
    setCoefficients (1.0 + alphaTimesA,
                     c2,
                     1.0 - alphaTimesA,
                     1.0 + alphaOverA,
                     c2,
                     1.0 - alphaOverA);
}

void BiquadFilter::makeAllpass(const double sampleRate,
								 const double frequency,
								 const double Q) throw()
{
	const double qFactor = jlimit(0.00001, 1000.0, Q);
	const double oneOverCurrentSampleRate = 1/sampleRate;
	
	
	float w0 = 2.0f * float_Pi * frequency * oneOverCurrentSampleRate;
	float cos_w0 = cos(w0);
	float sin_w0 = sin(w0);
	float alpha = sin_w0/(2*qFactor);
	
	setCoefficients(1.0f - alpha, 
					-2*cos_w0, 
					1.0f + alpha, 
					1.0f + alpha, 
					-2.0f * cos_w0, 
					1.0f - alpha);
}