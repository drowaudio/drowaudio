/*
 *  dRowAudio_Constants.h
 *  dRowAudio
 *
 *  Created by David Rowland on 22/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIOCONSTANTS_H_
#define _DROWAUDIOCONSTANTS_H_

#include <juce/juce.h>

/**
	@file 
 
	This file contains some useful constants for calculations such as reciprocals
	to avoid using expensive divides in programs.
 */

static const double oneOver60 = 1.0 / 60.0;
static const double oneOver60Squared = 1.0 / (60.0 * 60.0);
static const double oneOver180 = 1.0 / 180.0;
static const double oneOverPi = 1.0 / double_Pi;
static const double twoTimesPi = 2.0 * double_Pi;
static const double root2 = sqrt(2.0);
static const double oneOverRoot2 = 1.0 / sqrt(2.0);
static const double root3 = sqrt(3.0);
static const double oneOverRoot3 = 1.0 / sqrt(3.0);


#define squareNumber(x)	(x * x)

#endif //_DROWAUDIOCONSTANTS_H_