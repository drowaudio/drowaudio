/*
 *  Parameters.h
 *
 *  Created by David Rowland on 02/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

enum parameters
{
	THRESH,
	REDUCTION,
	ATTACK,
	RELEASE,
	BANDCF,
	BANDQ,
	param6,
	param7,
	param8,
	GAIN,
	noParams
};

static const char __attribute__((unused)) *parameterNames[] = { 
	"Threshold",					// 0
	"Reduction",					// 1
	"Attack",						// 2
	"Release",						// 3
	"Bandpass Cf",					// 4
	"Bandpass Q",					// 5
	"param6",						// 6
	"param7",						// 7
	"param8",						// 8
	"Gain"							// 9
};

#endif //_PARAMETERS_H_