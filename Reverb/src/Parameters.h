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
	param6,
	param7,
	param8,
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
	"param6",						// 6
	"param7",						// 7
	"param8",						// 8
	"Allpass"						// 9
};

#endif //_PARAMETERS_H_