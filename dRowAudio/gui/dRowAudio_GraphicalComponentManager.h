/*
  ==============================================================================

    dRowAudio_GraphicalComponentManager.h
    Created: 8 Jul 2010 2:13:22pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_GRAPHICALCOMPONENTMANAGER_H_C569FF3E__
#define __DROWAUDIO_GRAPHICALCOMPONENTMANAGER_H_C569FF3E__

#include "../core/dRowAudio_StandardHeader.h"

#include "dRowAudio_GraphicalComponent.h";

/**	This class manages a number of GraphicalComponents giving them each a
	portion of time to do some processing they require on a background thread.
	Using one of these can save the Message thread from being blocked for too
	long and can make drawing CPU intensive components like VU meters a lot
	more efficient. For an example of how to use one see the SegmentedMeter.
 
	@see SegmentedMeter
 */
class GraphicalComponentManager : public TimeSliceThread
{
public:
	/**	Creates a GraphicalComponentManager.
		Only one of these is needed for a number of different GraphicalComponents.
		Register them with addGraphicalComponent() to begin processing.
	 */
	GraphicalComponentManager();
	
	/**	Destructor.
	 */
	~GraphicalComponentManager();
	
	/**	Registers a GraphicalComponent to begin its processing.
		Remember to remove it before deleting the object.
	 */
	void addGraphicalComponent(GraphicalComponent *component);
	
	/**	Unregisters a GraphicalComponent to stop its processing.
	 */
	void removeGraphicalComponent(GraphicalComponent *component);

private:
	
	friend class GraphicalComponent;
	
	JUCE_LEAK_DETECTOR (GraphicalComponentManager);
};

#endif  // __DROWAUDIO_GRAPHICALCOMPONENTMANAGER_H_C569FF3E__
