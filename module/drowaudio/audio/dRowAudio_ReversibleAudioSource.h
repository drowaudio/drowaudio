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

#ifndef DROWAUDIO_REVERSIBLEAUDIOSOURCE_H
#define DROWAUDIO_REVERSIBLEAUDIOSOURCE_H

#include "../utility/dRowAudio_Utility.h"

/** A type of AudioSource that can reverse the stream of samples that
    flows through it.

    @see PositionableAudioSource, AudioTransportSource, BufferingAudioSource
*/
class ReversibleAudioSource : public AudioSource
{
public:
    /** Creates an ReversableAudioFormatReaderSource for a given reader.

        @param sourceReader                     the reader to use as the data source
        @param deleteReaderWhenThisIsDeleted    if true, the reader passed-in will be deleted
                                                when this object is deleted; if false it will be
                                                left up to the caller to manage its lifetime
    */
    ReversibleAudioSource (PositionableAudioSource* const inputSource,
                           const bool deleteInputWhenDeleted);

    //==============================================================================
    /** Sets whether the source should play forwards or backwards.  */
    void setPlayDirection (bool shouldPlayForwards) { isForwards = shouldPlayForwards; }

    /** @returns True if the source is playing forwards. */
    bool isPlayingForwards() const { return isForwards; }

    //==============================================================================
    /** @internal */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /** @internal */
    void releaseResources() override;
    /** @internal */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;

private:
    //==============================================================================
    OptionalScopedPointer<PositionableAudioSource> input;
    int64 previousReadPosition;
    bool volatile isForwards;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReversibleAudioSource)
};

#endif   // DROWAUDIO_REVERSIBLEAUDIOSOURCE_H
