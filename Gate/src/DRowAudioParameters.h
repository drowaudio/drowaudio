/*
 *  DRowAudioParameters.h
 *
 *  Created by David Rowland on 02/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWAUDIOPARAMETERS_H_
#define _DROWAUDIOPARAMETERS_H_

#include <dRowAudio/dRowAudio.h>

enum parameters
{
	THRESH,
	REDUCTION,
	ATTACK,
	HOLD,
	RELEASE,
	BANDCF,
	BANDQ,
	MONITOR,
	FILTER,
	PARAM09,
	noParams
};

static const char UNUSED_NOWARN *parameterNames[] = { 
	"Threshold",					// 0
	"Reduction",					// 1
	"Attack",						// 2
	"Hold",							// 3
	"Release",						// 4
	"Bandpass Cf",					// 5
	"Bandpass Q",					// 6
	"Monitor",						// 7
	"Filter Trigger",				// 8
	"Param09"						// 9
};

// this is used to map a button enum to a parameter
namespace buttons
{
	enum buttonEnum
	{
		MONITORBUTTON = MONITOR
	};
}

#endif //_DROWAUDIOPARAMETERS_H_