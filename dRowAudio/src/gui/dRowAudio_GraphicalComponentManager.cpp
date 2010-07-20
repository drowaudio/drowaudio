/*
  ==============================================================================

    dRowAudio_GraphicalComponentManager.cpp
    Created: 8 Jul 2010 2:13:03pm
    Author:  David Rowland

  ==============================================================================
*/
#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_GraphicalComponentManager.h"

GraphicalComponentManager::GraphicalComponentManager()
: TimeSliceThread ("graphical component manager")
{
	startThread(1);
}

GraphicalComponentManager::~GraphicalComponentManager()
{
}

void GraphicalComponentManager::addGraphicalComponent(GraphicalComponent *component)
{
	addTimeSliceClient(component);
}

void GraphicalComponentManager::removeGraphicalComponent(GraphicalComponent *component)
{
	removeTimeSliceClient(component);
}

END_DROWAUDIO_NAMESPACE