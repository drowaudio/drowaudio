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

TriggeredScope::TriggeredScope (TimeSliceThread* backgroundThreadToUse_)
    : backgroundThreadToUse (backgroundThreadToUse_, backgroundThreadToUse_ == nullptr ? true : false),
      numSamplesPerPixel (4), verticalZoomFactor (1.0f), triggerMode (Up), numLeftToAverage (numSamplesPerPixel),
      bufferSize (2048), bufferWritePos (0),
      minBuffer (bufferSize), maxBuffer (bufferSize),
      currentMax (-1.0f), currentMin (1.0f),
      samplesToProcess (32768),
      tempProcessingBlock (32768),
      needToUpdate (false),
      needToRepaint (true)
{
    const ScopedLock sl (imageLock);
    image = Image (Image::RGB, jmax (1, getWidth()), jmax (1, getHeight()), false);
    Graphics g (image);
    g.fillAll (Colours::black);
    
    if (backgroundThreadToUse == nullptr)
    {
        OptionalScopedPointer<TimeSliceThread> newThread (new TimeSliceThread ("Triggered Scope Rendering Thread"), true);
        backgroundThreadToUse = newThread;
        backgroundThreadToUse->startThread (1);
    }
        
    backgroundThreadToUse->addTimeSliceClient (this);
    
    minBuffer.clear (bufferSize);
    maxBuffer.clear (bufferSize);
     
    startTimer (1000 / 60);
}

TriggeredScope::~TriggeredScope()
{
    const ScopedLock sl (imageLock);

    stopTimer();
    
    backgroundThreadToUse->removeTimeSliceClient (this);

    if (backgroundThreadToUse.willDeleteObject())
        backgroundThreadToUse->stopThread (500);
}

void TriggeredScope::setNumSamplesPerPixel (int newNumSamplesPerPixel)
{
    numSamplesPerPixel = newNumSamplesPerPixel;
}

void TriggeredScope::setVerticalZoomFactor (float newVerticalZoomFactor)
{
    verticalZoomFactor = newVerticalZoomFactor;
}

void TriggeredScope::setTriggerMode (TriggerMode newTriggerMode)
{
    if (newTriggerMode == None
        || newTriggerMode == Up
        || newTriggerMode == Down)
    {
        triggerMode = newTriggerMode;
    }
}

void TriggeredScope::addSamples (const float* samples, int numSamples)
{
    // if we don't have enough space in the fifo, clear out some old samples
    const int numFreeInBuffer = samplesToProcess.getNumFree();
    if (numFreeInBuffer < numSamples)
        samplesToProcess.removeSamples (numFreeInBuffer);

    samplesToProcess.writeSamples (samples, numSamples);
    
    needToUpdate = true;
}

//==============================================================================
void TriggeredScope::resized()
{
    const ScopedLock sl (imageLock);

    image = Image (Image::RGB, jmax (1, getWidth()), jmax (1, getHeight()), false);
    Graphics g (image);
    g.fillAll (Colours::black);
    
    needToRepaint = true;
}

void TriggeredScope::paint (Graphics& g)
{
    const ScopedLock sl (imageLock);

    g.drawImageAt (image, 0, 0);
}

void TriggeredScope::timerCallback()
{
    if (needToRepaint)
        repaint();
}

int TriggeredScope::useTimeSlice()
{
    if (needToUpdate)
    {
        processPendingSamples();
        renderImage();
        
        needToUpdate = false;
    }

    return 10;
}

//==============================================================================
void TriggeredScope::processPendingSamples()
{
    int numSamples = samplesToProcess.getNumAvailable();
    samplesToProcess.readSamples (tempProcessingBlock, numSamples);
    float* samples = tempProcessingBlock.getData();
    
    while (--numSamples >= 0)
    {
        const float currentSample = *samples++;
        
        if (currentSample < currentMin)
            currentMin = currentSample;
        if (currentSample > currentMax)
            currentMax = currentSample;
        
        if (--numLeftToAverage == 0)
        {
            minBuffer[bufferWritePos] = currentMin;
            maxBuffer[bufferWritePos] = currentMax;

            currentMax = -1.0f;
            currentMin = 1.0f;
            
            ++bufferWritePos %= bufferSize;
            numLeftToAverage = numSamplesPerPixel;
        }
    }
}

void TriggeredScope::renderImage()
{
    const ScopedLock sl (imageLock);
    
    Graphics g (image);
    
    g.fillAll (Colours::black);
    g.setColour (Colours::white);
    
    const int w = image.getWidth();
    const int h = image.getHeight();
    
    int bufferReadPos = bufferWritePos - w;
    if (bufferReadPos < 0 )
        bufferReadPos += bufferSize;
    
    if (triggerMode != None)
    {
        int posToTest = bufferReadPos;
        int numToSearch = bufferSize;
        while (--numToSearch >= 0)
        {
            int prevPosToTest = posToTest - 1;
            if (prevPosToTest < 0)
                prevPosToTest += bufferSize;
            
            if (triggerMode == Up)
            {
                if (minBuffer[prevPosToTest] <= 0.0f
                    && maxBuffer[posToTest] > 0.0f)
                {
                    bufferReadPos = posToTest;
                    break;
                }
            }
            else
            {
                if (minBuffer[prevPosToTest] > 0.0f
                    && maxBuffer[posToTest] <= 0.0f)
                {
                    bufferReadPos = posToTest;
                    break;
                }
            }
            
            if (--posToTest < 0)
                posToTest += bufferSize;
        }
    }
    
    int currentX = 0;
    while (currentX < w)
    {
        ++bufferReadPos;
        if (bufferReadPos == bufferSize)
            bufferReadPos = 0;
        
        const float top = (1.0f - (0.5f + (0.5f * verticalZoomFactor * maxBuffer[bufferReadPos]))) * h;
        const float bottom = (1.0f - (0.5f + (0.5f * verticalZoomFactor * minBuffer[bufferReadPos]))) * h;

        g.drawVerticalLine (currentX, top, bottom);
        ++currentX;
    }
    
    needToRepaint = true;
}