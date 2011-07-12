/*
 *  dRowAudio.cpp
 *  
 *
 *  Created by David Rowland on 14/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#include "src/core/dRowAudio_StandardHeader.h"

/*===============================================================================

	This file can be used to include all of the dRowAudio files in one go.

 //============================================================================*/

// Audio
#include "src/audio/dRowAudio_AudioFilePlayer.cpp"
#include "src/audio/dRowAudio_FilteringAudioSource.cpp"
#include "src/audio/dRowAudio_SmoothingAudioSource.cpp"
#include "src/audio/dRowAudio_ReversibleAudioSource.cpp"
#include "src/audio/dRowAudio_FilteringAudioTransportSource.cpp"
#include "src/audio/dRowAudio_FilteringAudioFilePlayer.cpp"

#include "src/audio/dRowAudio_DraggableWaveDisplay.cpp"
#include "src/audio/dRowAudio_PositionableWaveDisplay.cpp"
//#include "src/audio/dRowAudio_TriBandAudioThumbnail.cpp"

//#include "src/audio/MADAudioFormat.cpp"
#include "src/audio/dRowAudio_FIFOBuffer.cpp"
#include "src/audio/dRowAudio_CircularBuffer.cpp"
#include "src/audio/dRowAudio_Buffer.cpp"
#include "src/audio/dRowAudio_BufferArray.cpp"

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
#include "src/audio/filters/dRowAudio_InterpolatingLBCF.cpp"
#include "src/audio/filters/dRowAudio_DelayRegister.cpp"
#include "src/audio/filters/dRowAudio_TappedDelayLine.cpp"

#include "src/audio/fft/dRowAudio_FFTEngine.cpp"
#include "src/audio/fft/dRowAudio_Window.cpp"

#ifdef JUCE_MAC
	#include "src/audio/fft/dRowAudio_mac_FFTOperation.cpp"
#endif

// maths
#include "src/maths/dRowAudio_BezierCurve.cpp"

// Gui
#include "src/gui/lookandfeel/dRowAudio_PluginLookAndFeel.cpp"
#include "src/gui/lookandfeel/GuitarAmpKnobLookAndFeel.cpp"

#include "src/gui/dRowAudio_GraphicalComponent.cpp"
#include "src/gui/dRowAudio_GraphicalComponentManager.cpp"
#include "src/gui/dRowAudio_SimpleAudioScope.cpp"
#include "src/gui/dRowAudio_AudioOscilloscope.cpp"
#include "src/gui/dRowAudio_SegmentedMeter.cpp"
#include "src/gui/dRowAudio_Meter.cpp"
#include "src/gui/dRowAudio_StereoMeter.cpp"
#include "src/gui/dRowAudio_FilmstripSlider.cpp"
#include "src/gui/dRowAudio_CpuMeter.cpp"
#include "src/gui/dRowAudio_Clock.cpp"
#include "src/gui/dRowAudio_CentreAlignViewport.cpp"
#include "src/gui/dRowAudio_MusicLibraryTable.cpp"
#include "src/gui/filebrowser/dRowAudio_BasicFileBrowser.cpp"
#include "src/gui/filebrowser/dRowAudio_ColumnFileBrowser.cpp"

#include "src/gui/audiothumbnail/dRowAudio_MultipleAudioThumbnailCache.cpp"
#include "src/gui/audiothumbnail/dRowAudio_PositionableWaveDisplay.cpp"
#include "src/gui/audiothumbnail/dRowAudio_ColouredPositionableWaveDisplay.cpp"
#include "src/gui/audiothumbnail/dRowAudio_AbstractDraggableWaveDisplay.cpp"
#include "src/gui/audiothumbnail/dRowAudio_FilteredAudioThumbnail.cpp"
#include "src/gui/audiothumbnail/dRowAudio_ColouredAudioThumbnail.cpp"
#include "src/gui/audiothumbnail/dRowAudio_DraggableWaveDisplay.cpp"
#include "src/gui/audiothumbnail/dRowAudio_ColouredDraggableWaveDisplay.cpp"
#include "src/gui/audiothumbnail/dRowAudio_TriBandDraggableWaveDisplay.cpp"
#include "src/gui/audiothumbnail/dRowAudio_SwitchableDraggableWaveDisplay.cpp"

// Utility
#include "src/parameters/dRowAudio_Parameter.cpp"
//#include "src/utility/dRowAudio_StateVariable.cpp"
#include "src/utility/dRowAudio_ITunesLibrary.cpp"
#include "src/utility/dRowAudio_ITunesLibaryParser.cpp"
