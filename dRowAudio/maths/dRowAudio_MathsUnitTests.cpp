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

        expectEquals ((int) isnan (1), 0);
        expectEquals ((int) isnan (sqrt (-1.0)), 1);
    }
};

static MathsUnitTests mathsUnitTests;

//==============================================================================


#endif // DROWAUDIO_UNIT_TESTS