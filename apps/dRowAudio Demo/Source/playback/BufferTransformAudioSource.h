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

#ifndef __DROWAUDIO_BUFFERTRANSFORMAUDIOSOURCE_H__
#define __DROWAUDIO_BUFFERTRANSFORMAUDIOSOURCE_H__

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class BufferTransformAudioSource :  public AudioSource
{
public:
    //==============================================================================
    BufferTransformAudioSource (AudioSource* source,
                                bool deleteSourceWhenDeleted = false);
    
    /** Destructor. */
    ~BufferTransformAudioSource();
    
    /** Returns all of the settings.
     */
    Buffer& getBuffer()     {   return buffer;    }
        
    //==============================================================================
    /** @internal. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    
    /** @internal. */
    void releaseResources();
    
    /** @internal. */
    void getNextAudioBlock (const AudioSourceChannelInfo& info);
        
private:
    //==============================================================================
    OptionalScopedPointer<AudioSource> source;
    CriticalSection lock;
    Buffer buffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BufferTransformAudioSource);
};

#endif //__DROWAUDIO_BUFFERTRANSFORMAUDIOSOURCE_H__