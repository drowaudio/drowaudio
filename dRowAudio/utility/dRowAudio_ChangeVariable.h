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

#ifndef __DROWAUDIO_CHANGEVARIABLE_H_5DADFA16__
#define __DROWAUDIO_CHANGEVARIABLE_H_5DADFA16__

#include "../core/dRowAudio_StandardHeader.h"

/**
	Variable that keeps a record of when it is changed.
	
	This is similar to a StateVariable except that it will keeps track of
	when the state of the variable changed.
 
	For example:
	ChangeVariable<bool> var(false);
	var = true;
	var.lastSetChanged();	// returns true
	var = true;
	var.lastSetChanged();	// returns false
	var = false;
	var.lastSetChanged();	// returns true
 */
template <class VariableType>
class ChangeVariable
{
public:
	/**	Create a ChangeVariable with an initial value of 0.
	 */
	ChangeVariable()
	{
		previous = current = 0.0;
		changed = false;
	}
	
	/**	Create a StateVariable with an initial value.
		To begin with the previous value will be the same as the initial.
	 */
	ChangeVariable(VariableType initialValue)
	{
		previous = current = initialValue;
		changed = false;
	}
	
	/*	Destructor.
	 */
	~ChangeVariable() {}
	
	VariableType getCurrent()	{	return current;		}
	VariableType getPrevious()	{	return previous;	}
	
	inline void set(VariableType newValue)
	{
		changed = (newValue == current) ? false : true;
		previous = current;
		current = newValue;
	}
	
//	void setOnlyCurrent(VariableType newValue)
//	{
//		current = newValue;
//	}
//	
//	void setBoth(VariableType newValue)
//	{
//		current = previous = newValue;
//	}
//	
//	void setPrevious(VariableType newValue)
//	{
//		previous = newValue;
//	}
//	
//	/**	Returns true if the current and previous states are equal.
//	 */
//	bool areEqual()
//	{
//		return (previous == current);
//	}
//	
//	bool areAlmostEqual(double precision =0.00001)
//	{
//		return almostEqual(current, previous, precision);
//	}

	inline void operator= (VariableType newValue)
	{
		set(newValue);
	}

//	void operator*= (VariableType newValue)
//	{
//		previous = current;
//		current *= newValue;
//	}
		
	/**	This returns the difference between the current and the previous state.
	 */
//	VariableType getDifference()
//	{
//		return current - previous;
//	}

	inline bool lastSetChanged()
	{
		return changed;
	}
	
private:
	VariableType current, previous;
	bool changed;
	
	JUCE_LEAK_DETECTOR (ChangeVariable);
};

#endif  // __DROWAUDIO_CHANGEVARIABLE_H_5DADFA16__
