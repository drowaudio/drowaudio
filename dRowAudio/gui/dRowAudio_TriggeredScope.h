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

#ifndef __DROWAUDIO_TRIGGERED_SCOPE_H__
#define __DROWAUDIO_TRIGGERED_SCOPE_H__

//==============================================================================
/**
    Triggered Scope.
    
    This class is similar to the AudioOscilloscope except that it can be set to
    start on a rising or falling signal. This makes it extremely useful for very
    zommed-in waveform viewing.
    
    At the expense of a large memory footpring this is also highly efficient,
    performing all its processing and image rendering on a background thread.
    This makes it suitable for use in time critical situationas such as audio
    plugins. The addSamples method simply takes a copy of the samples, weverything
    else happens later.
 */
class TriggeredScope :  public Component,
                        public Timer,
                        public TimeSliceClient
{
public:
    //==============================================================================
    /** The enum to use when setting the trace trigger mode.
     */
    enum TriggerMode
    {
        None,   /**<< The trace will just refresh at a constant rate. */
        Up,     /**<< The start of the trace will be a rising edge. */
        Down    /**<< The start of the trace will be a falling edge. */
    };
    
    //==============================================================================
    /** Creates a Triggered scope.
        You should really specify a backGroundThread to use and make sure you start
        it before creating the scope. If you pass a nullptr in here it will create
        its own thread and manage its lifetime internally.
     */
    TriggeredScope (TimeSliceThread* backgroundThreadToUse = nullptr);
    
    /** Destructor. */
    ~TriggeredScope();

    /** Sets the number of samples represented by each pixel on the scope.
        Setting this to a low number will give a very zoomed in display, a high
        number zoom out.
     */
    void setNumSamplesPerPixel (int newNumSamplesPerPixel);
    
    /** Sets the vertical zoom facotr of the display.
     */
    void setVerticalZoomFactor (float newVerticalZoomFactor);
    
    /** Sets the type of change that will trigger a trace.
     */
    void setTriggerMode (TriggerMode newTriggerMode);
    
    /** Adds a block of samples to the scope.
        
        Simply call this from your audio callback or similar to render the scope.
        This is a very quick method as it only takes a copy of the samples. All the
        processing and image rendering is performed on a backgroudn thread.
     */
    void addSamples (const float* samples, int numSamples);

    //==============================================================================
    /** @internal */
    void resized();
    
    /** @internal */
    void paint (Graphics& g);

    /** @internal */
    void timerCallback();
    
    /** @internal */
    int useTimeSlice();
    
private:
    //==============================================================================
    OptionalScopedPointer<TimeSliceThread> backgroundThreadToUse;
    
    int numSamplesPerPixel;
    float verticalZoomFactor;
    TriggerMode triggerMode;
    
    int numLeftToAverage;

    int bufferSize, bufferWritePos;
    HeapBlock<float> minBuffer, maxBuffer;
    
    float currentMax, currentMin;
    FifoBuffer<float> samplesToProcess;
    HeapBlock<float> tempProcessingBlock;
    bool needToUpdate;

    Image image;
    bool needToRepaint;
    CriticalSection imageLock;

    //==============================================================================
    void processPendingSamples();
    void renderImage();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriggeredScope);
};

#endif  // __DROWAUDIO_TRIGGERED_SCOPE_H__
