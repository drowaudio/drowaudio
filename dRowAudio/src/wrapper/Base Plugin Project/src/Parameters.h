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
	param0,
	param1,
	param2,
	param3,
	param4,
	param5,
	param6,
	param7,
	param8,
	param9,
	noParams
};

static const char UNUSED_NOWARN *parameterNames[] = { 
	"param0",						// 0
	"param1",						// 1
	"param2",						// 2
	"param3",						// 3
	"param4",						// 4
	"param5",						// 5
	"param6",						// 6
	"param7",						// 7
	"param8",						// 8
	"param9"						// 9
};

#endif //_PARAMETERS_H_