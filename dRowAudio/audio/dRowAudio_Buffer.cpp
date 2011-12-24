/*
 *  dRowAudio_Buffer.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 27/02/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

BEGIN_JUCE_NAMESPACE

Buffer::Buffer (int size)
    : bufferSize (size)
{
	buffer.allocate (bufferSize, true);
}

Buffer::Buffer (const Buffer& otherBuffer)
:	bufferSize (otherBuffer.bufferSize)
{
	buffer.allocate (bufferSize, false);
	memcpy (buffer, otherBuffer.buffer, bufferSize * sizeof (float));
}

Buffer::~Buffer()
{
}

void Buffer::setSize (int newSize)
{
	buffer.realloc (newSize);
	
	if (newSize > bufferSize)
		zeromem (buffer + bufferSize, (newSize - bufferSize) * sizeof (float));

	bufferSize = newSize;
}

void Buffer::applyBuffer (float* samples, int numSamples)
{
	const int numToApply = jmin (bufferSize, numSamples);
	for (int i = 0; i < numToApply; i++)
		samples[i] *= buffer[i];

	if (bufferSize < numSamples)
		zeromem (samples + numToApply, (numSamples - numToApply) * sizeof (float));
}

void Buffer::updateListeners()
{
    listeners.call (&Listener::bufferChanged, this);
}

//==============================================================================
void Buffer::addListener (Buffer::Listener* const listener)
{
    listeners.add (listener);
}

void Buffer::removeListener (Buffer::Listener* const listener)
{
    listeners.remove (listener);
}

END_JUCE_NAMESPACE