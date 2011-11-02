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

#ifndef _DROWAUDIO_SOUNDTOUCHPROCESSOR__H_
    #include "audio/dRowAudio_SoundTouchProcessor.h"
#endif

#ifndef _DROWAUDIO_SOUNDTOUCHAUDIOSOURCE__H_
    #include "audio/dRowAudio_SoundTouchAudioSource.h"
#endif

#ifndef __DROWAUDIO_FILTERINGAUDIOSOURCE_H__
    #include "audio/dRowAudio_FilteringAudioSource.h"
#endif
//#include "src/audio/dRowAudio_SmoothingAudioSource.h"
#ifndef __DROWAUDIO_REVERSIBLEAUDIOSOURCE_H__
    #include "audio/dRowAudio_ReversibleAudioSource.h"
#endif
#ifndef __DROWAUDIO_LOOPINGAUDIOSOURCE_H__
    #include "audio/dRowAudio_LoopingAudioSource.h"
#endif
//#include "src/audio/dRowAudio_FilteringAudioTransportSource.h"
//#include "src/audio/dRowAudio_FilteringAudioFilePlayer.h"
//
////#include "src/audio/dRowAudio_TriBandAudioThumbnail.h"
//
#ifndef __DROWAUDIO_AUDIOUTILITY_H__
    #include "audio/dRowAudio_AudioUtility.h"
#endif
#ifndef __DROWAUDIO_FIFOBUFFER_H__
    #include "audio/dRowAudio_FifoBuffer.h"
#endif
//#include "src/audio/dRowAudio_CircularBuffer.h"
//#include "src/audio/dRowAudio_Buffer.h"
////#include "src/audio/dRowAudio_BufferArray.h"
//
//#include "src/audio/dRowAudio_EnvelopeFollower.h"
//
//#include "src/audio/filters/dRowAudio_OnePoleFilter.h"
//#include "src/audio/filters/dRowAudio_OnePoleOneZeroFilter.h"
//#include "src/audio/filters/dRowAudio_TwoPoleTwoZeroFilter.h"
#ifndef __DROWAUDIO_BIQUADFILTER_H__
    #include "audio/filters/dRowAudio_BiquadFilter.h"
#endif
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

// Gui
#ifndef __DROWAUDIO_AUDIOFILEDROPTARGET_H__
    #include "gui/dRowAudio_AudioFileDropTarget.h"
#endif
//#include "src/gui/dRowAudio_Layouts.h"
//#include "src/gui/lookandfeel/dRowAudio_LookAndFeel.h"
//
#ifndef __DROWAUDIO_GRAPHICALCOMPONENT_H__
    #include "gui/dRowAudio_GraphicalComponent.h"
#endif
#ifndef __DROWAUDIO_GRAPHICALCOMPONENTMANAGER_H__
    #include "gui/dRowAudio_GraphicalComponentManager.h"
#endif
//#include "src/gui/dRowAudio_SimpleAudioScope.h"
//#include "src/gui/dRowAudio_AudioOscilloscope.h"
#ifndef __DROWAUDIO_SEGMENTEDMETER_H__
    #include "gui/dRowAudio_SegmentedMeter.h"
#endif
//#include "src/gui/dRowAudio_Meter.h"
//#include "src/gui/dRowAudio_StereoMeter.h"
//#include "src/gui/dRowAudio_FilmstripSlider.h"
#ifndef __DROWAUDIO_CPUMETER_H__
    #include "gui/dRowAudio_CpuMeter.h"
#endif
#ifndef __DROWAUDIO_CLOCK_H__
    #include "gui/dRowAudio_Clock.h"
#endif
#ifndef __DROWAUDIO_CENTREALIGNVIEWPORT_H__
    #include "gui/dRowAudio_CentreAlignViewport.h"
#endif
#ifndef __DROWAUDIO_MUSICLIBRARYTABLE_H__
    #include "gui/dRowAudio_MusicLibraryTable.h"
#endif
#ifndef __DROWAUDIO_BASICFILEBROWSER_H__
    #include "gui/filebrowser/dRowAudio_BasicFileBrowser.h"
#endif
#ifndef __DROWAUDIO_COLUMNFILEBROWSERLOOKANDFEEL_H__
    #include "gui/filebrowser/dRowAudio_ColumnFileBrowserLookAndFeel.h"
#endif
#ifndef __DROWAUDIO_COLUMNFILEBROWSER_H__
    #include "gui/filebrowser/dRowAudio_ColumnFileBrowser.h"
#endif

// audiothumbnail
#ifndef __DROWAUDIO_AUDIOTHUMBNAILIMAGE_H__
    #include "gui/audiothumbnail/dRowAudio_AudioThumbnailImage.h"
#endif
#ifndef __DROWAUDIO_MULTIPLEAUDIOTHUMBNAILCACHE_H__
    #include "gui/audiothumbnail/dRowAudio_MultipleAudioThumbnailCache.h"
#endif
#ifndef __DROWAUDIO_FILTEREDAUDIOTHUMBNAIL_H__
    #include "gui/audiothumbnail/dRowAudio_FilteredAudioThumbnail.h"
#endif

#ifndef __DROWAUDIO_COLOUREDAUDIOTHUMBNAIL_H__
    #include "gui/audiothumbnail/dRowAudio_ColouredAudioThumbnail.h"
#endif

#ifndef __DROWAUDIO_POSITIONALWAVEDISPLAY_H__
    #include "gui/audiothumbnail/dRowAudio_PositionableWaveDisplay.h"
#endif

#ifndef __DROWAUDIO_COLOUREDPOSITIONABLEWAVEDISPLAY_H__
    #include "gui/audiothumbnail/dRowAudio_ColouredPositionableWaveDisplay.h"
#endif
#ifndef __DROWAUDIO_ABSTRACTDRAGGABLEWAVEDISPLAY_H__
    #include "gui/audiothumbnail/dRowAudio_AbstractDraggableWaveDisplay.h"
#endif
//#include "src/gui/audiothumbnail/dRowAudio_DraggableWaveDisplay.h"
#ifndef __DROWAUDIO_COLOUREDDRAGGABLEWAVEDISPLAY_H__
    #include "gui/audiothumbnail/dRowAudio_ColouredDraggableWaveDisplay.h"
#endif
//#include "src/gui/audiothumbnail/dRowAudio_TriBandDraggableWaveDisplay.h"
//#include "src/gui/audiothumbnail/dRowAudio_SwitchableDraggableWaveDisplay.h"

// Utility
#ifndef __DROWAUDIO_PLUGINPARAMETER_H__
    #include "parameters/dRowAudio_PluginParameter.h"
#endif _DROWAUDIO_PLUGINPARAMETER_H

#ifndef __DROWAUDIO_CONSTANTS_H__
    #include "utility/dRowAudio_Constants.h"
#endif

#ifndef __DROWAUDIO_UTILITY_H__
    #include "utility/dRowAudio_Utility.h"
#endif

#ifndef __DROWAUDIO_STATEVARIABLE_H__
    #include "utility/dRowAudio_StateVariable.h"
#endif

//#include "src/utility/dRowAudio_ChangeVariable.h"
#ifndef __DROWAUDIO_XMLHELPERS_H__
    #include "utility/dRowAudio_XmlHelpers.h"
#endif

#ifndef __DROWAUDIO_COMPARATORS_H__
    #include "utility/dRowAudio_Comparators.h"
#endif
#ifndef __DROWAUDIO_MUSICLIBRARYHELPERS_H__
    #include "utility/dRowAudio_MusicLibraryHelpers.h"
#endif __DROWAUDIO_MUSICLIBRARYHELPERS_H__
#ifndef __DROWAUDIO_ITUNESLIBRARY_H__
    #include "utility/dRowAudio_ITunesLibrary.h"
#endif
#ifndef __DROWAUDIO_ITUNESLIBRARYPARSER_H__
    #include "utility/dRowAudio_ITunesLibraryParser.h"
#endif

END_JUCE_NAMESPACE

#endif //_DROWAUDIOHEADER_H_