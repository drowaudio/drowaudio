/*
 *  dRowParameter.cpp
 *
 *  Created by David Rowland on 09/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
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
		 0.01,				// step
		 String::empty);	// unit suffix
}

void dRowParameter::init(const String& name_, ParameterUnit unit_, String description_,
						 double value_, double min_, double max_, double default_,
						 double skewFactor_, double smoothCoeff_, double step_, String unitSuffix_)
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
	
	unitSuffix = unitSuffix_;
	
	// default label suffix's, these can be changed later
	switch (unit)
	{
		case UnitPercent:
			setUnitSuffix(T("%"));
			break;
		case UnitSeconds:
			setUnitSuffix(T("s"));
			break;
		case UnitPhase:
			setUnitSuffix(T("°"));
			break;
		case UnitHertz:
			setUnitSuffix(T("Hz"));
			break;
		case UnitDecibels:
			setUnitSuffix(T("dB"));
			break;
		case UnitDegrees:
			setUnitSuffix(T("°"));
			break;
		case UnitMeters:
			setUnitSuffix(T("m"));
			break;
		case UnitBPM:
			setUnitSuffix(T("BPM"));
			break;
		case UnitMilliseconds:
			setUnitSuffix(T("ms"));
			break;
	}	
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

const String dRowParameter::getUnitSuffix()
{
	return unitSuffix;
}
void dRowParameter::setUnitSuffix(String newSuffix)
{
	unitSuffix = newSuffix;
}

void dRowParameter::smooth()
{
	if(smoothValue != value)
	{
		if( (smoothCoeff == 1.0) || almostEqual(smoothValue, value) )
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
void dRowParameter::setSkewFactorFromMidPoint(const double valueToShowAtMidPoint)
{
	if (max > min)
        skewFactor = log (0.5) / log ((valueToShowAtMidPoint - min) / (max - min));
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

void dRowParameter::setupSlider(Slider &slider)
{
		slider.setRange (min, max, step);
		slider.setSkewFactor (skewFactor);
		slider.setValue (value, false);
		slider.setTextValueSuffix (unitSuffix);
}