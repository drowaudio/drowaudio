/*
 *  PluginParameter.cpp
 *
 *  Created by David Rowland on 09/01/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

PluginParameter::PluginParameter()
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

void PluginParameter::init(const String& name_, ParameterUnit unit_, String description_,
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
		case UnitPercent:       setUnitSuffix("%");     break;
		case UnitSeconds:       setUnitSuffix("s");     break;
		case UnitPhase:         setUnitSuffix("°");     break;
		case UnitHertz:         setUnitSuffix("Hz");    break;
		case UnitDecibels:      setUnitSuffix("dB");    break;
		case UnitDegrees:       setUnitSuffix("°");     break;
		case UnitMeters:        setUnitSuffix("m");     break;
		case UnitBPM:           setUnitSuffix("BPM");   break;
		case UnitMilliseconds:  setUnitSuffix("ms");    break;
		default:                                        break;
	}	
}

double PluginParameter::getValue()
{
	return value;
}

double PluginParameter::getNormalisedValue()
{
	return normaliseValue(value);
}

double PluginParameter::getSmoothedValue()
{
	return smoothValue;
}

double PluginParameter::getSmoothedNormalisedValue()
{
	return normaliseValue(smoothValue);
}

double PluginParameter::normaliseValue(double scaledValue)
{
	return ((scaledValue - min) / (max-min));
}

double PluginParameter::getMin()
{
	return min;
}
double PluginParameter::getMax()
{
	return max;
}
double PluginParameter::getDefault()
{
	return defaultValue;
}

bool PluginParameter::setValue(double value_)
{
	double newValue = jlimit(min, max, value_);
	
	if(newValue == value) 
    {
		return false;
    }
	else
    {
		value = newValue;
		return true;
	}
}

bool PluginParameter::setNormalisedValue(double nvalue)
{
	return setValue((max-min) * jlimit(0.0, 1.0, nvalue) + min);
}

ParameterUnit PluginParameter::getUnit()
{
	return unit;
}

const String PluginParameter::getUnitSuffix()
{
	return unitSuffix;
}
void PluginParameter::setUnitSuffix(String newSuffix)
{
	unitSuffix = newSuffix;
}

void PluginParameter::smooth()
{
	if(smoothValue != value)
	{
		if( (smoothCoeff == 1.0) || almostEqual(smoothValue, value) )
			smoothValue = value; 
		else
			smoothValue = ((value - smoothValue) * smoothCoeff) + smoothValue;
	}
}

void PluginParameter::setSmoothCoeff(double newSmoothCoef)
{
	smoothCoeff = newSmoothCoef;
}
double PluginParameter::getSmoothCoeff()
{
	return smoothCoeff;
}

void PluginParameter::setSkewFactor(double newSkewFactor)
{
	skewFactor = newSkewFactor;
}
void PluginParameter::setSkewFactorFromMidPoint(const double valueToShowAtMidPoint)
{
	if (max > min)
        skewFactor = log (0.5) / log ((valueToShowAtMidPoint - min) / (max - min));
}
double PluginParameter::getSkewFactor()
{
	return skewFactor;
}

void PluginParameter::setStep(double newStep)
{
	step = newStep;
}
double PluginParameter::getStep()
{
	return step;
}

void PluginParameter::writeXml(XmlElement& xmlState)
{
	xmlState.setAttribute(name,	value);
}

void PluginParameter::readXml(const XmlElement* xmlState)
{
	setValue(xmlState->getDoubleAttribute(name, value));
}

void PluginParameter::setupSlider(Slider &slider)
{
		slider.setRange             (min, max, step);
		slider.setSkewFactor        (skewFactor);
		slider.setValue             (value, false);
		slider.setTextValueSuffix   (unitSuffix);
}

END_JUCE_NAMESPACE