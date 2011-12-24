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

#ifndef __DROWAUDIO_GRAPHICALCOMPONENTMANAGER_H__
#define __DROWAUDIO_GRAPHICALCOMPONENTMANAGER_H__

class GraphicalComponent;

//==============================================================================
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
    //==============================================================================
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
	void addGraphicalComponent (GraphicalComponent* component);
	
	/**	Unregisters a GraphicalComponent to stop its processing.
	 */
	void removeGraphicalComponent (GraphicalComponent* component);

private:
    //==============================================================================
	friend class GraphicalComponent;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicalComponentManager);
};

#endif  // __DROWAUDIO_GRAPHICALCOMPONENTMANAGER_H__