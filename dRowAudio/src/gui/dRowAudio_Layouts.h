/*
 *  dRowAudio_Layouts.h
 *  dRowAudio
 *
 *  Created by David Rowland on 03/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_LAYOUTS__H_
#define _DROWAUDIO_LAYOUTS__H_

#include <juce/juce.h>

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