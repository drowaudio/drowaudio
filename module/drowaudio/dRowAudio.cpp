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


#ifdef DROWAUDIO_HEADER_H
    /** When you add this cpp file to your project, you mustn't include it in a file where you've
        already included any other headers - just put it inside a file on its own, possibly with your config
        flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
        header files that the compiler may be using.
    */
    #error "Incorrect use of DROWAUDIO cpp file"
#endif

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"

#if JUCE_MAC || JUCE_IOS
 #import <Foundation/Foundation.h>
 #import <AudioToolbox/AudioToolbox.h>
#endif

#if JUCE_IOS
 #import <AVFoundation/AVFoundation.h>
 #import <MediaPlayer/MediaPlayer.h>
#endif

#include "dRowAudio.h"

#if JUCE_MSVC
    #pragma warning (push)
    #pragma warning (disable: 4458)
#endif

#if DROWAUDIO_USE_SOUNDTOUCH
 #include "audio/soundtouch/SoundTouch_Source.cpp"
#endif

#if DROWAUDIO_USE_CURL && JUCE_WINDOWS
  #pragma comment (lib, "Wldap32.lib")
  #pragma comment (lib, "ws2_32.lib")

 #if JUCE_DEBUG
  #pragma comment (lib, __FILE__"/../network/curl/lib/libcurldebug.lib")
 #else
  #pragma comment (lib, __FILE__"/../network/curl/lib/libcurlrelease.lib")
 #endif
#endif

namespace drow
{
    #include "audio/dRowAudio_AudioFilePlayer.cpp"
    #include "audio/dRowAudio_AudioFilePlayerExt.cpp"
    #include "audio/dRowAudio_AudioSampleBufferAudioFormat.cpp"
    #include "audio/dRowAudio_SoundTouchProcessor.cpp"
    #include "audio/dRowAudio_SoundTouchAudioSource.cpp"
    #include "audio/dRowAudio_FilteringAudioSource.cpp"
    #include "audio/dRowAudio_ReversibleAudioSource.cpp"
    #include "audio/dRowAudio_LoopingAudioSource.cpp"
    #include "audio/dRowAudio_PitchDetector.cpp"
    #include "audio/dRowAudio_AudioUtilityUnitTests.cpp"
    #include "audio/dRowAudio_EnvelopeFollower.cpp"
    #include "audio/dRowAudio_SampleRateConverter.cpp"
    #include "audio/filters/dRowAudio_BiquadFilter.cpp"
    #include "audio/filters/dRowAudio_OnePoleFilter.cpp"
    #include "audio/fft/dRowAudio_Window.cpp"
    #include "audio/fft/dRowAudio_FFT.cpp"
    #include "audio/fft/dRowAudio_LTAS.cpp"
    #include "gui/dRowAudio_AudioFileDropTarget.cpp"
    #include "gui/dRowAudio_DefaultColours.cpp"
    #include "gui/dRowAudio_GraphicalComponent.cpp"
    #include "gui/dRowAudio_AudioOscilloscope.cpp"
    #include "gui/dRowAudio_AudioTransportCursor.cpp"
    #include "gui/dRowAudio_SegmentedMeter.cpp"
    #include "gui/dRowAudio_Sonogram.cpp"
    #include "gui/dRowAudio_Spectrograph.cpp"
    #include "gui/dRowAudio_Spectroscope.cpp"
    #include "gui/dRowAudio_TriggeredScope.cpp"
    #include "gui/dRowAudio_CpuMeter.cpp"
    #include "gui/dRowAudio_Clock.cpp"
    #include "gui/dRowAudio_MusicLibraryTable.cpp"
    #include "gui/filebrowser/dRowAudio_BasicFileBrowser.cpp"
    #include "gui/filebrowser/dRowAudio_ColumnFileBrowser.cpp"
    #include "gui/audiothumbnail/dRowAudio_AudioThumbnailImage.cpp"
    #include "gui/audiothumbnail/dRowAudio_ColouredAudioThumbnail.cpp"
    #include "gui/audiothumbnail/dRowAudio_PositionableWaveDisplay.cpp"
    #include "gui/audiothumbnail/dRowAudio_DraggableWaveDisplay.cpp"
    #include "maths/dRowAudio_MathsUnitTests.cpp"
   #if JUCE_IOS
    #include "native/dRowAudio_AudioPicker.mm"
    #include "native/dRowAudio_AVAssetAudioFormat.mm"
    #include "native/dRowAudio_IOSAudioConverter.mm"
   #endif
    #include "parameters/dRowAudio_PluginParameter.cpp"
   #if DROWAUDIO_USE_CURL
    #include "network/dRowAudio_CURLManager.cpp"
    #include "network/dRowAudio_CURLEasySession.cpp"
   #endif
    #include "streams/dRowAudio_MemoryInputSource.cpp"
    #include "utility/dRowAudio_EncryptedString.cpp"
    #include "utility/dRowAudio_ITunesLibrary.cpp"
    #include "utility/dRowAudio_ITunesLibraryParser.cpp"
    #include "utility/dRowAudio_UnityBuilder.cpp"
    #include "utility/dRowAudio_UnityProjectBuilder.cpp"
}

#if JUCE_MSVC
    #pragma warning (pop)
#endif
