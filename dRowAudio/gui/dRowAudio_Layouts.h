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

#ifndef _DROWAUDIO_LAYOUTS__H_
#define _DROWAUDIO_LAYOUTS__H_

#include "../core/dRowAudio_StandardHeader.h"

static void distributeEvenly (int noElements)
{
	
}

/**	Returns the horizontal centre of a Component.
 */
static int getCentreX(Component *component)
{
	return component->getX()+(component->getWidth()/2);
}
/**	Returns the vertical centre of a Component.
 */
static int getCentreY(Component *component)
{
	return component->getY()+(component->getHeight()/2);
}

//static Point<int> getPointAtDistanceAndAngle(Component *component, int distance, float angleRad)
//{
//	Point<int> point(getCentreX(component), getCentreY(component));
//	
//	point.getX()
//	
//	return point;
//}

#endif //_DROWAUDIO_LAYOUTS__H_