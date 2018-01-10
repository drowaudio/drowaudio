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
    verticalZoomFactor (1.0f),
    needToUpdate (false),
    needToRepaint (true)
{
    setNumChannels (1);
    setColour (lineColourId, Colours::white);
    setColour (backgroundColourId, Colours::black);
    
    for (int i = 0; i < 32; i++)
        setColour (traceColourId + i, Colours::white);
    
    const ScopedLock sl (imageLock);
    image = Image (Image::RGB, jmax (1, getWidth()), jmax (1, getHeight()), true);
    Graphics g (image);
    g.fillAll (Colours::transparentBlack);

    if (backgroundThreadToUse == nullptr)
    {
        OptionalScopedPointer<TimeSliceThread> newThread (new TimeSliceThread ("Triggered Scope Rendering Thread"), true);
        backgroundThreadToUse = newThread;
        backgroundThreadToUse->startThread (1);
    }

    backgroundThreadToUse->addTimeSliceClient (this);

    for (auto c : channels)
    {
        c->minBuffer.clear ((size_t) c->bufferSize);
        c->maxBuffer.clear ((size_t) c->bufferSize);
    }

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

void TriggeredScope::setNumChannels (int num)
{
    channels.clear();

    while (channels.size() < num)
        channels.add (new Channel());
    
    for (auto c : channels)
    {
        c->minBuffer.clear ((size_t) c->bufferSize);
        c->maxBuffer.clear ((size_t) c->bufferSize);
    }
}

void TriggeredScope::setNumSamplesPerPixel (const int newNumSamplesPerPixel)
{
    numSamplesPerPixel = newNumSamplesPerPixel;
}

void TriggeredScope::setVerticalZoomFactor (const float newVerticalZoomFactor)
{
    verticalZoomFactor = newVerticalZoomFactor;
}

void TriggeredScope::setVerticalZoomOffset (float newVerticalZoomOffset, int ch)
{
    if (verticalZoomOffset.size() < ch + 1)
        verticalZoomOffset.resize (ch + 1);
    
    verticalZoomOffset.set (ch, newVerticalZoomOffset);
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
    const float* const data[] = { samples };
    
    AudioSampleBuffer buffer ((float* const *)data, 1, numSamples);
    addSamples (buffer);
}

void TriggeredScope::addSamples (const AudioSampleBuffer& buffer)
{
    jassert (buffer.getNumChannels() == channels.size());
    
    for (int i = 0; i < jmin (buffer.getNumChannels(), channels.size()); i++)
    {
        const float* samples = buffer.getReadPointer (i);
        const int numSamples = buffer.getNumSamples();
        
        // if we don't have enough space in the fifo, clear out some old samples
        const int numFreeInBuffer = channels[i]->samplesToProcess.getNumFree();
        if (numFreeInBuffer < numSamples)
            channels[i]->samplesToProcess.removeSamples (numFreeInBuffer);
        
        channels[i]->samplesToProcess.writeSamples (samples, numSamples);
    }
    needToUpdate = true;
}

//==============================================================================
void TriggeredScope::resized()
{
    const ScopedLock sl (imageLock);

    image = Image (Image::ARGB, jmax (1, getWidth()), jmax (1, getHeight()), true);
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
    
    g.setColour (findColour (lineColourId).withMultipliedAlpha (0.5f));
    if (triggerMode != None && drawTriggerPos)
    {
        const int w = image.getWidth();
        const int h = image.getHeight();
        
        int ch = jmax (0, triggerChannel);
        const float y = (1.0f - (0.5f + (0.5f * verticalZoomFactor * (verticalZoomOffset[ch] + triggerLevel)))) * h;
        
        g.drawHorizontalLine (roundToInt (y), 0.0f, float (w));
        g.drawVerticalLine (roundToInt (w * triggerPos), 0.0f, float (h));
    }
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
    for (auto c : channels)
    {
        int numSamples = c->samplesToProcess.getNumAvailable();
        c->samplesToProcess.readSamples (c->tempProcessingBlock, numSamples);
        float* samples = c->tempProcessingBlock.getData();

        while (--numSamples >= 0)
        {
            const float currentSample = *samples++;

            if (currentSample < c->currentMin)
                c->currentMin = currentSample;
            if (currentSample > c->currentMax)
                c->currentMax = currentSample;

            if (--c->numLeftToAverage == 0)
            {
                c->minBuffer[c->bufferWritePos] = c->currentMin;
                c->maxBuffer[c->bufferWritePos] = c->currentMax;

                c->currentMax = -1.0f;
                c->currentMin = 1.0f;

                ++c->bufferWritePos %= c->bufferSize;
                c->numLeftToAverage = numSamplesPerPixel;
            }
        }
    }
}

int TriggeredScope::getTriggerPos()
{
    const int w = image.getWidth();
    
    int bufferReadPos = 0;
    
    auto minBuffer = [&] (int i) -> float
    {
        if (triggerChannel == -1)
        {
            float sum = 0;
            for (auto c : channels)
                sum += c->minBuffer[i];
            
            return sum / channels.size();
        }
        else
        {
            return channels[triggerChannel]->minBuffer[i];
        }
    };
    
    auto maxBuffer = [&] (int i) -> float
    {
        if (triggerChannel == -1)
        {
            float sum = 0;
            for (auto c : channels)
                sum += c->maxBuffer[i];
            
            return sum / channels.size();
        }
        else
        {
            return channels[triggerChannel]->maxBuffer[i];
        }
    };
    
    if (auto* c = triggerChannel >= 0 ? channels[triggerChannel] : channels.getFirst())
    {
        bufferReadPos = c->bufferWritePos - w;
        if (bufferReadPos < 0 )
            bufferReadPos += c->bufferSize;
        
        if (triggerMode != None)
        {
            int posToTest = bufferReadPos;
            int numToSearch = c->bufferSize;
            while (--numToSearch >= 0)
            {
                int prevPosToTest = posToTest - 1;
                if (prevPosToTest < 0)
                    prevPosToTest += c->bufferSize;
                
                if (triggerMode == Up)
                {
                    if (minBuffer (prevPosToTest) <= triggerLevel
                        && maxBuffer (posToTest) > triggerLevel)
                    {
                        bufferReadPos = posToTest;
                        break;
                    }
                }
                else
                {
                    if (minBuffer (prevPosToTest) > triggerLevel
                        && maxBuffer (posToTest) <= triggerLevel)
                    {
                        bufferReadPos = posToTest;
                        break;
                    }
                }
                
                if (--posToTest < 0)
                    posToTest += c->bufferSize;
            }
        }
    }
    return bufferReadPos;
}

void TriggeredScope::renderImage()
{
    const ScopedLock sl (imageLock);

    image.clear ({0, 0, image.getWidth(), image.getHeight()}, Colours::transparentBlack);
    
    Graphics g (image);

    g.fillAll (Colours::transparentBlack);

    const int w = image.getWidth();
    const int h = image.getHeight();

    
    
    int bufferReadPos = getTriggerPos();
    
    bufferReadPos -= roundToInt (w * triggerPos);
    if (bufferReadPos < 0 )
        bufferReadPos += channels[0]->bufferSize;
    
    int ch = 0;
    for (auto c : channels)
    {
        g.setColour (findColour (traceColourId + ch));
        
        int pos = bufferReadPos;
        int currentX = 0;
        Range<float> r0;
        while (currentX < w)
        {
            ++pos;
            if (pos == c->bufferSize)
                pos = 0;

            const float top = (1.0f - (0.5f + (0.5f * verticalZoomFactor * (verticalZoomOffset[ch] + c->maxBuffer[pos])))) * h;
            const float bottom = (1.0f - (0.5f + (0.5f * verticalZoomFactor * (verticalZoomOffset[ch] + c->minBuffer[pos])))) * h;
            
            //jassert (top <= bottom);
            
            Range<float> r1 (top, bottom);

            if (currentX == 0 || r1.intersects(r0))
                g.drawVerticalLine (currentX, top, bottom);
            else if (r0.getEnd() < r1.getStart())
                g.drawLine (currentX - 1.0f, float (r0.getEnd()), float (currentX), float (r1.getEnd()));
            else if (r0.getStart() > r1.getEnd())
                g.drawLine (currentX - 1.0f, r0.getStart(), float (currentX), r1.getStart());
            else
                g.drawLine (currentX - 1.0f, top, float (currentX), bottom);
            
            ++currentX;
            r0 = r1;
        }
        ch++;
    }

    needToRepaint = true;
}
