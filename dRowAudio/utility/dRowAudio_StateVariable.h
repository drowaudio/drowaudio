/*
 *  dRowAudio_StateVariable.h
 *  dRowAudio
 *
 *  Created by David Rowland on 15/07/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_STATEVARIABLE_H__
#define __DROWAUDIO_STATEVARIABLE_H__

#include "../core/dRowAudio_StandardHeader.h"

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
	
	JUCE_LEAK_DETECTOR (StateVariable);
};

#endif //__DROWAUDIO_STATEVARIABLE_H__