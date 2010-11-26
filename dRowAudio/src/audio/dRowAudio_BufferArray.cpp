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
	currentIndex(0)
{
	bufferArray.insertMultiple(0, Buffer(bufferSize), arraySize);
}

BufferArray::~BufferArray()
{
}

Buffer& BufferArray::getBuffer(int indexFromCurrent)
{
	if (indexFromCurrent == 0)
	{
		return bufferArray.getReference(currentIndex);
	}
	else if (indexFromCurrent < currentIndex)
	{
		return bufferArray.getReference(currentIndex - indexFromCurrent);
	}
	else if ((indexFromCurrent > currentIndex) && (indexFromCurrent < bufferArray.size()))
	{
		indexFromCurrent = bufferArray.size() - (indexFromCurrent - currentIndex);
		return bufferArray.getReference(indexFromCurrent);
	}
	else {
		return bufferArray.getReference(currentIndex);
	}

}

void BufferArray::incrimentCurrent(int numToIncrimentBy)
{
	if (numToIncrimentBy > 0) {
		currentIndex += numToIncrimentBy;
		if (currentIndex >= arraySize) {
			currentIndex -= arraySize;
		}
	}
}

//void BufferArray::free(int numberToFree)
//{
//}

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