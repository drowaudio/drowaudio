/*
    ==============================================================================

    This file is part of the dRowAudio JUCE module
    Copyright 2004-13 by dRowAudio.

    ------------------------------------------------------------------------------

    dRowAudio is provided under the terms of The MIT License (MIT):

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    ==============================================================================
*/


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.

#if 0
BEGIN_JUCE_MODULE_DECLARATION

    ID:             dRowAudio
    vendor:         jrlanglois
    name:           dRowAudio audio and gui classes
    version:        4.3.0
    description:    Extensions to the JUCE library including graphical components for audio
    website:        http://www.drowaudio.co.uk
    license:        MIT

    dependencies:   juce_audio_basics, juce_audio_devices, juce_audio_formats, juce_audio_utils, juce_core, juce_cryptography, juce_data_structures, juce_events, juce_graphics, juce_gui_basics

    OSXFrameworks   Accelerate
    iOSFrameworks   Accelerate AVFoundation MediaPlayer CoreMedia

END_JUCE_MODULE_DECLARATION

#endif

*******************************************************************************/


#ifndef DROWAUDIO_HEADER_H
#define DROWAUDIO_HEADER_H

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

    dRowAudio requires recent versions of JUCE. It won't work with versions 2.28 or
    earlier. To use the library it is necessary to first download JUCE to a
    location where your development environment can find it. Or, you can use your
    existing installation of JUCE.

    To use the module simply include it, or a symbolic link to it, in your
    juce/modules folder. Simply them run the Introjucer as normal and tick the
    dRowAudio module. Config flags are provided to disable some functionality if
    not required.

    ## License

    Copyright (C) 2013 by David Rowland ([e-mail][0])

    dRowAudio is provided under the terms of The MIT License (MIT):

    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in the
    Software without restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
    Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

    Some portions of the software including but not limited to [SoundTouch][5] and
    [FFTReal][6] are included with in the repository but released under separate
    licences. Please see the individual source files for details.

    [0]: mailto:dave@drowaudio.co.uk "David Rowland (Email)"
    [1]: http://www.rawmaterialsoftware.com/wiki/index.php/Coding_Standards
    [2]: https://github.com/drowaudio/drowaudio
    [3]: http://drowaudio.co.uk/docs/
    [4]: http://www.rawmaterialsoftware.com/wiki/index.php/DRowAudio
    [5]: http://www.surina.net/soundtouch/index.html
    [6]: http://ldesoras.free.fr/prod.html
    [7]: http://www.gnu.org/licenses/gpl-2.0.html
    [8]: http://www.opensource.org/licenses/mit-license.html "The MIT License"

    @author David Rowland (<a href="mailto:dave@drowaudio.co.uk">email</a>)
    @version 1.0
    @date 2008
    @copyright Copyright (C) 2013 by David Rowland.
    @copyright Provided under the [The MIT License][5]
*/

//=============================================================================
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_cryptography/juce_cryptography.h>

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

    Not available when building RTAS plugins.
*/
#ifndef DROWAUDIO_USE_FFTREAL
    #if ! JUCE_MAC
        #define DROWAUDIO_USE_FFTREAL 0
    #endif
#endif

#if JucePlugin_Build_RTAS
    #undef DROWAUDIO_USE_FFTREAL
    #define DROWAUDIO_USE_FFTREAL 0
#endif

/** Config: DROWAUDIO_USE_VDSP
    Will use VDSP where available.
    MacOSX and iOS only.
*/
#ifndef DROWAUDIO_USE_VDSP
    #define DROWAUDIO_USE_VDSP 0
#endif

#if ! (JUCE_MAC || JUCE_IOS)
    #undef DROWAUDIO_USE_VDSP
    #define DROWAUDIO_USE_VDSP 0
#endif

/** Config: DROWAUDIO_USE_SOUNDTOUCH
    Enables the SoundTouch library and the associated SoundTouch classes for
    independant pitch and tempo scaling. By default this is enabled.
*/
#ifndef DROWAUDIO_USE_SOUNDTOUCH
    #define DROWAUDIO_USE_SOUNDTOUCH 0
#endif

/** Config: DROWAUDIO_USE_CURL
    Enables the cURL library and the associated network classes.
    By default this is enabled.

    On Windows, this is only available for 32-bit projects.
*/
#ifndef DROWAUDIO_USE_CURL
    #define DROWAUDIO_USE_CURL 0

    #if JUCE_WINDOWS && JUCE_32BIT
        #undef CURL_STATICLIB
        #define CURL_STATICLIB 1
    #endif
#endif

#if JUCE_WINDOWS// && JUCE_64BIT
    #undef CURL_STATICLIB
    #undef DROWAUDIO_USE_CURL
#endif

//=============================================================================
#if JUCE_MSVC
    #pragma warning (push)
    #pragma warning (disable: 4458)
#endif

//=============================================================================
// fftReal needs to be outside of the drow namespace
#if DROWAUDIO_USE_FFTREAL
    #include "audio/fft/fftreal/FFTReal.h"
#endif

//=============================================================================
namespace drow
{
    using namespace juce;
    using juce::MemoryBlock;

    #include "audio/dRowAudio_AudioFilePlayer.h"
    #include "audio/dRowAudio_AudioFilePlayerExt.h"
    #include "audio/dRowAudio_AudioSampleBufferAudioFormat.h"
    #include "audio/dRowAudio_AudioUtility.h"
    #include "audio/dRowAudio_Buffer.h"
    #include "audio/dRowAudio_EnvelopeFollower.h"
    #include "audio/dRowAudio_FifoBuffer.h"
    #include "audio/dRowAudio_FilteringAudioSource.h"
    #include "audio/dRowAudio_LoopingAudioSource.h"
    #include "audio/dRowAudio_Pitch.h"
    #include "audio/dRowAudio_PitchDetector.h"
    #include "audio/dRowAudio_ReversibleAudioSource.h"
    #include "audio/dRowAudio_SampleRateConverter.h"
    #include "audio/dRowAudio_SoundTouchAudioSource.h"
    #include "audio/dRowAudio_SoundTouchProcessor.h"
    #include "audio/fft/dRowAudio_FFT.h"
    #include "audio/fft/dRowAudio_LTAS.h"
    #include "audio/fft/dRowAudio_Window.h"
    #include "audio/filters/dRowAudio_BiquadFilter.h"
    #include "audio/filters/dRowAudio_OnePoleFilter.h"
    #include "gui/audiothumbnail/dRowAudio_AudioThumbnailImage.h"
    #include "gui/audiothumbnail/dRowAudio_ColouredAudioThumbnail.h"
    #include "gui/audiothumbnail/dRowAudio_DraggableWaveDisplay.h"
    #include "gui/audiothumbnail/dRowAudio_PositionableWaveDisplay.h"
    #include "gui/dRowAudio_AudioFileDropTarget.h"
    #include "gui/dRowAudio_AudioOscilloscope.h"
    #include "gui/dRowAudio_AudioTransportCursor.h"
    #include "gui/dRowAudio_CentreAlignViewport.h"
    #include "gui/dRowAudio_Clock.h"
    #include "gui/dRowAudio_CpuMeter.h"
    #include "gui/dRowAudio_DefaultColours.h"
    #include "gui/dRowAudio_GraphicalComponent.h"
    #include "gui/dRowAudio_GuiHelpers.h"
    #include "gui/dRowAudio_MusicLibraryTable.h"
    #include "gui/dRowAudio_SegmentedMeter.h"
    #include "gui/dRowAudio_Sonogram.h"
    #include "gui/dRowAudio_Spectrograph.h"
    #include "gui/dRowAudio_Spectroscope.h"
    #include "gui/dRowAudio_TriggeredScope.h"
    #include "gui/filebrowser/dRowAudio_BasicFileBrowser.h"
    #include "gui/filebrowser/dRowAudio_ColumnFileBrowser.h"
    #include "gui/filebrowser/dRowAudio_ColumnFileBrowserLookAndFeel.h"
    #include "maths/dRowAudio_BezierCurve.h"
    #include "maths/dRowAudio_CumulativeMovingAverage.h"
    #include "maths/dRowAudio_MathsUtilities.h"
    #include "native/dRowAudio_AudioPicker.h"
    #include "native/dRowAudio_AVAssetAudioFormat.h"
    #include "native/dRowAudio_IOSAudioConverter.h"
    #include "network/dRowAudio_CURLEasySession.h"
    #include "network/dRowAudio_CURLManager.h"
    #include "parameters/dRowAudio_PluginParameter.h"
    #include "streams/dRowAudio_MemoryInputSource.h"
    #include "streams/dRowAudio_StreamAndFileHandler.h"
    #include "utility/dRowAudio_Comparators.h"
    #include "utility/dRowAudio_Constants.h"
    #include "utility/dRowAudio_DebugObject.h"
    #include "utility/dRowAudio_EncryptedString.h"
    #include "utility/dRowAudio_ITunesLibrary.h"
    #include "utility/dRowAudio_ITunesLibraryParser.h"
    #include "utility/dRowAudio_LockedPointer.h"
    #include "utility/dRowAudio_MusicLibraryHelpers.h"
    #include "utility/dRowAudio_StateVariable.h"
    #include "utility/dRowAudio_UnityBuilder.h"
    #include "utility/dRowAudio_UnityProjectBuilder.h"
    #include "utility/dRowAudio_Utility.h"
    #include "utility/dRowAudio_XmlHelpers.h"
}

#ifdef __clang__
    #pragma clang diagnostic pop
#endif

#if JUCE_MSVC
    #pragma warning (pop)
#endif

#endif //DROWAUDIO_HEADER_H
