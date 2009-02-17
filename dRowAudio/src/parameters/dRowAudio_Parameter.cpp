/*
 *  dRowParameter.cpp
 *
 *  Created by David Rowland on 09/01/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "dRowAudio_Parameter.h"

dRowParameter::dRowParameter()
{
	init("parameter",		// name
		 UnitGeneric,		// unit
		 "A parameter",		// description
		 1.0f,				// value
		 0.0f,				// min
		 1.0f,				// max
		 0.0f,				// default
		 1.0f,				// skew factor
		 0.1f,				// smooth coeff
		 0.001);			// step
	
}

void dRowParameter::init(const String& name_, ParameterUnit unit_, String description_,
						 double value_, double min_, double max_, double default_,
						 double skewFactor_, double smoothCoeff_, double step_)
{
	name = name_;
	unit = unit_;
	description = description_;
	
	value = value_;
	min = min_;
	max = max_;
	defaultValue = default_;
	
	smoothCoeff = smoothCoeff_;
	smoothValue = value;
	
	skewFactor = skewFactor_;
	step = step_;
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

void dRowParameter::setSmoothCoeff(double newSmoothCoef)
{
	smoothCoeff = newSmoothCoef;
}
double dRowParameter::getSmoothCoeff()
{
	return smoothCoeff;
}

void dRowParameter::setSkewFactor(double newSkewFactor)
{
	skewFactor = newSkewFactor;
}
double dRowParameter::getSkewFactor()
{
	return skewFactor;
}

void dRowParameter::setStep(double newStep)
{
	step = newStep;
}
double dRowParameter::getStep()
{
	return step;
}

void dRowParameter::writeXml(XmlElement& xmlState)
{
	xmlState.setAttribute(name,	value);
}

void dRowParameter::readXml(const XmlElement* xmlState)
{
	setValue(xmlState->getDoubleAttribute(name, value));
}
