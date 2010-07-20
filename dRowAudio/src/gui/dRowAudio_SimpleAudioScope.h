/*
 *  dRowAudio_SimpleAudioScope.h
 *  dRowAudio
 *
 *  Created by David Rowland on 30/07/2009.
 *  Copyright 2009 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_SIMPLEAUDIOSCOPE_H_
#define _DROWAUDIO_SIMPLEAUDIOSCOPE_H_

#include "../core/dRowAudio_StandardHeader.h"

/**
	A simple oscilliscope class for displaying audio waveforms.
 
	This is a simple scope that will display a low res image of whatever buffer you pass to it.
	If you need more detailed images use AudioOscilliscope.
 */
class SimpleAudioScope  :	public Component,
							public Timer
{
public:
	/// Constructor
    SimpleAudioScope()
	:	verticalZoomFactor (1.0f),
		horizontalZoomFactor (1.0f),
		backgroundColour(Colours::black),
		traceColour(Colours::lightgreen)
    {
        bufferPos = 0;
        bufferSize = 2048;		// Needs to be larger than the width of your scope in pixels!
        circularBuffer = (float*) juce_calloc (sizeof (float) * bufferSize);
		clear();
        currentInputLevel = 0.0f;
        numSamplesIn = 0;
		
        setOpaque (true);
        startTimer (1000 / 50);  // repaint every 1/50 of a second
    }
	
	/// Descructor
    ~SimpleAudioScope()
    {
        juce_free (circularBuffer);
    }
	
	/// @internal
    void paint (Graphics& g)
    {
        g.fillAll (backgroundColour);
        g.setColour (traceColour);
		
        const float halfHeight = getHeight() * 0.5f;
		
        int bp = bufferPos;
		
        for (int x = getWidth(); --x >= 0;)
        {
            const int samplesAgo = getWidth() - x;
            const float level = verticalZoomFactor * circularBuffer [(bp + bufferSize - samplesAgo) % bufferSize];

            if (level > 0.01f)
                g.drawLine ((float) x, halfHeight - halfHeight * level,
                            (float) x, halfHeight + halfHeight * level);
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
        currentInputLevel += fabsf (sample);
		
        const int samplesToAverage = 1 + (int)(127.0 * horizontalZoomFactor);
		
        if (++numSamplesIn > samplesToAverage)
        {
            circularBuffer [bufferPos++ % bufferSize] = currentInputLevel / samplesToAverage;
			
            numSamplesIn = 0;
            currentInputLevel = 0.0f;
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
        zeromem (circularBuffer, sizeof (float) * bufferSize);
    }
		
	/** Adjusts the vertical zoom of the scope.
		0 = nothing, 1 = normal >1 = zoomed.
	 */
	void setVerticalZoom(float newVerticalZoomFactor)	{	verticalZoomFactor = newVerticalZoomFactor;	}
	
	/**	Adjusts the horizontal zoom of the scope.
		The lower this value is the more zoomed in and detailed the image will be. However, as more information
		is passing the scope will move quicker and be difficult to see. Consider using juce::Component::createComponentSnapshot()
		to capture an image of the scope.
	 */
	void setHorizontalZoom(float newHorizontalZoomFactor)	{	horizontalZoomFactor = newHorizontalZoomFactor;	}
	
	/// Sets the background colour of the scope
	void setBackgroundColour (Colour newBackgroundColour)	{	backgroundColour = newBackgroundColour;	}
	
	/// Sets the trace colour of the scope.
	void setTraceColour (Colour newTraceColour)	{	traceColour = newTraceColour;	}
	
	
private:
    float* circularBuffer;
    float currentInputLevel;
    int volatile bufferPos, bufferSize, numSamplesIn;
	
	float verticalZoomFactor, horizontalZoomFactor;
	Colour backgroundColour, traceColour;
};

#endif //_DROWAUDIO_SIMPLEAUDIOSCOPE_H_