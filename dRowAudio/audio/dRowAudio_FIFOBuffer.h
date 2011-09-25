/*
  ==============================================================================

    FIFOBuffer.h
    Created: 5 Feb 2011 12:43:01am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_FIFOBUFFER_H_1464FF36__
#define __DROWAUDIO_FIFOBUFFER_H_1464FF36__

#include "../core/dRowAudio_StandardHeader.h"

class FIFOBuffer
{
public:
	FIFOBuffer(int initialSize)
	:	abstractFifo (initialSize)
	{
		buffer.malloc(abstractFifo.getTotalSize());
	}
	
	inline int getNumAvailable()
	{
		return abstractFifo.getNumReady();
	}
	
	inline void setSize(int newSize)
	{
		abstractFifo.setTotalSize(newSize);
		buffer.malloc(abstractFifo.getTotalSize());
	}

	inline int getSize()
	{
		return abstractFifo.getTotalSize();
	}
	
	void writeSamples (const float* samples, int numSamples)
	{
		int start1, size1, start2, size2;
		abstractFifo.prepareToWrite (numSamples, start1, size1, start2, size2);

		if (size1 > 0)
			memcpy(buffer.getData()+start1, samples, size1 * sizeof(float));
		
		if (size2 > 0)
			memcpy(buffer.getData()+start2, samples+size1, size2 * sizeof(float));

		abstractFifo.finishedWrite (size1 + size2);
//		jassert((size1 + size2) == numSamples);
	}
	
	void readSamples (float* bufferToFill, int numSamples)
	{
		int start1, size1, start2, size2;
		abstractFifo.prepareToRead (numSamples, start1, size1, start2, size2);
		
		if (size1 > 0)
			memcpy(bufferToFill, buffer.getData() + start1, size1 * sizeof(float));

		if (size2 > 0)
			memcpy(bufferToFill + size1, buffer.getData() + start2, size2 * sizeof(float));
		
		abstractFifo.finishedRead (size1 + size2);
//		jassert((size1 + size2) == numSamples);
	}
	
private:
	AbstractFifo abstractFifo;
	HeapBlock<float> buffer;
};

#endif  // __DROWAUDIO_FIFOBUFFER_H_1464FF36__
