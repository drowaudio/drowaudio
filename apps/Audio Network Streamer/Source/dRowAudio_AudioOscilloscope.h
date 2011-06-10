/*
 *  dRowAudio_AudioOscilloscope.h
 *  dRowAudio
 *
 *  Created by David Rowland on 30/07/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_AUDIOOSCILLISCOPE_H_
#define _DROWAUDIO_AUDIOOSCILLISCOPE_H_

#include "../JuceLibraryCode/JuceHeader.h"

/**
	An oscilliscope class for displaying audio waveforms.
 
	This is a high-res version of the SimpleAudioScope class and as such is slightly more CPU intensive.
	Use this when you need detailed images of a waveform instead of a general idea of what is passing through it.
 */
class AudioOscilliscope  :	public Component,
							public Timer
{
public:
	/// Constructor
    AudioOscilliscope()
	:	verticalZoomFactor (1.0f),
		horizontalZoomFactor (1.0f),
		backgroundColour(Colours::black),
		traceColour(Colours::lightgreen)
    {
        bufferPos = 0;
        bufferSize = 2048;		// Needs to be a power of 2 and larger than the width of your scope!
		bufferSizeMask = bufferSize - 1;
        circularBufferMax.calloc(bufferSize);
        circularBufferMin.calloc(bufferSize);
		clear();
		currentMin = bufferLastMin = 1.0e6f;
		currentMax = bufferLastMax = -currentMin;
        numSamplesIn = 0;
		
        setOpaque (true);
        startTimer (1000 / 50);  // repaint every 1/50 of a second
    }
	
	/// Destructor
    ~AudioOscilliscope()
    {
    }
	
	/// @internal
    void paint (Graphics& g)
    {
		g.fillAll (backgroundColour);
		g.setColour (traceColour);
		
		const int width = getWidth();
		const float halfHeight = getHeight() * 0.5f;
		
		const int bp = bufferPos + bufferSize;
		
		for (int x = width; --x >= 0;)
		{
			const int samplesAgo = width - x;

			float max = circularBufferMax [(bp - samplesAgo) &bufferSizeMask];
			float min = circularBufferMin [(bp - samplesAgo) &bufferSizeMask];

			if (min > bufferLastMax)
				min = bufferLastMax;
			if (max < bufferLastMin)
				max = bufferLastMin;
			
			bufferLastMax = max;
			bufferLastMin = min;
			
			g.drawLine ((float) x, halfHeight + (halfHeight * verticalZoomFactor * max),
						(float) x, halfHeight + (halfHeight * verticalZoomFactor * min));
		}
    }

	/// @internal
    void timerCallback()
    {
        repaint();
    }
	
	/// @internal. Used to add a sample to the internal buffer.
    void addSample (const float sample)
    {
		if (sample > currentMax)
			currentMax = sample;
		if (sample < currentMin)
			currentMin = sample;
		
        const int samplesToAverage = 1 + (int)(127.0 * horizontalZoomFactor);
		
        if (++numSamplesIn > samplesToAverage)
        {
			circularBufferMax [bufferPos &bufferSizeMask] = currentMax;
			circularBufferMin [bufferPos &bufferSizeMask] = currentMin;
			bufferPos++;
			
            numSamplesIn = 0;
			currentMin = 1.0e6f;
			currentMax = -currentMin;
        }
    }
	
	/**	Processes a number of samples displaying them on the scope.
		The resolution will depend on how zoomed in you are @see setHorizontalZoom.
	 */
    void processBlock (const float* inputChannelData,
                       int numSamples)
    {
		if (inputChannelData != 0)
		{
			for (int i = 0; i < numSamples; ++i)
				addSample (inputChannelData [i]);
		}
    }
	
	/**	Clears the internal buffers.
		It is a good idea to call this when an audio device is started or stopped to avoid clicks on the scope.
	 */
	void clear()
    {
        zeromem (circularBufferMax, sizeof (float) * bufferSize);
        zeromem (circularBufferMin, sizeof (float) * bufferSize);
	}
	
	/** Adjusts the vertical zoom of the scope.
		0 = nothing, 1 = normal >1 = zoomed.
	 */
	void setVerticalZoom(float newVerticalZoomFactor)	{	verticalZoomFactor = newVerticalZoomFactor;	}

	/**	Adjusts the horizontal zoom of the scope.
		The lower this value is the more zoomed in and detailed the image will be. However, as more information
		is passing the scope will move quicker and be difficult to see. Consider using Component::createComponentSnapshot()
		to capture an image of the scope.
	 */
	void setHorizontalZoom(float newHorizontalZoomFactor)	{	horizontalZoomFactor = newHorizontalZoomFactor;	}

	/// Sets the background colour of the scope
	void setBackgroundColour (Colour newBackgroundColour)	{	backgroundColour = newBackgroundColour;	}
	
	/// Sets the trace colour of the scope.
	void setTraceColour (Colour newTraceColour)	{	traceColour = newTraceColour;	}

private:
    HeapBlock<float> circularBufferMax, circularBufferMin;
	int bufferSizeMask;
    float currentMax, currentMin;
    int volatile bufferPos, bufferSize, numSamplesIn;
	float bufferLastMax, bufferLastMin;
	
	float verticalZoomFactor, horizontalZoomFactor;
	Colour backgroundColour, traceColour;
	
	JUCE_LEAK_DETECTOR (AudioOscilliscope);
};

#endif //_DROWAUDIO_AUDIOOSCILLISCOPE_H_