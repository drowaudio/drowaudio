/*
  ==============================================================================

    dRowAudio_CircularBuffer.cpp
    Created: 20 Oct 2010 8:27:26pm
    Author:  David Rowland

  ==============================================================================
*/

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_CircularBuffer.h"

CircularBuffer::CircularBuffer(int initialBufferSize)
:	bufferSize(initialBufferSize),
	readPos(0),
	writePos(1)
{
	buffer.malloc(bufferSize);
}

CircularBuffer::~CircularBuffer()
{
}

void CircularBuffer::writeSamples(float* samples, int numSamples)
{
	// only use last n samples if too many passed in
	if (numSamples > bufferSize)
	{
		samples = samples + (numSamples - bufferSize);
		numSamples = bufferSize;
	}
	
	// can fit all data in at end
	if ((bufferSize - writePos) >= numSamples)
	{
		memcpy(buffer + writePos, samples, numSamples*sizeof(float));
		writePos += numSamples;
	}
	else // need to split up
	{
		const int firstNum = bufferSize - writePos;
		const int secondNum = numSamples - firstNum;

		memcpy(buffer + writePos, samples, firstNum*sizeof(float));
		samples += firstNum;
		memcpy(buffer, samples, secondNum*sizeof(float));
		
		writePos = secondNum;
	}

	const int readWriteGap = getNumAvailable();
	
	//if going to overlap readPos
	if (numSamples > readWriteGap) {
		readPos = writePos + 1;
		if (readPos >= bufferSize) {
			readPos -= bufferSize;
		}
	}
}

void CircularBuffer::readSamples(float* bufferToFill, int numSamples)
{
	jassert(bufferSize > numSamples);
	
	int numToEnd = bufferSize - readPos;
	
	// read can be done in one operation
	if (numSamples < numToEnd) {
		memcpy(bufferToFill, buffer + readPos, numSamples*sizeof(float));
	}
	// need to split into two read operations
	else {
		memcpy(bufferToFill, buffer + readPos, numToEnd*sizeof(float));
		
		int numSecondSamples = numSamples - numToEnd;
		memcpy(bufferToFill + numToEnd, buffer, numSecondSamples*sizeof(float));
	}

	// incriment readPos
	readPos += numSamples;
	if (readPos >= bufferSize) {
		readPos -= bufferSize;
	}	
}

void CircularBuffer::readWithoutFreeing(float* bufferToFill, int numSamples)
{
	jassert(bufferSize >= numSamples);
	
	int numToEnd = bufferSize - readPos;
	
	// read can be done in one operation
	if (numSamples < numToEnd) {
		memcpy(bufferToFill, buffer + readPos, numSamples*sizeof(float));
	}
	// need to split into two read operations
	else {
		memcpy(bufferToFill, buffer + readPos, numToEnd*sizeof(float));
		
		int numSecondSamples = numSamples - numToEnd;
		memcpy(bufferToFill + numToEnd, buffer, numSecondSamples*sizeof(float));
	}
}

void CircularBuffer::free(int numToFree)
{
	readPos += numToFree;
	if (readPos >= bufferSize)
		readPos -= bufferSize;
}

void CircularBuffer::push(float newSample)
{
	buffer[writePos++] = newSample;
	
	if (writePos == bufferSize)
		writePos = 0;
}

float CircularBuffer::pop()
{
	float sample = buffer[readPos];
	
	if (++readPos == bufferSize)
		readPos = 0;

	return sample;
}

void CircularBuffer::setSize(int newSize)
{
	buffer.malloc(newSize);
	readPos = writePos = 0;
	bufferSize = newSize;
}

END_DROWAUDIO_NAMESPACE
