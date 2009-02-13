/*
 *  CustomFilter.cpp
 *  Practical5
 *
 *  Created by David Rowland on 11/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudioIIRFilter.h"

void CustomFilter::createLowPass(const double sampleRate,
								 const double frequency)
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

void CustomFilter::createHighPass(const double sampleRate,
								  const double frequency)
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

void CustomFilter::createBandPass(const double sampleRate,
								  const double frequency,
								  const double qFactor)
{
	double oneOverCurrentSampleRate = 1/sampleRate; 
	float bandwidth = qFactor;
		
//	bandwidth = jlimit(0.01f, 0.49f, bandwidth);
	
	float w0 = 2.0f * float_Pi * frequency * oneOverCurrentSampleRate;
	float cos_w0 = cos(w0);
	float sin_w0 = sin(w0);
	float alpha = sin_w0 * sinh( (log(2.0)/2.0) * bandwidth * w0/sin_w0 );
	setCoefficients(alpha, 
					0.0f, 
					-alpha, 
					1.0f + alpha, 
					-2.0f * cos_w0, 
					1.0f - alpha);
}