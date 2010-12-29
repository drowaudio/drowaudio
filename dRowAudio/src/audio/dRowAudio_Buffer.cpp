/*
 *  dRowAudio_Buffer.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 27/02/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_Buffer.h"

Buffer::Buffer(int size)
:	bufferSize(size)
{
	buffer.allocate(bufferSize, true);
}

Buffer::Buffer(const Buffer& otherBuffer)
:	bufferSize(otherBuffer.bufferSize)
{
	buffer.allocate(bufferSize, false);
	memcpy(buffer, otherBuffer.buffer, bufferSize*sizeof(float));
}

Buffer::~Buffer()
{
}

void Buffer::setSize(int newSize)
{
	buffer.realloc(newSize);
	
	if (newSize > bufferSize)
		zeromem(buffer + bufferSize, (newSize - bufferSize)*sizeof(float));

	bufferSize = newSize;
}

void Buffer::applyBuffer(float *samples, int numSamples)
{
	const int numToApply = jmin(bufferSize, numSamples);
	for (int i = 0; i < numToApply; i++) {
		samples[i] *= buffer[i];
	}

	if (bufferSize < numSamples) {
		zeromem((samples+numToApply), (numSamples - numToApply) * sizeof(float));
	}
}

void Buffer::updateListeners()
{
	callListeners();
}

void Buffer::addListener (Listener* const listener)
{
    jassert (listener != 0);
    if (listener != 0)
        listeners.add (listener);
}

void Buffer::removeListener (Listener* const listener)
{
    listeners.removeValue (listener);
}

void Buffer::callListeners()
{
    for (int i = listeners.size(); --i >= 0;)
    {
        Listener* const l = listeners[i];
		
        if (l != 0)
            l->bufferChanged (this);
    }
}

END_DROWAUDIO_NAMESPACE