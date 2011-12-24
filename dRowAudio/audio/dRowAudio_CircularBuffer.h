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

#ifndef __DROWAUDIO_CIRCULARBUFFER_H_D649299F__
#define __DROWAUDIO_CIRCULARBUFFER_H_D649299F__

#include "../core/dRowAudio_StandardHeader.h"
//==============================================================================
/**	A circular buffer used to store a number of samples.
 
	The idea is that you can create one of these on the stack then
	continuously write samples to it, reading them off when you need to.
	The CircularBuffer will not resize itself automatically so it is a good idea
	to make sure that you give it an appropriate size and read samples off frequently
	to avoid overwriting old ones. 
 */
class CircularBuffer
{
public:
	
	/**	Creates a circular buffer with a given size.
		This size must be bigger than 0 and the buffer may contain garbage
		before you write to it so it may be a good idea to call clear after
		construction.
	 */
	explicit CircularBuffer(int initialBufferSize);
	
	/**	Destructor.
	 */
	~CircularBuffer();
	
	/**	Writes a number of samples to the buffer incrimenting the write position.
		If the buffer is not big enough to hold the number of samples being
		written it will only take the last section so the buffer will hold the most
		recent data.
	 */
	void writeSamples(float* samples, int numSamples);
	
	/**	Reads a number of samples from the buffer incrimenting the read position.
	 */
	void readSamples(float* bufferToFill, int numSamples);

	/**	Reads a number of samples from the buffer without freeing them.
		This can be useful if you need to re-use samples from the buffer
		eg. if overlapping an FFT. Call free() afterwards to release a number of samples.
	 */
	void readWithoutFreeing(float* bufferToFill, int numSamples);
	
	/**	Frees a number of samples, incrimenting the read position.
		This used to free up a number of samples in the buffer and is most
		often used in conjunction with readWithoutFreeing()
	 */
	void free(int numToFree);
	
	/**	Writes one sample to the buffer incrimenting the write position.
	 */
	void push(float newSample);
	
	/**	Reads one sample from the buffer incrimenting the read position.
	 */
	float pop();

	/**	Resizes the buffer.
		This is not guaranteed to keep any of the existing data so a call to
		clear would be a good idea before using.
	 */
	void setSize(int newSize);
	
	/**	Returns the current size of the buffer.
	 */
	inline int getSize()		{	return bufferSize;	}
	
	/**	Fills the buffer with 0.0's.
	 */
	inline void clear()
	{
		zeromem(buffer.getData(), bufferSize*sizeof(float));
	}	
	
	/**	Returns the number of samples available to read.
		This returns the number of samples that have been written but not read
		to give an indication as to weather you can use the buffer yet eg. in
		an FFT where you need 1024 samples but are only passing samples in in
		256 blocks.
	 */
	inline int getNumAvailable()
	{
		if (readPos > writePos) {
			return (bufferSize - readPos) + writePos;
		}
		
		return writePos - readPos;
	}	
	
private:
	
	HeapBlock<float> buffer;
	int bufferSize, readPos, writePos;
	
	JUCE_LEAK_DETECTOR (CircularBuffer);
};


#endif  // __DROWAUDIO_CIRCULARBUFFER_H_D649299F__
