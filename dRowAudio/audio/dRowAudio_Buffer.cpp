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

