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


#ifdef __DROWAUDIO_JUCEHEADER__
    /*  When you add this cpp file to your project, you mustn't include it in a file where you've
        already included any other headers - just put it inside a file on its own, possibly with your config
        flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
        header files that the compiler may be using.
     */
#error "Incorrect use of DROWAUDIO cpp file"
#endif

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"

#include "../../../juce_source/juce/modules/juce_core/native/juce_BasicNativeHeaders.h"

#if JUCE_MAC || JUCE_IOS
    #import <Foundation/Foundation.h>
    #import <AudioToolbox/AudioToolbox.h>
#endif

#if JUCE_IOS
    #import <AVFoundation/AVFoundation.h>
#endif

#include "dRowAudio.h"

// Audio
#include "audio/dRowAudio_AudioFilePlayer.cpp"
#include "audio/dRowAudio_AudioFilePlayerExt.cpp"

#include "audio/soundtouch/SoundTouch_Source.cpp"
#include "audio/dRowAudio_SoundTouchProcessor.cpp"
#include "audio/dRowAudio_SoundTouchAudioSource.cpp"

#include "audio/dRowAudio_FilteringAudioSource.cpp"
//#include "src/audio/dRowAudio_SmoothingAudioSource.cpp"
#include "audio/dRowAudio_ReversibleAudioSource.cpp"
#include "audio/dRowAudio_LoopingAudioSource.cpp"
//#include "src/audio/dRowAudio_FilteringAudioTransportSource.cpp"
//#include "src/audio/dRowAudio_FilteringAudioFilePlayer.cpp"
//
//#include "src/audio/dRowAudio_CircularBuffer.cpp"
#include "audio/dRowAudio_Buffer.cpp"
//#include "src/audio/dRowAudio_BufferArray.cpp"
//
//#include "src/audio/dRowAudio_EnvelopeFollower.cpp"
//
//#include "src/audio/filters/dRowAudio_OnePoleFilter.cpp"
//#include "src/audio/filters/dRowAudio_OnePoleOneZeroFilter.cpp"
//#include "src/audio/filters/dRowAudio_TwoPoleTwoZeroFilter.cpp"
#include "audio/filters/dRowAudio_BiquadFilter.cpp"
//#include "src/audio/filters/dRowAudio_CascadedBiquadFilter.cpp"
//#include "src/audio/filters/dRowAudio_CombFilter.cpp"
//#include "src/audio/filters/dRowAudio_AllpassFilter.cpp"
//#include "src/audio/filters/dRowAudio_InterpolatingAllpassFilter.cpp"
//#include "src/audio/filters/dRowAudio_LBCF.cpp"
//#include "src/audio/filters/dRowAudio_InterpolatingLBCF.cpp"
//#include "src/audio/filters/dRowAudio_DelayRegister.cpp"
//#include "src/audio/filters/dRowAudio_TappedDelayLine.cpp"
//
#include "audio/fft/dRowAudio_Window.cpp"
#include "audio/fft/dRowAudio_FFTEngine.cpp"
#include "audio/fft/dRowAudio_mac_FFTOperation.cpp"

// Gui
#include "gui/dRowAudio_AudioFileDropTarget.cpp"

// network
#include "network/curl/dRowAudio_CURLManager.cpp"
#include "network/curl/dRowAudio_CURLEasySession.cpp"

// streams
#include "streams/dRowAudio_MemoryInputSource.cpp"

//#include "src/gui/lookandfeel/dRowAudio_PluginLookAndFeel.cpp"
//#include "src/gui/lookandfeel/GuitarAmpKnobLookAndFeel.cpp"
//
#include "gui/dRowAudio_GraphicalComponent.cpp"
#include "gui/dRowAudio_GraphicalComponentManager.cpp"
//#include "src/gui/dRowAudio_SimpleAudioScope.cpp"
#include "gui/dRowAudio_AudioOscilloscope.cpp"
#include "gui/dRowAudio_SegmentedMeter.cpp"
//#include "src/gui/dRowAudio_Meter.cpp"
//#include "src/gui/dRowAudio_StereoMeter.cpp"
//#include "src/gui/dRowAudio_FilmstripSlider.cpp"
#include "gui/dRowAudio_CpuMeter.cpp"
#include "gui/dRowAudio_Clock.cpp"
#include "gui/dRowAudio_CentreAlignViewport.cpp"
#include "gui/dRowAudio_MusicLibraryTable.cpp"
#include "gui/filebrowser/dRowAudio_BasicFileBrowser.cpp"
#include "gui/filebrowser/dRowAudio_ColumnFileBrowser.cpp"
//

#include "gui/audiothumbnail/dRowAudio_AudioThumbnailImage.cpp"
#include "gui/audiothumbnail/dRowAudio_MultipleAudioThumbnailCache.cpp"
#include "gui/audiothumbnail/dRowAudio_PositionableWaveDisplay.cpp"
#include "gui/audiothumbnail/dRowAudio_ColouredPositionableWaveDisplay.cpp"
#include "gui/audiothumbnail/dRowAudio_AbstractDraggableWaveDisplay.cpp"
#include "gui/audiothumbnail/dRowAudio_FilteredAudioThumbnail.cpp"
#include "gui/audiothumbnail/dRowAudio_ColouredAudioThumbnail.cpp"
#include "gui/audiothumbnail/dRowAudio_DraggableWaveDisplay.cpp"
#include "gui/audiothumbnail/dRowAudio_ColouredDraggableWaveDisplay.cpp"
//#include "src/gui/audiothumbnail/dRowAudio_TriBandDraggableWaveDisplay.cpp"
//#include "src/gui/audiothumbnail/dRowAudio_SwitchableDraggableWaveDisplay.cpp"

// Utility
#include "utility/dRowAudio_ITunesLibrary.cpp"
#include "utility/dRowAudio_ITunesLibraryParser.cpp"
#include "parameters/dRowAudio_PluginParameter.cpp"

#if JUCE_IOS
    #include "audio/dRowAudio_AVAssetAudioFormat.mm"
#endif
