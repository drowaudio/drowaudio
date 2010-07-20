/*
 *  dRowAudio_Buffer.cpp
 *  dRowAudio
 *
 *  Created by David Rowland on 27/02/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#include "../core/dRowAudio_StandardHeader.h"

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
	memcpy(buffer, otherBuffer.buffer, bufferSize);
}

Buffer::~Buffer()
{
	buffer.free();
}

void Buffer::setSize(int newSize)
{
	buffer.realloc(newSize);
	
	if (newSize > bufferSize)
		zeromem(buffer + bufferSize, newSize - bufferSize);

	bufferSize = newSize;
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
