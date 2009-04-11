/*
 *  Parameters.h
 *
 *  Created by David Rowland on 02/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <dRowAudio/dRowAudio.h>

enum parameters
{
	GAIN,
	PREDELAY,
	FBCOEFF,
	DELTIME,
	FILTERCF,
	DIFFUSION,
	SPREAD,
	LOWEQ,
	HIGHEQ,
	WETDRYMIX,
	noParams
};

static const char UNUSED_NOWARN *parameterNames[] = { 
	"Gain",						// 0
	"Pre Delay",						// 1
	"fbCoeff",						// 2
	"DelTime",						// 3
	"Cuttoff F",						// 4
	"Allpass Coeff",						// 5
	"Stereo Spread",						// 6
	"Low EQ",						// 7
	"High EQ",						// 8
	"Wet/Dry Mix"						// 9
};

#endif //_PARAMETERS_H_