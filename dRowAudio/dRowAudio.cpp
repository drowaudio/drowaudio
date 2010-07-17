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
#include "src/audio/dRowAudio_AudioFilePlayer.cpp"
#include "src/audio/dRowAudio_FilteringAudioTransportSource.cpp"
#include "src/audio/dRowAudio_FilteringAudioFilePlayer.cpp"

#include "src/audio/dRowAudio_DraggableWaveDisplay.cpp"
#include "src/audio/dRowAudio_PositionableWaveDisplay.cpp"
#include "src/audio/dRowAudio_TriBandAudioThumbnail.cpp"

#include "src/audio/MADAudioFormat.cpp"
#include "src/audio/dRowAudio_Buffer.cpp"

#include "src/audio/dRowAudio_EnvelopeFollower.cpp"

#include "src/audio/filters/dRowAudio_OnePoleFilter.cpp"
#include "src/audio/filters/dRowAudio_OnePoleOneZeroFilter.cpp"
#include "src/audio/filters/dRowAudio_TwoPoleTwoZeroFilter.cpp"
#include "src/audio/filters/dRowAudio_BiquadFilter.cpp"
#include "src/audio/filters/dRowAudio_CascadedBiquadFilter.cpp"
#include "src/audio/filters/dRowAudio_CombFilter.cpp"
#include "src/audio/filters/dRowAudio_AllpassFilter.cpp"
#include "src/audio/filters/dRowAudio_InterpolatingAllpassFilter.cpp"
#include "src/audio/filters/dRowAudio_LBCF.cpp"
#include "src/audio/filters/dRowAudio_LBCFPointer.cpp"
#include "src/audio/filters/dRowAudio_InterpolatingLBCF.cpp"
#include "src/audio/filters/dRowAudio_DelayRegister.cpp"
#include "src/audio/filters/dRowAudio_TappedDelayLine.cpp"


// maths
#include "src/maths/dRowAudio_BezierCurve.cpp"

// Gui
#include "src/gui/lookandfeel/dRowAudio_PluginLookAndFeel.cpp"
#include "src/gui/lookandfeel/GuitarAmpKnobLookAndFeel.cpp"
#include "src/gui/lookandfeel/dRowAudio_GraphicalComponent.cpp"
#include "src/gui/lookandfeel/dRowAudio_GraphicalComponentManager.cpp"

#include "src/gui/dRowAudio_SimpleAudioScope.cpp"
#include "src/gui/dRowAudio_AudioOscilloscope.cpp"
#include "src/gui/dRowAudio_SegmentedMeter.cpp"
#include "src/gui/dRowAudio_Meter.cpp"
#include "src/gui/dRowAudio_StereoMeter.cpp"
#include "src/gui/dRowAudio_FilmstripSlider.cpp"
#include "src/gui/dRowAudio_CachedThumbnail.cpp"
#include "src/gui/dRowAudio_CpuMeter.cpp"
#include "src/gui/dRowAudio_Clock.cpp"
#include "src/gui/filebrowser/dRowAudio_BasicFileBrowser.cpp"
#include "src/gui/filebrowser/dRowAudio_ColumnFileBrowser.cpp"
//#include "src/gui/dRowAudio_Thumbnail.cpp"


// Utility
#include "src/parameters/dRowAudio_Parameter.cpp"
//#include "src/utility/dRowAudio_StateVariable.cpp"
