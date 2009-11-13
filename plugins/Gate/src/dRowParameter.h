/*
 *  dRowParameter.h
 *
 *  Created by David Rowland on 09/01/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef DROWPARAMETER_H
#define DROWPARAMETER_H

#include "includes.h"

/* parameter units - currently values are the same as the AudioUnit enums for this purpose */
enum ParameterUnit
{
	UnitGeneric				= 0,	/* untyped value generally between 0.0 and 1.0 */
	UnitIndexed				= 1,	/* takes an integer value (good for menu selections) */
	UnitBoolean				= 2,	/* 0.0 means FALSE, non-zero means TRUE */
	UnitPercent				= 3,	/* usually from 0 -> 100, sometimes -50 -> +50 */
	UnitSeconds				= 4,	/* absolute or relative time */
	UnitSampleFrames		= 5,	/* one sample frame equals (1.0/sampleRate) seconds */
	UnitPhase				= 6,	/* -180 to 180 degrees */
	UnitRate				= 7,	/* rate multiplier, for playback speed, etc. (e.g. 2.0 == twice as fast) */
	UnitHertz				= 8,	/* absolute frequency/pitch in cycles/second */
	UnitCents				= 9,	/* unit of relative pitch */
	UnitRelativeSemiTones	= 10,	/* useful for coarse detuning */
	UnitMIDINoteNumber		= 11,	/* absolute pitch as defined in the MIDI spec (exact freq may depend on tuning table) */
	UnitMIDIController		= 12,	/* a generic MIDI controller value from 0 -> 127 */
	UnitDecibels			= 13,	/* logarithmic relative gain */
	UnitLinearGain			= 14,	/* linear relative gain */
	UnitDegrees				= 15,	/* -180 to 180 degrees, similar to phase but more general (good for 3D coord system) */
	UnitEqualPowerCrossfade = 16,	/* 0 -> 100, crossfade mix two sources according to sqrt(x) and sqrt(1.0 - x) */
	UnitMixerFaderCurve1	= 17,	/* 0.0 -> 1.0, pow(x, 3.0) -> linear gain to simulate a reasonable mixer channel fader response */
	UnitPan					= 18,	/* standard left to right mixer pan */
	UnitMeters				= 19,	/* distance measured in meters */
	UnitAbsoluteCents		= 20,	/* absolute frequency measurement : if f is freq in hertz then 	*/
	/* absoluteCents = 1200 * log2(f / 440) + 6900					*/
	UnitOctaves				= 21,	/* octaves in relative pitch where a value of 1 is equal to 1200 cents*/
	UnitBPM					= 22,	/* beats per minute, ie tempo */
    UnitBeats               = 23,	/* time relative to tempo, ie. 1.0 at 120 BPM would equal 1/2 a second */
	UnitMilliseconds		= 24,	/* parameter is expressed in milliseconds */
	UnitRatio				= 25,	/* for compression, expansion ratio, etc. */
	
	UnitCustomUnit			= 26	/* this is the parameter unit type for parameters that present a custom unit name */
};


/**	This file defines a parameter used in an application.
	Both full-scale and normalised values must be present for
	AU and VST host campatability.
 */
class dRowParameter
{
	public:
	/// Create an uninitialised parameter
//	dRowParameter();
	
	/** Create a default initialised parameter.
	 
		This is called from a default constructor so can be used as a placeholder.
		Call init() once you know the parameter values.
		@see dRowParameter::init
	 */
	dRowParameter(const String& name_ ="parameter", ParameterUnit unit_ =UnitGeneric, String description_ ="A parameter",
				  double value_ =1.0f, double min_ =0.0f, double max_ =1.0f, double default_ =0.0f);
	
	/// Initialise parameter
	void init(const String& name_, ParameterUnit unit_, String description_,
			  double value_, double min_ =0.0f, double max_ =1.0f, double default_ =0.0f);

	double getValue();
	double getNormalisedValue();
	double getSmoothedValue();
	double getSmoothedNormalisedValue();
	double normaliseValue(double scaledValue);
	double getMin();
	double getMax();
	double getDefault();
	
	void smooth();
	
	const String getName();
	ParameterUnit getUnit();
	
	// the return of these can be used to see if the parameter has changed
	bool setValue(double value_);
	bool setNormalisedValue(double nvalue);
	
private:
	String name, description;
	double value, min, max, defaultValue;
	double smoothCoeff, smoothValue;
	ParameterUnit unit;
};

#endif //DROWPARAMETER_H