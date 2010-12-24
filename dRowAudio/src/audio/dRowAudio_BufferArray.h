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
	BufferArray(int bufferSize, int arraySizeLimit);
	
	~BufferArray();

	/**	Sets a limit on the number of Buffers the array can hold.
	 */
	void setSizeLimit(int newLimit);
	
	/** Returns the number of buffers the BufferArray can hold.
	 */
	int getNumBuffers()		{	return arraySize;	}
	
	Buffer& getLatest()		{	return bufferArray.getReference(currentIndex);	}
	
	Buffer& getBuffer(int indexFromCurrent);
	
	/**	Returns the size of the buffers within the array.
	 */
	int getBufferSize()	{	return bufferSize;	}
	/** Returns a Buffer object with a given offset from the current one.
		This is the same as using getBuffer() but uses the [] operator.
		This can be useful when chaining operators
	 
		Eg. @code
	
		// To return a specific sample from a Buffer
		float sample = bufferArrayObject[0][5];
	 
		@endcode
	 */
	Buffer& operator[](int indexFromCurrent)	{	return getBuffer(indexFromCurrent);	}
	
	float getBufferValue(int indexFromCurrent, int bufferPosition);
	
	void incrimentCurrent(int numToIncrimentBy=1);
	
	/**	Returns the current index in the array. 
	 */
	int getCurrentIndex()	{	return currentIndex;	}
	
//	void free(int numberToFree);
	
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
	
	Array<Buffer> bufferArray;
	int bufferSize;
	int arraySize;
	int currentIndex;
	
	void callListeners(Buffer& changedBuffer);
	SortedSet <Listener*> listeners;
	
	JUCE_LEAK_DETECTOR (BufferArray);
};


#endif  // __DROWAUDIO_BUFFERARRAY_H_102CCFCD__
