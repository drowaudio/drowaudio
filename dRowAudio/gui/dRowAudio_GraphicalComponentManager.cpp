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