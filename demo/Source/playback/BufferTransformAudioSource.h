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

#ifndef BUFFER_TRANSFORM_AUDIO_SOURCE_H
#define BUFFER_TRANSFORM_AUDIO_SOURCE_H

#include "../DemoHeader.h"

class BufferTransformAudioSource : public juce::AudioSource
{
public:
    BufferTransformAudioSource (AudioSource* source,
                                bool deleteSourceWhenDeleted = false);

    //==============================================================================
    /** Setting this to true does not apply the buffer. */
    void setBypass (bool shouldBypass);

    /** Returns all of the settings. */
    Buffer& getBuffer() { return buffer; }

    //==============================================================================
    /** @internal */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** @internal */
    void releaseResources() override;
    /** @internal */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& info) override;

private:
    //==============================================================================
    juce::OptionalScopedPointer<AudioSource> source;
    Buffer buffer;
    bool isBypassed;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BufferTransformAudioSource)
};

#endif //BUFFER_TRANSFORM_AUDIO_SOURCE_H
