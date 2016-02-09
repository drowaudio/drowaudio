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
/** Parameter Units - currently values are the same as the AudioUnit enums
 */
enum ParameterUnit
{
    UnitGeneric                = 0,    /* untyped value generally between 0.0 and 1.0 */
    UnitIndexed                = 1,    /* takes an integer value (good for menu selections) */
    UnitBoolean                = 2,    /* 0.0 means FALSE, non-zero means TRUE */
    UnitPercent                = 3,    /* usually from 0 -> 100, sometimes -50 -> +50 */
    UnitSeconds                = 4,    /* absolute or relative time */
    UnitSampleFrames        = 5,    /* one sample frame equals (1.0/sampleRate) seconds */
    UnitPhase                = 6,    /* -180 to 180 degrees */
    UnitRate                = 7,    /* rate multiplier, for playback speed, etc. (e.g. 2.0 == twice as fast) */
    UnitHertz                = 8,    /* absolute frequency/pitch in cycles/second */
    UnitCents                = 9,    /* unit of relative pitch */
    UnitRelativeSemiTones    = 10,    /* useful for coarse detuning */
    UnitMIDINoteNumber        = 11,    /* absolute pitch as defined in the MIDI spec (exact freq may depend on tuning table) */
    UnitMIDIController        = 12,    /* a generic MIDI controller value from 0 -> 127 */
    UnitDecibels            = 13,    /* logarithmic relative gain */
    UnitLinearGain            = 14,    /* linear relative gain */
    UnitDegrees                = 15,    /* -180 to 180 degrees, similar to phase but more general (good for 3D coord system) */
    UnitEqualPowerCrossfade = 16,    /* 0 -> 100, crossfade mix two sources according to sqrt(x) and sqrt(1.0 - x) */
    UnitMixerFaderCurve1    = 17,    /* 0.0 -> 1.0, pow(x, 3.0) -> linear gain to simulate a reasonable mixer channel fader response */
    UnitPan                    = 18,    /* standard left to right mixer pan */
    UnitMeters                = 19,    /* distance measured in meters */
    UnitAbsoluteCents        = 20,    /* absolute frequency measurement : if f is freq in hertz then     */
                                    /* absoluteCents = 1200 * log2(f / 440) + 6900                    */
    UnitOctaves                = 21,    /* octaves in relative pitch where a value of 1 is equal to 1200 cents*/
    UnitBPM                    = 22,    /* beats per minute, ie tempo */
    UnitBeats               = 23,    /* time relative to tempo, ie. 1.0 at 120 BPM would equal 1/2 a second */
    UnitMilliseconds        = 24,    /* parameter is expressed in milliseconds */
    UnitRatio                = 25,    /* for compression, expansion ratio, etc. */

    UnitCustomUnit            = 26    /* this is the parameter unit type for parameters that present a custom unit name */
};


//==============================================================================
/**    This file defines a parameter used in an application.

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

    /** Creates a copy of another parameter.
     */
    PluginParameter (const PluginParameter& other);

    //==============================================================================
    /** Initialise the parameter.
        Used to set up the parameter as required.
     */
    void init (const String& name_, ParameterUnit unit_ = UnitGeneric, String description_ = String::empty,
               double value_ = 0.0f, double min_ =0.0f, double max_ =1.0f, double default_ =0.0f,
               double skewFactor_ =1.0f, double smoothCoeff_ =0.1f, double step_ =0.01, String unitSuffix_ =String::empty);

    Value& getValueObject()                              {   return valueObject;     }

    double getValue() const                                    {   return double (valueObject.getValue()); }
    double getNormalisedValue() const                          {   return normaliseValue (getValue());     }
    void setValue (double value);
    void setNormalisedValue (double normalisedValue);
    double getSmoothedValue() const                            {   return smoothValue;     }
    double getSmoothedNormalisedValue() const                  {   return normaliseValue (smoothValue);     }

    double getMin() const                                      {   return min;             }
    double getMax() const                                      {   return max;             }
    double getDefault() const                                  {   return defaultValue;    }

    void smooth();
    void setSmoothCoeff (double newSmoothCoef);
    double getSmoothCoeff() const                              {   return smoothCoeff;     }

    void setSkewFactor (const double newSkewFactor);
    void setSkewFactorFromMidPoint (const double valueToShowAtMidPoint);
    double getSkewFactor() const                               {   return skewFactor;      }

    void setStep (double newStep);
    double getStep() const                                     {   return step;            }

    const String& getName() const                               {   return name;            }
    ParameterUnit getUnit() const                              {   return unit;            }
    const String& getUnitSuffix() const                         {   return unitSuffix;      }
    void setUnitSuffix (const String& newSuffix);

    void writeXml (XmlElement& xmlState);
    void readXml (const XmlElement* xmlState);

    /** Sets up a given slider with the parmeters properties.
     */
    void setupSlider (Slider& slider);

private:
    //==============================================================================
    Value valueObject;
    String name, description, unitSuffix;
    double min, max, defaultValue;
    double smoothCoeff, smoothValue;
    double skewFactor, step;
    ParameterUnit unit;

    double normaliseValue (double scaledValue) const noexcept;

    //==============================================================================
    JUCE_LEAK_DETECTOR (PluginParameter)
};

#endif //DROWAUDIO_PLUGINPARAMETER_H
