/*
  ==============================================================================

    dRowAudio_GraphicalComponent.cpp
    Created: 8 Jul 2010 2:13:51pm
    Author:  David Rowland

  ==============================================================================
*/

#include "dRowAudio_GraphicalComponent.h"

GraphicalComponent::GraphicalComponent()
:	paused(false),
	needToProcess(true),
	numSamples(0)
{
	samples.malloc(numSamples);

	startTimer(30);
}

GraphicalComponent::~GraphicalComponent()
{
}

void GraphicalComponent::process()
{
}

bool GraphicalComponent::useTimeSlice()
{
	if (paused) {
		return false;
	}
	else
	{
		if (needToProcess)
			process();
		needToProcess = false;
		return true;
	}
}

void GraphicalComponent::timerCallback()
{
	repaint();
}

void GraphicalComponent::copyValues(float *values, int noValues)
{
		// allocate new memory only if needed
		if (noValues != numSamples) {
			numSamples = noValues;
			samples.malloc(numSamples);
		}
		
		// lock whilst copying
		ScopedLock sl (lock);
		memcpy(samples, values, noValues*sizeof(float));
		
		needToProcess = true;
}
