/*
 *  dRowAudio_StandardHeader.h
 *  dRowAudio
 *
 *  Created by David Rowland on 20/07/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef __DROWAUDIO_STANDARDHEADER_H__
#define __DROWAUDIO_STANDARDHEADER_H__

#include "../../dRowAudio_Config.h"

#ifdef USE_VDSP
	#include <Accelerate/Accelerate.h>
//	#include <CoreServices/CoreServices.h>
#endif

// as a lot of classes and set-up macros are dependant on juce we will include it here
#include <juce/juce.h>

#ifdef DROWAUDIO_NAMESPACE
	#define BEGIN_DROWAUDIO_NAMESPACE    namespace DROWAUDIO_NAMESPACE {
	#define END_DROWAUDIO_NAMESPACE      }
#else
	#define BEGIN_DROWAUDIO_NAMESPACE 
	#define END_DROWAUDIO_NAMESPACE   
#endif


#endif //__DROWAUDIO_STANDARDHEADER_H__