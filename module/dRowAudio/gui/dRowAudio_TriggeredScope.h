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

#ifndef DROWAUDIO_TRIGGERED_SCOPE_H
#define DROWAUDIO_TRIGGERED_SCOPE_H

/** Triggered Scope.

    This class is similar to the AudioOscilloscope except that it can be set to
    start on a rising or falling signal. This makes it extremely useful for very
    zoomed-in waveform viewing.

    At the expense of a large memory footpring this is also highly efficient,
    performing all its processing and image rendering on a background thread.
    This makes it suitable for use in time critical situationas such as audio
    plugins. The addSamples method simply takes a copy of the samples, weverything
    else happens later.
*/
class TriggeredScope : public Component,
                       public Timer,
                       public TimeSliceClient
{
public:

    /** Creates a Triggered scope.

        You should really specify a backGroundThread to use and make sure you start
        it before creating the scope.

        If you pass a nullptr in here, it will create its own thread
        and manage its lifetime internally.
    */
    TriggeredScope (TimeSliceThread* backgroundThreadToUse = nullptr);

    /** Destructor. */
    ~TriggeredScope();
    
    enum ColourIds
    {
        lineColourId             = 0x1231e10,
        backgroundColourId       = 0x1231e11, 
        traceColourId            = 0x1231e12
    };
    
    void setNumChannels (int num);
    
    //==============================================================================
    /** Sets the number of samples represented by each pixel on the scope.
        Setting this to a low number will give a very zoomed in display, a high
        number zoom out.
     */
    void setNumSamplesPerPixel (int newNumSamplesPerPixel);

    /** Sets the vertical zoom factor of the display. */
    void setVerticalZoomFactor (float newVerticalZoomFactor);

    /** Sets the vertical zoom offset of the display. */
    void setVerticalZoomOffset (float newVerticalZoomOffset, int ch);

    //==============================================================================
    /** The enum to use when setting the trace trigger mode. */
    enum TriggerMode
    {
        None = 0,   //< The trace will just refresh at a constant rate.
        Up,         //< The start of the trace will be a rising edge.
        Down        //< The start of the trace will be a falling edge.
    };

    /** Sets the type of change that will trigger a trace. */
    void setTriggerMode (TriggerMode newTriggerMode);
    
    /** Set channel to trigger on, -1 ave of all channels */
    void setTriggerChannel (int ch) { triggerChannel = ch; }
    
    /** Set level to trigger on */
    void setTriggerLevel (float l)  { triggerLevel = l;     }

    void setTriggerPos (float l)    { triggerPos = l;       }
    
    void setDrawTriggerPos (bool d) { drawTriggerPos = d;   }

    //==============================================================================
    /** Adds a block of samples to the scope.

        Simply call this from your audio callback or similar to render the scope.
        This is a very quick method as it only takes a copy of the samples. All the
        processing and image rendering is performed on a backgroudn thread.
    */
    void addSamples (const float* samples, int numSamples);
    
    void addSamples (const AudioSampleBuffer& buffer);

    //==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void timerCallback() override;
    /** @internal */
    int useTimeSlice() override;

private:
    //==============================================================================
    OptionalScopedPointer<TimeSliceThread> backgroundThreadToUse;

    TriggerMode triggerMode;
    int numSamplesPerPixel;
    float verticalZoomFactor;
    Array<float> verticalZoomOffset;
    float triggerLevel = 0.0f;
    float triggerPos = 0.0f;
    int triggerChannel = -1;
    bool drawTriggerPos = false;
    
    struct Channel
    {
        Channel() :
          numLeftToAverage (4),
          bufferSize (4096),
          bufferWritePos (0),
          minBuffer ((size_t) bufferSize),
          maxBuffer ((size_t) bufferSize),
          currentMax (-1.0f),
          currentMin (1.0f),
          samplesToProcess (32768),
          tempProcessingBlock (32768)
        {}
        
        int numLeftToAverage;
        int bufferSize, bufferWritePos;

        HeapBlock<float> minBuffer, maxBuffer;

        float currentMax, currentMin;
        FifoBuffer<float> samplesToProcess;
        HeapBlock<float> tempProcessingBlock;
    };
    
    OwnedArray<Channel> channels;
    
    bool needToUpdate;

    bool needToRepaint;
    Image image;
    CriticalSection imageLock;

    //==============================================================================
    void processPendingSamples();
    void renderImage();
    int getTriggerPos();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggeredScope)
};

#endif  // DROWAUDIO_TRIGGERED_SCOPE_H
