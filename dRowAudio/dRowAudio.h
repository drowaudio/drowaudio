/*
 *  dRowAudio.h
 *
 *  Created by David Rowland on 14/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIOHEADER_H_
#define _DROWAUDIOHEADER_H_

/*==============================================================================

 This is the main dRowAudio header file that applications need to include.

//============================================================================*/


// Audio
#include "src/audio/filters/dRowAudio_BiquadFilter.h"
#include "src/audio/filters/dRowAudio_OnePoleFilter.h"
#include "src/audio/filters/dRowAudio_CombFilter.h"
#include "src/audio/filters/dRowAudio_AllpassFilter.h"
#include "src/audio/filters/dRowAudio_InterpolatingAllpassFilter.h"
#include "src/audio/filters/dRowAudio_LBCF.h"
#include "src/audio/filters/dRowAudio_LBCFPointer.h"
#include "src/audio/filters/dRowAudio_DelayRegister.h"
#include "src/audio/filters/dRowAudio_InterpolatingLBCF.h"
#include "src/audio/filters/dRowAudio_TappedDelayLine.h"

#include "src/audio/dRowAudio_AudioFilePlayer.h"
#include "src/audio/dRowAudio_FilteringAudioTransportSource.h"
#include "src/audio/dRowAudio_FilteringAudioFilePlayer.h"

#include "src/audio/dRowAudio_DraggableWaveDisplay.h"
#include "src/audio/dRowAudio_PositionableWaveDisplay.h"
#include "src/audio/dRowAudio_TriBandAudioThumbnail.h"

#include "src/audio/dRowAudio_AudioUtility.h"

#include "src/audio/MADAudioFormat.h"


// Gui
#include "src/gui/lookandfeel/dRowAudio_LookAndFeel.h"

#include "src/gui/dRowAudio_Meter.h"
#include "src/gui/dRowAudio_StereoMeter.h"


// Utility
#include "src/parameters/dRowAudio_Parameter.h"
#include "src/utility/dRowAudio_Constants.h"
#include "src/utility/dRowAudio_Utility.h"


#endif //_DROWAUDIOHEADER_H_