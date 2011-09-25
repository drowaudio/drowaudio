/*
  ==============================================================================

    dRowAudio_BufferArray.h
    Created: 19 Oct 2010 7:51:59pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_BUFFERARRAY_H_102CCFCD__
#define __DROWAUDIO_BUFFERARRAY_H_102CCFCD__

#include "../core/dRowAudio_StandardHeader.h"

/**	Holds a number of Buffers in a circular fashion.
 
	This can be used to store many Buffers sequentially untill they are ready to
	be used. This acts like a circular buffer holding a pointer to the most recent
	buffer added. Once a Buffer has been used and is no longer needed it can be
	freed using the free() method to save memory.
 */
class BufferArray
{
public:
	/**	Creates an array of Buffer objects.
		@param bufferSize The size of each of the Buffer objects it holds.
		@param arraySizeLimit The maximum number of buffer ojects to hold. 
	 */
	BufferArray(int bufferSize, int arraySizeLimit);
	
	/**	Destructor.
	 */
	~BufferArray();

	/**	Sets a limit on the number of Buffers the array can hold.
		This will only increase the size of teh array at the moment.
		If a smaller size than the current is specified no change will be made.
	 */
	void setSizeLimit(int newLimit);
	
	/** Returns the number of buffers the BufferArray can hold.
	 */
	inline int getNumBuffers()		{	return arraySize;	}
		
	/**	Returns the size of the buffers within the array.
	 */
	inline int getBufferSize()	{	return bufferSize;	}
	
	/**	Returns a buffer with a given offset from the most recent one written.
		This must be > 0.
	 */
	Buffer& getBuffer(int indexFromCurrent =0);
	
	/** Returns a Buffer object with a given offset from the current one.
		This is the same as using getBuffer() but uses the [] operator.
		This can be useful when chaining operators
	 
		Eg. @code
	
		// To return a specific sample from a Buffer
		float sample = bufferArrayObject[0][5];
	 
		@endcode
	 */
	Buffer& operator[](int indexFromCurrent)	{	return getBuffer(indexFromCurrent);	}
	
	/**	Returns a value from one of the buffers at a given position.
		This is the same as calling bufferArrayObject[0][5] to return
		the 5th sample in teh most recent buffer.
	 */
	//float getBufferValue(int indexFromCurrent, int bufferPosition);
	
	/**	Returns the number of buffers that haven't been read.
	 i.e. Calling ba.getBuffer(ba.getNumBuffersInUse());
	 will return the oldest one in the array.
	 */
	int getNumBuffersInUse();

	/**	Incriments the next position to be written to.
		You will need to call this manually as 
	 */
	void incrimentWritePos();
	
	/**	Frees a number of buffers in the array.
		This effectively incriments the read position and as Buffers
		are only taken as references is the only way to do it.
	 */
	void free(int numberToFree =1);
	
	//==============================================================================
    /** Receives callbacks when a Buffer object changes.
	 @see BufferArray::addListener
	 */
    class  Listener
    {
    public:
        Listener()          {}
        virtual ~Listener() {}
		
        /** Called when a Buffer object is changed.
		 */
        virtual void bufferChanged (Buffer& buffer) = 0;
    };
	
    /** Adds a listener to receive callbacks when a buffer changes.
		Inherit from BufferArray::Listener then call addListener() to register
		yourself to recieve changes when a buffer is updated.
		This will constantly call the bufferChanged() method passing the buffer
		that changed so you don't have to worry about the current index in the array.
	 */
    void addListener (Listener* const listener);
	
    /** Removes a listener that was previously added with addListener(). */
    void removeListener (Listener* const listener);
	
    //==============================================================================

private:
	
	AbstractFifo abstractFifo;
	Array<Buffer> bufferArray;
	int bufferSize;
	int arraySize;
	int writePos, readPos;
	
	void callListeners(Buffer& changedBuffer);
	SortedSet <Listener*> listeners;
	
	JUCE_LEAK_DETECTOR (BufferArray);
};


#endif  // __DROWAUDIO_BUFFERARRAY_H_102CCFCD__
