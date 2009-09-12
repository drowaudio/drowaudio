/*
 *  Parameters.h
 *
 *  Created by David Rowland on 02/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#ifndef UNUSED_NOWARN

#if defined(JUCE_MAC)
// enable supression of unused variable is GCC
#define UNUSED_NOWARN __attribute__((unused))

#elif defined(JUCE_MSVC)

#define UNUSED_NOWARN

// disable unused variable warnings in MSVC (Windows)
#pragma warning( push )
#pragma warning( disable : 4705 )

#else

#define UNUSED_NOWARN

#endif

#endif // #ifndef UNUSED_NOWARN

enum parameters
{
	THRESH,
	REDUCTION,
	ATTACK,
	RELEASE,
	BANDCF,
	BANDQ,
	MONITOR,
	RMS,
	FILTER,
	PARAM09,
	noParams
};

static const char UNUSED_NOWARN *parameterNames[] = { 
	"Threshold",					// 0
	"Reduction",					// 1
	"Attack",						// 2
	"Release",						// 3
	"Bandpass Cf",					// 4
	"Bandpass Q",					// 5
	"Monitor",						// 6
	"Use RMS",						// 7
	"Filter Trigger",				// 8
	"Param09"							// 9
};

// this is used to map a button enum to a parameter
namespace buttons
{
	enum buttonEnum
	{
		MONITORBUTTON = MONITOR
	};
}

#endif //_PARAMETERS_H_