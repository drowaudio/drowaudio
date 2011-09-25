/*
 *  dRowAudio.h
 *
 *  Created by David Rowland on 14/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIOHEADER_H_
#define _DROWAUDIOHEADER_H_

#include "../../../juce_source/juce/modules/juce_audio_basics/juce_audio_basics.h"
#include "../../../juce_source/juce/modules/juce_audio_devices/juce_audio_devices.h"
#include "../../../juce_source/juce/modules/juce_audio_formats/juce_audio_formats.h"
#include "../../../juce_source/juce/modules/juce_audio_processors/juce_audio_processors.h"
#include "../../../juce_source/juce/modules/juce_audio_utils/juce_audio_utils.h"
#include "../../../juce_source/juce/modules/juce_core/juce_core.h"
#include "../../../juce_source/juce/modules/juce_data_structures/juce_data_structures.h"
#include "../../../juce_source/juce/modules/juce_events/juce_events.h"
#include "../../../juce_source/juce/modules/juce_graphics/juce_graphics.h"
#include "../../../juce_source/juce/modules/juce_gui_basics/juce_gui_basics.h"
#include "../../../juce_source/juce/modules/juce_gui_extra/juce_gui_extra.h"

BEGIN_JUCE_NAMESPACE

// Audio
#ifndef _DROWAUDIO_AUDIOFILEPLAYER__H_
    #include "audio/dRowAudio_AudioFilePlayer.h"
#endif

//#include "src/audio/dRowAudio_FilteringAudioSource.h"
//#include "src/audio/dRowAudio_SmoothingAudioSource.h"
//#include "src/audio/dRowAudio_ReversibleAudioSource.h"
//#include "src/audio/dRowAudio_FilteringAudioTransportSource.h"
//#include "src/audio/dRowAudio_FilteringAudioFilePlayer.h"
//
////#include "src/audio/dRowAudio_TriBandAudioThumbnail.h"
//
//#include "src/audio/dRowAudio_AudioUtility.h"
//#include "src/audio/dRowAudio_FIFOBuffer.h"
//#include "src/audio/dRowAudio_CircularBuffer.h"
//#include "src/audio/dRowAudio_Buffer.h"
////#include "src/audio/dRowAudio_BufferArray.h"
//
//#include "src/audio/dRowAudio_EnvelopeFollower.h"
//
//#include "src/audio/filters/dRowAudio_OnePoleFilter.h"
//#include "src/audio/filters/dRowAudio_OnePoleOneZeroFilter.h"
//#include "src/audio/filters/dRowAudio_TwoPoleTwoZeroFilter.h"
//#include "src/audio/filters/dRowAudio_BiquadFilter.h"
//#include "src/audio/filters/dRowAudio_CascadedBiquadFilter.h"
//#include "src/audio/filters/dRowAudio_CombFilter.h"
//#include "src/audio/filters/dRowAudio_AllpassFilter.h"
//#include "src/audio/filters/dRowAudio_InterpolatingAllpassFilter.h"
//#include "src/audio/filters/dRowAudio_LBCF.h"
//#include "src/audio/filters/dRowAudio_DelayRegister.h"
//#include "src/audio/filters/dRowAudio_InterpolatingLBCF.h"
//#include "src/audio/filters/dRowAudio_TappedDelayLine.h"
//
//#include "src/audio/fft/dRowAudio_FFTOperation.h"
//#include "src/audio/fft/dRowAudio_FFTEngine.h"
//#include "src/audio/fft/dRowAudio_Window.h"

// maths
#ifndef __DROWAUDIO_MATHSUTILITIES_H__
    #include "maths/dRowAudio_MathsUtilities.h"
#endif 
#ifndef __DROWAUDIO_BEZIERCURVE_H__
    #include "maths/dRowAudio_BezierCurve.h"
#endif

//// Gui
//#include "src/gui/dRowAudio_Layouts.h"
//#include "src/gui/lookandfeel/dRowAudio_LookAndFeel.h"
//
//#include "src/gui/dRowAudio_GraphicalComponent.h"
//#include "src/gui/dRowAudio_GraphicalComponentManager.h"
//#include "src/gui/dRowAudio_SimpleAudioScope.h"
//#include "src/gui/dRowAudio_AudioOscilloscope.h"
//#include "src/gui/dRowAudio_SegmentedMeter.h"
//#include "src/gui/dRowAudio_Meter.h"
//#include "src/gui/dRowAudio_StereoMeter.h"
//#include "src/gui/dRowAudio_FilmstripSlider.h"
//#include "src/gui/dRowAudio_CpuMeter.h"
//#include "src/gui/dRowAudio_Clock.h"
//#include "src/gui/dRowAudio_CentreAlignViewport.h"
//#include "src/gui/dRowAudio_MusicLibraryTable.h"
//#include "src/gui/filebrowser/dRowAudio_BasicFileBrowser.h"
//#include "src/gui/filebrowser/dRowAudio_ColumnFileBrowser.h"
//
//// audiothumbnail
//#include "src/gui/audiothumbnail/dRowAudio_MultipleAudioThumbnailCache.h"
//#include "src/gui/audiothumbnail/dRowAudio_PositionableWaveDisplay.h"
//#include "src/gui/audiothumbnail/dRowAudio_ColouredPositionableWaveDisplay.h"
//#include "src/gui/audiothumbnail/dRowAudio_AbstractDraggableWaveDisplay.h"
//#include "src/gui/audiothumbnail/dRowAudio_FilteredAudioThumbnail.h"
//#include "src/gui/audiothumbnail/dRowAudio_ColouredAudioThumbnail.h"
//#include "src/gui/audiothumbnail/dRowAudio_DraggableWaveDisplay.h"
//#include "src/gui/audiothumbnail/dRowAudio_ColouredDraggableWaveDisplay.h"
//#include "src/gui/audiothumbnail/dRowAudio_TriBandDraggableWaveDisplay.h"
//#include "src/gui/audiothumbnail/dRowAudio_SwitchableDraggableWaveDisplay.h"
//
// Utility

#ifndef __DROWAUDIO_PLUGINPARAMETER_H__
    #include "parameters/dRowAudio_PluginParameter.h"
#endif _DROWAUDIO_PLUGINPARAMETER_H
//#include "src/utility/dRowAudio_Constants.h"
#ifndef __DROWAUDIO_UTILITY_H__
    #include "utility/dRowAudio_Utility.h"
#endif
//#include "src/utility/dRowAudio_StateVariable.h"
//#include "src/utility/dRowAudio_ChangeVariable.h"
#ifndef __DROWAUDIO_XMLHELPERS_H__
    #include "utility/dRowAudio_XmlHelpers.h"
#endif
//#include "src/utility/dRowAudio_Comparators.h"
//#include "src/utility/dRowAudio_ITunesLibrary.h"
//#include "src/utility/dRowAudio_ITunesLibaryParser.h"
//
//using namespace DROWAUDIO_NAMESPACE;

END_JUCE_NAMESPACE

#endif //_DROWAUDIOHEADER_H_