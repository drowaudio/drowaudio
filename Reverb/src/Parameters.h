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
	FFCOEFF,
	FBCOEFF,
	DELTIME,
	FILTERCF,
	DIFFUSION,
	SPREAD,
	param7,
	WETDRYMIX,
	ALLPASS,
	noParams
};

static const char UNUSED_NOWARN *parameterNames[] = { 
	"Gain",						// 0
	"ffCoeff",						// 1
	"fbCoeff",						// 2
	"DelTime",						// 3
	"Cuttoff F",						// 4
	"Allpass Coeff",						// 5
	"Stereo Spread",						// 6
	"param7",						// 7
	"Wet/Dry Mix",						// 8
	"Allpass"						// 9
};

#endif //_PARAMETERS_H_