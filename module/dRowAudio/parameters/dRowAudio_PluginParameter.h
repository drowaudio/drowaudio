/*
  ==============================================================================

  This file is part of the dRowAudio JUCE module
  Copyright 2004-13 by dRowAudio.

  ------------------------------------------------------------------------------

  dRowAudio is provided under the terms of The MIT License (MIT):

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  ==============================================================================
*/

#ifndef DROWAUDIO_PLUGINPARAMETER_H
#define DROWAUDIO_PLUGINPARAMETER_H

#include "../utility/dRowAudio_Utility.h"

//==============================================================================
/** Parameter Units - currently values are the same as the AudioUnit enums */
enum ParameterUnit
{
    UnitGeneric,                /* untyped value generally between 0.0 and 1.0 */
    UnitIndexed,                /* takes an integer value (good for menu selections) */
    UnitBoolean,                /* 0.0 means FALSE, non-zero means TRUE */
    UnitPercent,                /* usually from 0 -> 100, sometimes -50 -> +50 */
    UnitSeconds,                /* absolute or relative time */
    UnitSampleFrames,           /* one sample frame equals (1.0/sampleRate) seconds */
    UnitPhase,                  /* -180 to 180 degrees */
    UnitRate,                   /* rate multiplier, for playback speed, etc. (e.g. 2.0 == twice as fast) */
    UnitHertz,                  /* absolute frequency/pitch in cycles/second */
    UnitCents,                  /* unit of relative pitch */
    UnitRelativeSemiTones,      /* useful for coarse detuning */
    UnitMIDINoteNumber,         /* absolute pitch as defined in the MIDI spec (exact freq may depend on tuning table) */
    UnitMIDIController,         /* a generic MIDI controller value from 0 -> 127 */
    UnitDecibels,               /* logarithmic relative gain */
    UnitLinearGain,             /* linear relative gain */
    UnitDegrees,                /* -180 to 180 degrees, similar to phase but more general (good for 3D coord system) */
    UnitEqualPowerCrossfade,    /* 0 -> 100, crossfade mix two sources according to sqrt(x) and sqrt(1.0 - x) */
    UnitMixerFaderCurve1,       /* 0.0 -> 1.0, pow(x, 3.0) -> linear gain to simulate a reasonable mixer channel fader response */
    UnitPan,                    /* standard left to right mixer pan */
    UnitMeters,                 /* distance measured in meters */
    UnitAbsoluteCents,          /* absolute frequency measurement : if f is freq in hertz then     */
                                /* absoluteCents = 1200 * log2(f / 440) + 6900                    */
    UnitOctaves,                /* octaves in relative pitch where a value of 1 is equal to 1200 cents*/
    UnitBPM,                    /* beats per minute, ie tempo */
    UnitBeats,                  /* time relative to tempo, ie. 1.0 at 120 BPM would equal 1/2 a second */
    UnitMilliseconds,           /* parameter is expressed in milliseconds */
    UnitRatio,                  /* for compression, expansion ratio, etc. */

    UnitCustomUnit              /* this is the parameter unit type for parameters that present a custom unit name */
};

//==============================================================================
/** This file defines a parameter used in an application.

    Both full-scale and normalised values must be present for
    AU and VST host campatability.
*/
class PluginParameter
{
public:
    /** Create a default parameter.

        This just uses some standard default values so it can be used as a placeholder.
        Call init() once you know the parameter values.

        @see init()
    */
    PluginParameter();

    /** Creates a copy of another parameter. */
    PluginParameter (const PluginParameter& other);

    //==============================================================================
    /** Initialise the parameter.

        Used to set up the parameter as required.
    */
    void init (const String& name_, ParameterUnit unit_ = UnitGeneric, String description_ = {},
               double value_ = 0.0, double min_ = 0.0, double max_ = 1.0, double default_ = 0.0,
               double skewFactor_ = 1.0, double smoothCoeff_ = 0.1, double step_ = 0.01, String unitSuffix_ = {});

    Value& getValueObject() { return valueObject; }

    double getValue() const                                     { return double (valueObject.getValue()); }
    double getNormalisedValue() const                           { return normaliseValue (getValue()); }
    void setValue (double value);
    void setNormalisedValue (double normalisedValue);
    double getSmoothedValue() const                             { return smoothValue; }
    double getSmoothedNormalisedValue() const                   { return normaliseValue (smoothValue); }

    double getMin() const                                       { return min; }
    double getMax() const                                       { return max; }
    double getDefault() const                                   { return defaultValue; }

    void smooth();
    void setSmoothCoeff (double newSmoothCoef);
    double getSmoothCoeff() const                               { return smoothCoeff; }

    void setSkewFactor (double newSkewFactor);
    void setSkewFactorFromMidPoint (double valueToShowAtMidPoint);
    double getSkewFactor() const                                { return skewFactor; }

    void setStep (double newStep);
    double getStep() const                                      { return step; }

    const String& getName() const                               { return name; }
    ParameterUnit getUnit() const                               { return unit; }
    const String& getUnitSuffix() const                         { return unitSuffix; }
    void setUnitSuffix (const String& newSuffix);

    void writeXml (XmlElement& xmlState);
    void readXml (const XmlElement* xmlState);

    /** Sets up a given slider with the parmeters properties. */
    void setupSlider (Slider& slider);

private:
    //==============================================================================
    Value valueObject;
    String name, description, unitSuffix;
    double min, max, defaultValue;
    double smoothCoeff, smoothValue;
    double skewFactor, step;
    ParameterUnit unit;

    //==============================================================================
    double normaliseValue (double scaledValue) const noexcept;

    //==============================================================================
    JUCE_LEAK_DETECTOR (PluginParameter)
};

#endif //DROWAUDIO_PLUGINPARAMETER_H
