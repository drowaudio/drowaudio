/*
 *  dRowAudio.h
 *
 *  Created by David Rowland on 14/02/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIOHEADER_H_
#define _DROWAUDIOHEADER_H_

#include "src/core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

/*==============================================================================
 
 This is the main dRowAudio header file that applications need to include.
 As this library is so heavily dependant on juce, if you include this there is
 no need to also include juce.h
 
 //============================================================================*/

// Audio
#include "src/audio/dRowAudio_AudioFilePlayer.h"
#include "src/audio/dRowAudio_FilteringAudioSource.h"
#include "src/audio/dRowAudio_SmoothingAudioSource.h"
#include "src/audio/dRowAudio_ReversibleAudioSource.h"
#include "src/audio/dRowAudio_FilteringAudioTransportSource.h"
#include "src/audio/dRowAudio_FilteringAudioFilePlayer.h"

//#include "src/audio/dRowAudio_TriBandAudioThumbnail.h"

#include "src/audio/dRowAudio_AudioUtility.h"
#include "src/audio/dRowAudio_FIFOBuffer.h"
#include "src/audio/dRowAudio_CircularBuffer.h"
#include "src/audio/dRowAudio_Buffer.h"
//#include "src/audio/dRowAudio_BufferArray.h"

//#include "src/audio/MADAudioFormat.h"

#include "src/audio/dRowAudio_EnvelopeFollower.h"

#include "src/audio/filters/dRowAudio_OnePoleFilter.h"
#include "src/audio/filters/dRowAudio_OnePoleOneZeroFilter.h"
#include "src/audio/filters/dRowAudio_TwoPoleTwoZeroFilter.h"
#include "src/audio/filters/dRowAudio_BiquadFilter.h"
#include "src/audio/filters/dRowAudio_CascadedBiquadFilter.h"
#include "src/audio/filters/dRowAudio_CombFilter.h"
#include "src/audio/filters/dRowAudio_AllpassFilter.h"
#include "src/audio/filters/dRowAudio_InterpolatingAllpassFilter.h"
#include "src/audio/filters/dRowAudio_LBCF.h"
#include "src/audio/filters/dRowAudio_DelayRegister.h"
#include "src/audio/filters/dRowAudio_InterpolatingLBCF.h"
#include "src/audio/filters/dRowAudio_TappedDelayLine.h"

#include "src/audio/fft/dRowAudio_FFTOperation.h"
#include "src/audio/fft/dRowAudio_FFTEngine.h"
#include "src/audio/fft/dRowAudio_Window.h"

// maths
#include "src/maths/dRowAudio_MathsUtilities.h"
#include "src/maths/dRowAudio_BezierCurve.h"

// Gui
#include "src/gui/dRowAudio_Layouts.h"
#include "src/gui/lookandfeel/dRowAudio_LookAndFeel.h"

#include "src/gui/dRowAudio_GraphicalComponent.h"
#include "src/gui/dRowAudio_GraphicalComponentManager.h"
#include "src/gui/dRowAudio_SimpleAudioScope.h"
#include "src/gui/dRowAudio_AudioOscilloscope.h"
#include "src/gui/dRowAudio_SegmentedMeter.h"
#include "src/gui/dRowAudio_Meter.h"
#include "src/gui/dRowAudio_StereoMeter.h"
#include "src/gui/dRowAudio_FilmstripSlider.h"
#include "src/gui/dRowAudio_CpuMeter.h"
#include "src/gui/dRowAudio_Clock.h"
#include "src/gui/dRowAudio_CentreAlignViewport.h"
#include "src/gui/dRowAudio_MusicLibraryTable.h"
#include "src/gui/filebrowser/dRowAudio_BasicFileBrowser.h"
#include "src/gui/filebrowser/dRowAudio_ColumnFileBrowser.h"

// audiothumbnail
#include "src/gui/audiothumbnail/dRowAudio_MultipleAudioThumbnailCache.h"
#include "src/gui/audiothumbnail/dRowAudio_PositionableWaveDisplay.h"
#include "src/gui/audiothumbnail/dRowAudio_ColouredPositionableWaveDisplay.h"
#include "src/gui/audiothumbnail/dRowAudio_AbstractDraggableWaveDisplay.h"
#include "src/gui/audiothumbnail/dRowAudio_FilteredAudioThumbnail.h"
#include "src/gui/audiothumbnail/dRowAudio_ColouredAudioThumbnail.h"
#include "src/gui/audiothumbnail/dRowAudio_DraggableWaveDisplay.h"
#include "src/gui/audiothumbnail/dRowAudio_ColouredDraggableWaveDisplay.h"
#include "src/gui/audiothumbnail/dRowAudio_TriBandDraggableWaveDisplay.h"
#include "src/gui/audiothumbnail/dRowAudio_SwitchableDraggableWaveDisplay.h"

// Utility
#include "src/parameters/dRowAudio_Parameter.h"
#include "src/utility/dRowAudio_Constants.h"
#include "src/utility/dRowAudio_Utility.h"
#include "src/utility/dRowAudio_StateVariable.h"
#include "src/utility/dRowAudio_ChangeVariable.h"
#include "src/utility/dRowAudio_XmlHelpers.h"
#include "src/utility/dRowAudio_Comparators.h"
#include "src/utility/dRowAudio_ITunesLibrary.h"
#include "src/utility/dRowAudio_ITunesLibaryParser.h"

END_DROWAUDIO_NAMESPACE

using namespace DROWAUDIO_NAMESPACE;

#endif //_DROWAUDIOHEADER_H_