/*
 *  dRowAudio.cpp
 *  
 *
 *  Created by David Rowland on 14/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

/*===============================================================================

	This file can be used to include all of the dRowAudio files in one go.

 //============================================================================*/


// Audio
#include "src/audio/filters/dRowAudio_BiquadFilter.cpp"
#include "src/audio/filters/dRowAudio_OnePoleFilter.cpp"
#include "src/audio/filters/dRowAudio_CombFilter.cpp"
#include "src/audio/filters/dRowAudio_AllpassFilter.cpp"
#include "src/audio/filters/dRowAudio_InterpolatingAllpassFilter.cpp"
#include "src/audio/filters/dRowAudio_LBCF.cpp"
#include "src/audio/filters/dRowAudio_LBCFPointer.cpp"
#include "src/audio/filters/dRowAudio_InterpolatingLBCF.cpp"
#include "src/audio/filters/dRowAudio_DelayRegister.cpp"
#include "src/audio/filters/dRowAudio_TappedDelayLine.cpp"

#include "src/audio/dRowAudio_AudioFilePlayer.cpp"
#include "src/audio/dRowAudio_FilteringAudioTransportSource.cpp"
#include "src/audio/dRowAudio_FilteringAudioFilePlayer.cpp"

#include "src/audio/dRowAudio_DraggableWaveDisplay.cpp"
#include "src/audio/dRowAudio_PositionableWaveDisplay.cpp"
#include "src/audio/dRowAudio_TriBandAudioThumbnail.cpp"

#include "src/audio/MADAudioFormat.cpp"

// Gui
#include "src/gui/lookandfeel/dRowAudio_PluginLookAndFeel.cpp"

#include "src/gui/dRowAudio_Meter.cpp"
#include "src/gui/dRowAudio_StereoMeter.cpp"

// Utility
#include "src/parameters/dRowAudio_Parameter.cpp"