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

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_BufferArray.h"

BufferArray::BufferArray(int bufferSize_, int arraySizeLimit)
:	abstractFifo(arraySizeLimit),
	bufferSize(bufferSize_),
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
	if (++writePos >= arraySize)
		writePos -= arraySize;
}

void BufferArray::free(int numToFree)
{
	if (numToFree > 0)
	{
		readPos += numToFree;
		
/*		if (readPos > writePos) {
			readPos = writePos;
		}
		else */if (readPos >= arraySize) {
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