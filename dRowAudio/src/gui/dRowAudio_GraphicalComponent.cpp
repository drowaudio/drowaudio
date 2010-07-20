/*
  ==============================================================================

    dRowAudio_GraphicalComponent.cpp
    Created: 8 Jul 2010 2:13:51pm
    Author:  David Rowland

  ==============================================================================
*/
#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

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

void GraphicalComponent::copyValues(float **values, int noValues, int noChannels)
{
	// allocate new memory only if needed
	if (noValues != numSamples) {
		numSamples = noValues;
		samples.malloc(numSamples);
	}
	
	// lock whilst copying
	ScopedLock sl (lock);

	if (noChannels == 1) {
		memcpy(samples, values[0], noValues*sizeof(float));
	}
	// this is quicker than the generic method below
	else if (noChannels == 2) {
		for (int i = 0; i < noValues; i++) {
			samples[i] = (fabsf(values[0][i]) > fabsf(values[1][i])) ? values[0][i] : values[1][i];
		}
	}
	else {
		zeromem(samples, noValues*sizeof(float));
		for (int c = 0; c < noChannels; c++) {
			for (int i = 0; i < noValues; i++) {
				if (fabsf(values[c][i]) > samples[i]) {
					samples[i] = values[c][i];
				}
			}			
		}
	}

	
	needToProcess = true;
}

END_DROWAUDIO_NAMESPACE