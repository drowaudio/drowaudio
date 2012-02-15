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

#ifndef __DROWAUDIO_CUMALATIVEMOVINGAVERAGE_H__
#define __DROWAUDIO_CUMALATIVEMOVINGAVERAGE_H__

//==============================================================================
/**
    Simple cumalative average class which you can add values to and will return
    the mean of them. This can be used when you don't know the total number of
    values that need to be averaged.
 */
class CumalativeMovingAverage
{
public:
    //==============================================================================
    /** Creates a blank CumalativeMovingAverage.
     */
    CumalativeMovingAverage() noexcept
        : currentAverage (0.0),
          numValues (0)
    {
    }
    
    /** Creates a copy of another CumalativeMovingAverage.
     */
    CumalativeMovingAverage (CumalativeMovingAverage& other) noexcept
    {
        currentAverage = other.currentAverage;
        numValues = other.numValues;
    }
    
    /** Destructor.
     */
    ~CumalativeMovingAverage()
    {
    }
    
    /** Resets the CumalativeMovingAverage.
     */
    void reset() noexcept
    {
        currentAverage = 0.0;
        numValues = 0;
    }
    
    /** Adds a new value to contribute to the average and returns the average.
     */
    inline double add (double newValue) noexcept
    {
        return currentAverage = (newValue + (numValues * currentAverage)) /  ++numValues;
    }
    
    /** Returns the current average.
     */
    inline double getAverage()                      {   return currentAverage;  }
    
    /** Returns the number of values that have contributed to the current average.
     */
    inline double getNumValues()                    {   return numValues;       }
    
private:
    //==============================================================================
    double currentAverage;
    int numValues;
    
    JUCE_LEAK_DETECTOR (CumalativeMovingAverage);
};


#endif //__DROWAUDIO_CUMALATIVEMOVINGAVERAGE_H__