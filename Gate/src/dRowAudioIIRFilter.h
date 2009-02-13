/*
 *  CustomFilter.h
 *  Practical5
 *
 *  Created by David Rowland on 11/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include <juce/juce.h>

class CustomFilter : public IIRFilter
{
public:
	
//	IIRFilter();
//	~IIRFilter();
	
	void createLowPass(const double sampleRate,
					   const double frequency);
	
	void createHighPass(const double sampleRate,
						const double frequency);
	
	void createBandPass(const double sampleRate,
						const double frequency,
						const double qFactor);
};