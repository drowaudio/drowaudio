/*
  ==============================================================================

    FifoBuffer.h
    Created: 5 Feb 2011 12:43:01am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_FIFOBUFFER_H__
#define __DROWAUDIO_FIFOBUFFER_H__

//==============================================================================
/** This is a simple implementation of a lock free Fifo buffer that uses
    floating point samples.
 */
class FifoBuffer
{
public:
    //==============================================================================
    /** Creates a FifoBuffer with a given initial size.
     */
	FifoBuffer (int initialSize)
        : abstractFifo (initialSize)
	{
		buffer.malloc (abstractFifo.getTotalSize());
	}
	
    /** Returns the number of samples in the buffer.
     */
	inline int getNumAvailable()
	{
		return abstractFifo.getNumReady();
	}
	
    /** Sets the size of the buffer.
        This does not keep any of the old data.
     */
	inline void setSize (int newSize)
	{
		abstractFifo.setTotalSize (newSize);
		buffer.malloc (abstractFifo.getTotalSize());
	}

    /** Returns the size of the buffer.
     */
	inline int getSize()
	{
		return abstractFifo.getTotalSize();
	}
	
    /** Writes a number of samples into the buffer.
     */
	void writeSamples (const float* samples, int numSamples)
	{
		int start1, size1, start2, size2;
		abstractFifo.prepareToWrite (numSamples, start1, size1, start2, size2);

		if (size1 > 0)
			memcpy(buffer.getData()+start1, samples, size1 * sizeof (float));
		
		if (size2 > 0)
			memcpy(buffer.getData()+start2, samples+size1, size2 * sizeof (float));

		abstractFifo.finishedWrite (size1 + size2);
	}
	
    /** Reads a number of samples from the buffer into the array provided.
     */
	void readSamples (float* bufferToFill, int numSamples)
	{
		int start1, size1, start2, size2;
		abstractFifo.prepareToRead (numSamples, start1, size1, start2, size2);
		
		if (size1 > 0)
			memcpy(bufferToFill, buffer.getData() + start1, size1 * sizeof (float));

		if (size2 > 0)
			memcpy(bufferToFill + size1, buffer.getData() + start2, size2 * sizeof (float));
		
		abstractFifo.finishedRead (size1 + size2);
	}
	
private:
    //==============================================================================
	AbstractFifo abstractFifo;
	HeapBlock<float> buffer;
};

#endif  // __DROWAUDIO_FIFOBUFFER_H__
