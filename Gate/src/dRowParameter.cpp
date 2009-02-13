/*
 *  dRowParameter.cpp
 *
 *  Created by David Rowland on 09/01/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowParameter.h"

//dRowParameter::dRowParameter()
//{
//}

dRowParameter::dRowParameter(const String& name_, ParameterUnit unit_, String description_,
							 double value_, double min_, double max_, double default_)
{
	name = name_;
	unit = unit_;
	description = description_;
	
	value = value_;
	min = min_;
	max = max_;
	defaultValue = default_;
	
	smoothCoeff = 0.1f;
	smoothValue = value;
}

void dRowParameter::init(const String& name_, ParameterUnit unit_, String description_,
						 double value_, double min_, double max_, double default_)
{
	name = name_;
	unit = unit_;
	description = description_;
	
	value = value_;
	min = min_;
	max = max_;
	defaultValue = default_;
	
	smoothCoeff = 0.1f;
	smoothValue = value;
}

double dRowParameter::getValue()
{
	return value;
}

double dRowParameter::getNormalisedValue()
{
	return normaliseValue(value);
}

double dRowParameter::getSmoothedValue()
{
	return smoothValue;
}

double dRowParameter::getSmoothedNormalisedValue()
{
	return normaliseValue(smoothValue);
}

double dRowParameter::normaliseValue(double scaledValue)
{
	return ((scaledValue - min) / (max-min));
}

double dRowParameter::getMin()
{
	return min;
}
double dRowParameter::getMax()
{
	return max;
}
double dRowParameter::getDefault()
{
	return defaultValue;
}

bool dRowParameter::setValue(double value_)
{
	double newValue = jlimit(min, max, value_);
	
	if(newValue == value) 
		return false;
	else {
		value = newValue;
		return true;
	}
}

bool dRowParameter::setNormalisedValue(double nvalue)
{
	return setValue((max-min) * jlimit(0.0, 1.0, nvalue) + min);
}

ParameterUnit dRowParameter::getUnit()
{
	return unit;
}

void dRowParameter::smooth()
{
	if(smoothValue != value)
	{
		//if( (smoothCoeff == 1.0) || (abs(smoothValue-value) < 0.0001) )
		if( (smoothCoeff == 1.0) )
			smoothValue = value; 
		else
			smoothValue = ((value - smoothValue) * smoothCoeff) + smoothValue;
	}
}