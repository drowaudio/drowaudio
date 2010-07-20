/*
  ==============================================================================

    dRowAudio_GraphicalComponent.h
    Created: 8 Jul 2010 2:14:02pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_GRAPHICALCOMPONENT_H_91791CE3__
#define __DROWAUDIO_GRAPHICALCOMPONENT_H_91791CE3__

#include "../core/dRowAudio_StandardHeader.h"

/**	This class is an abstract base blass for some kind of graphical component
	that requires some intenisve processing.
	Inherit your class from this then register it with a GraphicalComponentManager
	to continually call the process() method where you can do your required
	processing on a background thread to avoid blocking the Message thread for too long.
 
	@see SegmentedMeter
 */
class GraphicalComponent :	public Component,
							public TimeSliceClient,
							public Timer
{
protected:
	/**	Creates a GraphicalComponent.
		Don't instantiate directly, use as a base class.
	 */
	GraphicalComponent();
	
public:
	/**	Destructor.
	 */
	~GraphicalComponent();

	/**	Overload to do your processing.
		Once registered with a GraphicalComponentManager this will repeatedly get called.
		To save CPU cycles this will only get called if paused is false and some new data
		has been set with copyValues(). The idea is that you push some new data to your
		class with copyValues() then do whatever processing you require here.
	 */
	virtual void process();
	
	/**	Pauses the processing of the GraphicalComponent.
	 */
	void pause(bool shouldPause)	{	paused = shouldPause;	}
	
	/**	Returns true if the processing is currently suspended.
	 */
	bool isPaused()					{	return paused;	};
	
	//==============================================================	
	/** Copies data to the component to use.
		This should be as quick as possible as is accessed from what
		ever thread calls it so could cause blocking. 
	 */
	void copyValues(float *values, int noValues);

	/** Copies data from a number of channels to the component to use.
		This is a lot slower than copyValues(float *values, int noValues) but if the
		number of channels is 2 it will use the maximum sample from the pair of channels.
	 */
	void copyValues(float **values, int noValues, int noChannels);
	
	/** @internal */
	bool useTimeSlice();
	
	/** @internal */
	void timerCallback();
	
	//==============================================================

protected:
	
	CriticalSection lock;
	bool paused;
	bool needToProcess;
	
	int numSamples;
	HeapBlock<float> samples;
};

#endif  // __DROWAUDIO_GRAPHICALCOMPONENT_H_91791CE3__
