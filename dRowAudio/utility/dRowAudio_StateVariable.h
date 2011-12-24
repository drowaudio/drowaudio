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

#ifndef __DROWAUDIO_STATEVARIABLE_H__
#define __DROWAUDIO_STATEVARIABLE_H__

/**
	Variable that holds its previous value.
 
	This can be used instead of keeping track of a current and previous state
	of a primitive variable. Just calling set() will automatically update the
	new and previous states.
 */
template <class VariableType>
class StateVariable
{
public:
	/**	Create a StateVariable with an initial value of 0.
	 */
	StateVariable()
	{
		previous = current = 0.0;
	}
	
	/**	Create a StateVariable with an initial value.
		To begin with the previous value will be the same as the initial.
	 */
	StateVariable(VariableType initialValue)
	{
		previous = current = initialValue;
	}
	
	/*	Destructor.
	 */
	~StateVariable() {}
	
	VariableType getCurrent()	{	return current;		}
	VariableType getPrevious()	{	return previous;	}
	
	void set(VariableType newValue)
	{
		previous = current;
		current = newValue;
	}
	
	void setOnlyCurrent(VariableType newValue)
	{
		current = newValue;
	}
	
	void setBoth(VariableType newValue)
	{
		current = previous = newValue;
	}
	
	void setPrevious(VariableType newValue)
	{
		previous = newValue;
	}
	
	/**	Returns true if the current and previous states are equal.
	 */
	bool areEqual()
	{
		return (previous == current);
	}
	
	bool areAlmostEqual(double precision =0.00001)
	{
		return almostEqual(current, previous, precision);
	}

	void operator= (VariableType newValue)
	{
		previous = current;
		current = newValue;
	}

	void operator*= (VariableType newValue)
	{
		previous = current;
		current *= newValue;
	}
		
	/**	This returns the difference between the current and the previous state.
	 */
	VariableType getDifference()
	{
		return current - previous;
	}
	
private:
	VariableType current, previous;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StateVariable);
};

#endif //__DROWAUDIO_STATEVARIABLE_H__