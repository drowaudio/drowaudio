/*
  ==============================================================================

    dRowAudio_GraphicalComponentManager.cpp
    Created: 8 Jul 2010 2:13:03pm
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

GraphicalComponentManager::GraphicalComponentManager()
    : TimeSliceThread ("Graphical Component Manager Thread")
{
	startThread(1);
}

GraphicalComponentManager::~GraphicalComponentManager()
{
}

void GraphicalComponentManager::addGraphicalComponent (GraphicalComponent* component)
{
	addTimeSliceClient (component);
}

void GraphicalComponentManager::removeGraphicalComponent (GraphicalComponent* component)
{
	removeTimeSliceClient (component);
}

END_JUCE_NAMESPACE