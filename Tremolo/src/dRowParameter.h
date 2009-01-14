/*
 *  dRowParameter.h
 *  dRow_Tremolo
 *
 *  Created by David Rowland on 09/01/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef DROWPARAMETER_H
#define DROWPARAMETER_H

#include <juce/juce.h>

/**	This file defines a parameter used in an application.
	Both full-scale and normalised values must be present for
	AU and VST host campatability.
 */
class dRowParameter
{
	public:
//	dRowParameter();

	dRowParameter(const String& name_, String unit_, String description_,
				  double value_, double min_ =0.0f, double max_ =1.0f, double default_ =0.0f,
				  double scale_ =1, double offset_=0);
		
	void init(const String& name_, String unit_, String description_,
			  double value_, double min_ =0.0f, double max_ =1.0f, double default_ =0.0f,
			  double scale_ =1, double offset_ =0);

	double getValue();
	double getNormalisedValue();
	double normaliseValue(double scaledValue);
	double getMin();
	double getMax();
	double getDefault();
	double getScale();
	double getOffset();
	
	// the return of these can be used to see if the parameter has changed
	bool setValue(double value_);
	bool setNormalisedValue(double nvalue);
	
	private:
	String name, unit, description;
	double value, min, max, defaultValue, scale, offset;
};

#endif //DROWPARAMETER_H