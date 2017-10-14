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

TriggeredScope::TriggeredScope (TimeSliceThread* tst) :
    backgroundThreadToUse (tst, tst == nullptr),
    triggerMode (Up),
    numSamplesPerPixel (4),
    numLeftToAverage (numSamplesPerPixel),
    verticalZoomFactor (1.0f),
    bufferSize (2048),
    bufferWritePos (0),
    minBuffer ((size_t) bufferSize),
    maxBuffer ((size_t) bufferSize),
    currentMax (-1.0f),
    currentMin (1.0f),
    samplesToProcess (32768),
    tempProcessingBlock (32768),
    needToUpdate (false),
    needToRepaint (true)
{
    setColour (lineColourId, Colours::white);
    setColour (backgroundColourId, Colours::black);
    
    const ScopedLock sl (imageLock);
    image = Image (Image::RGB, jmax (1, getWidth()), jmax (1, getHeight()), false);
    Graphics g (image);
    g.fillAll (Colours::transparentBlack);

    if (backgroundThreadToUse == nullptr)
    {
        OptionalScopedPointer<TimeSliceThread> newThread (new TimeSliceThread ("Triggered Scope Rendering Thread"), true);
        backgroundThreadToUse = newThread;
        backgroundThreadToUse->startThread (1);
    }

    backgroundThreadToUse->addTimeSliceClient (this);

    minBuffer.clear ((size_t) bufferSize);
    maxBuffer.clear ((size_t) bufferSize);

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

void TriggeredScope::setNumSamplesPerPixel (const int newNumSamplesPerPixel)
{
    numSamplesPerPixel = newNumSamplesPerPixel;
}

void TriggeredScope::setVerticalZoomFactor (const float newVerticalZoomFactor)
{
    verticalZoomFactor = newVerticalZoomFactor;
}

void TriggeredScope::setTriggerMode (const TriggerMode newTriggerMode)
{
    if (newTriggerMode == None
        || newTriggerMode == Up
        || newTriggerMode == Down)
    {
        triggerMode = newTriggerMode;
    }
}

void TriggeredScope::addSamples (const float* samples, const int numSamples)
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
    g.fillAll (findColour (backgroundColourId));

    needToRepaint = true;
}

void TriggeredScope::paint (Graphics& g)
{
    const ScopedLock sl (imageLock);

    g.drawImageAt (image, 0, 0);
    
    g.setColour (findColour (lineColourId));
    g.drawRect (getLocalBounds());
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

    image.clear ({0, 0, image.getWidth(), image.getHeight()}, Colours::transparentBlack);
    
    Graphics g (image);

    g.fillAll (Colours::transparentBlack);

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

    g.setColour (findColour (lineColourId));
    
    int currentX = 0;
    Range<float> r0;
    while (currentX < w)
    {
        ++bufferReadPos;
        if (bufferReadPos == bufferSize)
            bufferReadPos = 0;

        const float top = (1.0f - (0.5f + (0.5f * verticalZoomFactor * maxBuffer[bufferReadPos]))) * h;
        const float bottom = (1.0f - (0.5f + (0.5f * verticalZoomFactor * minBuffer[bufferReadPos]))) * h;
        
        jassert (top <= bottom);
        
        Range<float> r1 (top, bottom);

        if (currentX == 0 || r1.intersects(r0))
            g.drawVerticalLine (currentX, top, bottom);
        else if (r0.getEnd() < r1.getStart())
            g.drawLine (currentX - 1.0f, float (r0.getEnd()), float (currentX), float (r1.getEnd()));
        else if (r0.getStart() > r1.getEnd())
            g.drawLine (currentX - 1, r0.getStart(), currentX, r1.getStart());
        else
            g.drawLine (currentX - 1, top, currentX, bottom);
        
        ++currentX;
        r0 = r1;
    }

    needToRepaint = true;
}
