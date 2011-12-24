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
	
    /** Returns the number of items free in the buffer. */
    inline int getNumFree()
    {
        return abstractFifo.getFreeSpace();
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
	
    inline void reset()
    {
        abstractFifo.reset();
    }
    
    /** Writes a number of samples into the buffer.
     */
	void writeSamples (const float* samples, int numSamples)
	{
		int start1, size1, start2, size2;
		abstractFifo.prepareToWrite (numSamples, start1, size1, start2, size2);

		if (size1 > 0)
			memcpy (buffer.getData()+start1, samples, size1 * sizeof (float));
		
		if (size2 > 0)
			memcpy (buffer.getData()+start2, samples+size1, size2 * sizeof (float));

		abstractFifo.finishedWrite (size1 + size2);
	}
	
    /** Reads a number of samples from the buffer into the array provided.
     */
	void readSamples (float* bufferToFill, int numSamples)
	{
		int start1, size1, start2, size2;
		abstractFifo.prepareToRead (numSamples, start1, size1, start2, size2);
		
		if (size1 > 0)
			memcpy (bufferToFill, buffer.getData() + start1, size1 * sizeof (float));

		if (size2 > 0)
			memcpy (bufferToFill + size1, buffer.getData() + start2, size2 * sizeof (float));
		
		abstractFifo.finishedRead (size1 + size2);
	}
	
private:
    //==============================================================================
	AbstractFifo abstractFifo;
	HeapBlock<float> buffer;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FifoBuffer);
};

#endif  // __DROWAUDIO_FIFOBUFFER_H__
