/*
  ==============================================================================

    dRowAudio_BufferArray.cpp
    Created: 19 Oct 2010 7:51:59pm
    Author:  David Rowland

  ==============================================================================
*/

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_BufferArray.h"

BufferArray::BufferArray(int bufferSize_, int arraySizeLimit)
:	bufferSize(bufferSize_),
	arraySize(arraySizeLimit),
	writePos(0),
	readPos(0)
{
	bufferArray.insertMultiple(0, Buffer(bufferSize), arraySize);
}

BufferArray::~BufferArray()
{
}

void BufferArray::setSizeLimit(int newLimit)
{
	if (newLimit > arraySize) {
		bufferArray.insertMultiple(-1, Buffer(bufferSize), newLimit - arraySize);
		arraySize = newLimit;
	}
}

Buffer& BufferArray::getBuffer(int indexFromCurrent)
{
	if (indexFromCurrent == 0)
	{
		return bufferArray.getReference(writePos);
	}

	indexFromCurrent = jlimit(0, arraySize-1, indexFromCurrent);
	int actualIndex = writePos - indexFromCurrent;
	
	if ((writePos - indexFromCurrent) < 0) {
		actualIndex = arraySize - (indexFromCurrent - writePos);
	}
	return bufferArray.getReference(actualIndex);
}

int BufferArray::getNumBuffersInUse()
{
	if (writePos >= readPos) {
		return writePos - readPos;
	}
	else {
		return writePos + (arraySize - readPos);
	}

}

void BufferArray::incrimentWritePos()
{
//	if (++writePos >= arraySize)
//		writePos -= arraySize;
}

void BufferArray::free(int numToFree)
{
	if (numToFree > 0)
	{
		readPos += numToFree;
		
		if (readPos > writePos) {
			readPos = writePos;
		}
		else if (readPos >= arraySize) {
			readPos -= arraySize;
		}
	}
}

//==============================================================================

void BufferArray::addListener (Listener* const listener)
{
    jassert (listener != 0);
    if (listener != 0)
        listeners.add (listener);
}

void BufferArray::removeListener (Listener* const listener)
{
    listeners.removeValue (listener);
}

void BufferArray::callListeners(Buffer& changedBuffer)
{
    for (int i = listeners.size(); --i >= 0;)
    {
        Listener* const l = listeners[i];
		
        if (l != 0)
            l->bufferChanged (changedBuffer);
    }
}
//==============================================================================

END_DROWAUDIO_NAMESPACE