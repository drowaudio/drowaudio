/*
 *  dRowAudio.h
 *
 *  Created by David Rowland on 14/02/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _DROWAUDIOHEADER_H_
#define _DROWAUDIOHEADER_H_

/*==============================================================================

 This is the main dRowAudio header file that applications need to include.

//============================================================================*/


// Audio
#include "src/audio/dRowAudio_BiquadFilter.h"
#include "src/audio/dRowAudio_OnePoleFilter.h"

#include "src/audio/dRowAudio_AudioFilePlayer.h"
#include "src/audio/dRowAudio_DraggableWaveDisplay.h"
#include "src/audio/dRowAudio_PositionableWaveDisplay.h"
#include "src/audio/dRowAudio_TriBandAudioThumbnail.h"


// Gui
#include "src/gui/lookandfeel/dRowAudio_LookAndFeel.h"

#include "src/gui/dRowAudio_Meter.h"

// Utility
#include "src/parameters/dRowAudio_Parameter.h"
#include "src/utility/dRowAudio_Constants.h"
#include "src/utility/dRowAudio_Utility.h"


#endif //_DROWAUDIOHEADER_H_