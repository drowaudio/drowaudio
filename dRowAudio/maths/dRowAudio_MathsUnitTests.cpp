/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#if DROWAUDIO_UNIT_TESTS

#include <modules/juce_core/juce_core.h>

//==============================================================================
class CumulativeMovingAverageTests  : public UnitTest
{
public:
    CumulativeMovingAverageTests() : UnitTest ("CumulativeMovingAverage") {}
    
    void runTest()
    {
        beginTest ("CumulativeMovingAverage");
        
        CumulativeMovingAverage average;
        
        expectEquals (average.add (1.0), 1.0);
        expectEquals (average.add (2.0), 1.5);
        expectEquals (average.add (3.0), 2.0);
        expectEquals (average.add (4.0), 2.5);
        expectEquals (average.add (5.0), 3.0);
        
        expectEquals (average.getNumValues(), 5);
    }
};

static CumulativeMovingAverageTests cumulativeMovingAverageUnitTests;

//==============================================================================
class MathsUnitTests  : public UnitTest
{
public:
    MathsUnitTests() : UnitTest ("Maths Utilities") {}
    
    void runTest()
    {
        beginTest ("Maths Utilities");

        expectEquals ((int) isEven (0), (int) true);
        expectEquals ((int) isEven (4), (int) true);
        expectEquals ((int) isEven (746352), (int) true);
        expectEquals ((int) isEven (-0), (int) true);
        expectEquals ((int) isEven (-4), (int) true);
        expectEquals ((int) isEven (-746352), (int) true);

        expectEquals ((int) isOdd (1), (int) true);
        expectEquals ((int) isOdd (23), (int) true);
        expectEquals ((int) isOdd (1763523), (int) true);
        expectEquals ((int) isOdd (-1), (int) true);
        expectEquals ((int) isOdd (-23), (int) true);
        expectEquals ((int) isOdd (-1763523), (int) true);

		expectEquals ((int) isnan (1), (int) false);
        expectEquals ((int) isnan (sqrt (-1.0)), (int) true);
    }
};

static MathsUnitTests mathsUnitTests;

//==============================================================================
class PitchTests  : public UnitTest
{
public:
    PitchTests() : UnitTest ("Pitch") {}
    
    void runTest()
    {
        beginTest ("Pitch");
        
        Pitch pitch (Pitch::fromFrequency (440));
        expectEquals (pitch.getFrequencyHz(), 440.0);
        expectEquals (pitch.getMidiNote(), 69.0);
        expectEquals (pitch.getMidiNoteName(), String ("A4"));

        pitch = Pitch::fromMidiNote (68);
        expect (almostEqual (pitch.getFrequencyHz(), 415.304698, 0.000001));
        expectEquals (pitch.getMidiNote(), 68.0);
        expectEquals (pitch.getMidiNoteName(), String ("G#4"));
        
        pitch = Pitch::fromNoteName ("A#3");
        expect (almostEqual (pitch.getFrequencyHz(), 116.54094, 0.000001));
        expectEquals (pitch.getMidiNote(), 46.0);
        expectEquals (pitch.getMidiNoteName(), String ("A#2"));
        
        String fFlatThree;
        fFlatThree << "F" << Pitch::getFlatSymbol() << "2";
        pitch = Pitch::fromNoteName (fFlatThree);
        expect (almostEqual (pitch.getFrequencyHz(), 41.2034446, 0.000001));
        expectEquals (pitch.getMidiNote(), 28.0);
        expectEquals (pitch.getMidiNoteName(), String ("E1"));
    }
};

static PitchTests pitchTests;

//==============================================================================

#endif // DROWAUDIO_UNIT_TESTS
