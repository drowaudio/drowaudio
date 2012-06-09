/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

#ifndef _DROWAUDIOHEADER_H_
#define _DROWAUDIOHEADER_H_

/**
  @mainpage dRowAudio - A JUCE module for high level audio application development.

  @details

  dRowAudio is a 3rd party JUCE module designed for rapid audio application
  development. It contains classes for audio processing and gui elements.
  Additionally there are several wrappers around 3rd party libraries including
  cURL, FFTReal and SoundTouch. dRowAudio is written in the strict JUCE style,
  closely following the style guide set out at [JUCE Coding Standards][1].

  dRowAudio is hosted on Github at [https://github.com/drowaudio/drowaudio][2]

  The online documentation is at [http://drowaudio.co.uk/docs/][3]

  A wiki with feature overview and demo screenshots can be found at
  [http://www.rawmaterialsoftware.com/wiki/index.php/DRowAudio][4]

  ## Platforms

  All platforms supported by JUCE are also supported by dRowAudio. Currently these
  platforms include:

  - **Windows**: Applications and VST/RTAS/NPAPI/ActiveX plugins can be built 
  using MS Visual Studio. The results are all fully compatible with Windows
  XP, Vista or Windows 7.

  - **Mac OS X**: Applications and VST/AudioUnit/RTAS/NPAPI plugins with Xcode.

  - **GNU/Linux**: Applications and plugins can be built for any kernel 2.6 or
  later.

  - **iOS**: Native iPhone and iPad apps.

  - **Android**: Supported.

  ## Prerequisites

  This documentation assumes that the reader has a working knowledge of JUCE.

  ## External Modules

  In order to use the cURL classes you will need to link to the cURL library.
  This is included as part of Mac OSX, for Windows there pre-built 32-bit binaries
  or you can download the library yourself for the most recent version.
  Instructions for linkage are detailed on the [dRowAudio wiki][4].

  Although some aspects of dRowAudio rely on other 3rd party modules such as
  [SoundTouch][5] and [FFTReal][6], these are included as part of the module so
  no external linking is required. Their use should be transparent to the user.

  ## Integration

  dRowAudio requires recent versions of JUCE. It won't work with versions 2.20 or
  earlier. To use the library it is necessary to first download JUCE to a
  location where your development environment can find it. Or, you can use your
  existing installation of JUCE.

  To use the module simply include it, or a symbolic link to it, in your
  juce/modules folder. Simply them run the Introjucer as normal and tick the
  dRowAudio module. Config flags are provided to disable some functionality if
  not required.

  ## License

  Copyright (C) 2010 by David Rowland ([e-mail][0])

  Unless otherwise marked, files in dRowAudio are published under the [GNU General
  Public License v2][7]. A copy of the licence is contained within each source file.
  Commercial licences are available on request please [contact][0] the author for
  details.

  [0]: mailto:dave@drowaudio.co.uk "David Rowland (Email)"
  [1]: http://www.rawmaterialsoftware.com/wiki/index.php/Coding_Standards
  [2]: https://github.com/drowaudio/drowaudio
  [3]: http://drowaudio.co.uk/docs/
  [4]: http://www.rawmaterialsoftware.com/wiki/index.php/DRowAudio
  [5]: http://www.surina.net/soundtouch/index.html
  [6]: http://ldesoras.free.fr/prod.html
  [7]: http://www.gnu.org/licenses/gpl-2.0.html

  @author David Rowland (<a href="mailto:dave@drowaudio.co.uk">email</a>)
  @version 1.0
  @date 2008
  @copyright Copyright (C) 2008 by David Rowland.
  @copyright Provided under the [GNU General Public License][5]
*/

#include "../juce_audio_basics/juce_audio_basics.h"
#include "../juce_audio_devices/juce_audio_devices.h"
#include "../juce_audio_formats/juce_audio_formats.h"
#include "../juce_audio_utils/juce_audio_utils.h"
#include "../juce_core/juce_core.h"
#include "../juce_data_structures/juce_data_structures.h"
#include "../juce_events/juce_events.h"
#include "../juce_graphics/juce_graphics.h"
#include "../juce_gui_basics/juce_gui_basics.h"

#if JUCE_MODULE_AVAILABLE_juce_cryptography
 #include "../juce_cryptography/juce_cryptography.h"
#endif

#if JUCE_MAC || JUCE_IOS
    #define Point CarbonDummyPointName
    #define Component CarbonDummyCompName
    #include <Accelerate/Accelerate.h>
    #undef Point
    #undef Component
#endif

#undef min
#undef max

#ifdef __clang__
 #pragma clang diagnostic push
 #pragma clang diagnostic ignored "-Wunused-function"
#endif

//=============================================================================
/** Config: DROWAUDIO_USE_FFTREAL
    Enables the FFTReal library. By default this is enabled except on the Mac
    where the Accelerate framework is preferred. However, if you do explicity 
    enable this setting fftreal can be used for testing purposes.
 */
#ifndef DROWAUDIO_USE_FFTREAL
    #if (! JUCE_MAC)
        #define DROWAUDIO_USE_FFTREAL 1
    #endif
#endif

/** Config: DROWAUDIO_USE_SOUNDTOUCH
    Enables the SoundTouch library and the associated SoundTouch classes for
    independant pitch and tempo scaling. By default this is enabled.
 */
#ifndef DROWAUDIO_USE_SOUNDTOUCH
    #define DROWAUDIO_USE_SOUNDTOUCH 1
#endif

/** Config: DROWAUDIO_USE_CURL
    Enables the cURL library and the associated network classes. By default
    this is enabled.
 */
#ifndef DROWAUDIO_USE_CURL
    #define DROWAUDIO_USE_CURL 1
#endif

//=============================================================================
// fftReal needs to be outside of the drow namespace
#if DROWAUDIO_USE_FFTREAL
    #include "audio/fft/fftreal/FFTReal.h"
#endif

//=============================================================================
namespace drow {
using namespace juce;
using juce::int64;
using juce::uint32;
using juce::int32;
using juce::MemoryBlock;
//#define MemoryBlock juce::MemoryBlock //*** bit of a nasty hack, better methods?

// Audio
#ifndef __DROWAUDIO_AUDIOFILEPLAYER_H__
    #include "audio/dRowAudio_AudioFilePlayer.h"
#endif

#ifndef __DROWAUDIO_AUDIOFILEPLAYEREXT_H__
    #include "audio/dRowAudio_AudioFilePlayerExt.h"
#endif

#ifndef __DROWAUDIO_AUDIOSAMPLEBUFFERAUDIOFORMAT_H__
    #include "audio/dRowAudio_AudioSampleBufferAudioFormat.h"
#endif

#ifndef __DROWAUDIO_SOUNDTOUCHPROCESSOR_H__
    #include "audio/dRowAudio_SoundTouchProcessor.h"
#endif

#ifndef __DROWAUDIO_SOUNDTOUCHAUDIOSOURCE_H__
    #include "audio/dRowAudio_SoundTouchAudioSource.h"
#endif

#ifndef __DROWAUDIO_FILTERINGAUDIOSOURCE_H__
    #include "audio/dRowAudio_FilteringAudioSource.h"
#endif

#ifndef __DROWAUDIO_REVERSIBLEAUDIOSOURCE_H__
    #include "audio/dRowAudio_ReversibleAudioSource.h"
#endif

#ifndef __DROWAUDIO_LOOPINGAUDIOSOURCE_H__
    #include "audio/dRowAudio_LoopingAudioSource.h"
#endif

#ifndef __DROWAUDIO_AUDIOUTILITY_H__
    #include "audio/dRowAudio_AudioUtility.h"
#endif

#ifndef __DROWAUDIO_FIFOBUFFER_H__
    #include "audio/dRowAudio_FifoBuffer.h"
#endif

#ifndef __DROWAUDIO_BUFFER_H__
    #include "audio/dRowAudio_Buffer.h"
#endif

#ifndef __DROWAUDIO_ENVELOPEFOLLOWER_H__
    #include "audio/dRowAudio_EnvelopeFollower.h"
#endif

#ifndef __DROWAUDIO_SAMPLERATECONVERTER_H__
    #include "audio/dRowAudio_SampleRateConverter.h"
#endif

#ifndef __DROWAUDIO_BIQUADFILTER_H__
    #include "audio/filters/dRowAudio_BiquadFilter.h"
#endif

#ifndef __DROWAUDIO_ONEPOLEFILTER_H__
    #include "audio/filters/dRowAudio_OnePoleFilter.h"
#endif

#ifndef __DROWAUDIO_WINDOW_H__
    #include "audio/fft/dRowAudio_Window.h"
#endif

#ifndef __DROWAUDIO_FFTENGINE_H__
    #include "audio/fft/dRowAudio_FFTEngine.h"
#endif

#ifndef __DROWAUDIO_FFTOPERATION_H__
    #include "audio/fft/dRowAudio_FFTOperation.h"
#endif

#ifndef __DROWAUDIO_LTAS_H__
 #include "audio/fft/dRowAudio_LTAS.h"
#endif

// Gui
#ifndef __DROWAUDIO_AUDIOFILEDROPTARGET_H__
    #include "gui/dRowAudio_AudioFileDropTarget.h"
#endif
#ifndef __DROWAUDIO_GUIHELPERS_H__
    #include "gui/dRowAudio_GuiHelpers.h"
#endif

#ifndef __DROWAUDIO_GRAPHICALCOMPONENT_H__
    #include "gui/dRowAudio_GraphicalComponent.h"
#endif

#ifndef __DROWAUDIO_AUDIOOSCILLOSCOPE_H__
    #include "gui/dRowAudio_AudioOscilloscope.h"
#endif

#ifndef __DROWAUDIO_AUDIOTRANSPORTCUROSR_H__
    #include "gui/dRowAudio_AudioTransportCursor.h"
#endif

#ifndef __DROWAUDIO_SEGMENTEDMETER_H__
    #include "gui/dRowAudio_SegmentedMeter.h"
#endif

#ifndef __DROWAUDIO_SONOGRAM_H__
    #include "gui/dRowAudio_Sonogram.h"
#endif

#ifndef __DROWAUDIO_SPECTROSCOPE_H__
    #include "gui/dRowAudio_Spectroscope.h"
#endif

#ifndef __DROWAUDIO_TRIGGEREDSCOPE_H__
    #include "gui/dRowAudio_TriggeredScope.h"
#endif

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

#ifndef __DROWAUDIO_COLOUREDAUDIOTHUMBNAIL_H__
    #include "gui/audiothumbnail/dRowAudio_ColouredAudioThumbnail.h"
#endif

#ifndef __DROWAUDIO_POSITIONALWAVEDISPLAY_H__
    #include "gui/audiothumbnail/dRowAudio_PositionableWaveDisplay.h"
#endif

#ifndef __DROWAUDIO_DRAGGABLEWAVEDISPLAY_H__
    #include "gui/audiothumbnail/dRowAudio_DraggableWaveDisplay.h"
#endif

// maths
#ifndef __DROWAUDIO_MATHSUTILITIES_H__
    #include "maths/dRowAudio_MathsUtilities.h"
#endif

#ifndef __DROWAUDIO_CUMULATIVEMOVINGAVERAGE_H__
    #include "maths/dRowAudio_CumulativeMovingAverage.h"
#endif 

#ifndef __DROWAUDIO_BEZIERCURVE_H__
    #include "maths/dRowAudio_BezierCurve.h"
#endif

// native
#ifndef __DROWAUDIO_AUDIOPICKER__
    #include "native/dRowAudio_AudioPicker.h"
#endif

#ifndef __DROWAUDIO_AVASSETAUDIOFORMAT_H__
    #include "native/dRowAudio_AVAssetAudioFormat.h"
#endif

#ifndef __DROWAUDIO_IOSAUDIOCONVERTER__
    #include "native/dRowAudio_IOSAudioConverter.h"
#endif

// network
#ifndef __DROWAUDIO_CURLMANAGER_H__
    #include "network/dRowAudio_CURLManager.h"
#endif 

#ifndef __DROWAUDIO_CURLEASYSESSION_H__
    #include "network/dRowAudio_CURLEasySession.h"
#endif 

// streams
#ifndef __DROWAUDIO_MEMORYINPUTSOURCE_H__
    #include "streams/dRowAudio_MemoryInputSource.h"
#endif

// Utility
#ifndef __DROWAUDIO_PLUGINPARAMETER_H__
    #include "parameters/dRowAudio_PluginParameter.h"
#endif

#ifndef __DROWAUDIO_CONSTANTS_H__
    #include "utility/dRowAudio_Constants.h"
#endif

#ifndef __DROWAUDIO_ENCRYPTEDSTRING_H__
    #include "utility/dRowAudio_EncryptedString.h"
#endif

#ifndef __DROWAUDIO_UTILITY_H__
    #include "utility/dRowAudio_Utility.h"
#endif

#ifndef __DROWAUDIO_STATEVARIABLE_H__
    #include "utility/dRowAudio_StateVariable.h"
#endif

#ifndef __DROWAUDIO_XMLHELPERS_H__
    #include "utility/dRowAudio_XmlHelpers.h"
#endif

#ifndef __DROWAUDIO_COMPARATORS_H__
    #include "utility/dRowAudio_Comparators.h"
#endif
#ifndef __DROWAUDIO_MUSICLIBRARYHELPERS_H__
    #include "utility/dRowAudio_MusicLibraryHelpers.h"
#endif
#ifndef __DROWAUDIO_ITUNESLIBRARY_H__
    #include "utility/dRowAudio_ITunesLibrary.h"
#endif
#ifndef __DROWAUDIO_ITUNESLIBRARYPARSER_H__
    #include "utility/dRowAudio_ITunesLibraryParser.h"
#endif

}

#ifdef __clang__
 #pragma clang diagnostic pop
#endif

#endif //_DROWAUDIOHEADER_H_