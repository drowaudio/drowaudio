/*
 *  dRowAudio_Buffer.h
 *  dRowAudio
 *
 *  Created by David Rowland on 27/02/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DROWAUDIO_BUFFER__H_
#define _DROWAUDIO_BUFFER__H_

#include <juce/juce.h>

//==============================================================================
/**
	A buffer to hold an array of floats.
	This is a simple container for an array of floats that can be used to store 
	common values such as a waveshape or look-up table.
	Create one of these on the stack for most efficient and safe use.
 
	You can attach listners that inherit from Buffer::Listener to a Buffer
	to be notified when they change their contents using Buffer::addListener.
	This can be useful in updating UI elements such as wave displays.
 */
class Buffer
{
public:
	/** Creates a buffer with a given size. */
	Buffer(int size);
	
	/** Creates a copy of another buffer. */
	Buffer(const Buffer& otherBuffer);
	
	/**	Destructor. */
	~Buffer();
	
	/** Changes the size of the buffer.
		This will change the size of the buffer, keeping as much of the existing data
		as possible. Any elements beyond the orginal ones will have a value of zero.
		Therefore it is best to either reset the whole buffer or refill it from your own
		algorithm before using it.
	 */
	inline void setSize(int newSize);
	
	/**	Changes the size of the buffer.
		This does the same as setSize() but slightly quicker with the expense of possibly
		having rubbish in the buffer. Be sure to either refill or reset the buffer before using it.
	 */
	inline void setSizeQuick(int newSize)	{	buffer.malloc(newSize);
												bufferSize = newSize;		}
	
	/** Returns a value from the buffer.
		This method performs no bounds checking so if the index is out of the internal array
		bounds will contain garbage.
	 */
	inline float operator[](const int index)	{	return buffer[index];		}
	
	/** Zeros the buffer's contents.
	 */
	inline void reset()			{	zeromem(buffer, bufferSize);	}
		
	/** Returns a pointer to the beggining of the data.
		Don't hang on to this pointer as it may change if the buffer is internally re-allocated.
	 */
	inline float* getData()		{	return (float*)buffer;	}
	
	/** Returns the current size of the buffer. */
	inline int getSize()		{	return bufferSize;	}
	
	/**	Copies the contents of a section of memory into the internal buffer.
	 
		This is simpler than using the [] operator or getData and your own for loop if you are
		copying a buffer you have already created, especially if the sizes may be different.
		If resizeToFit is true the internal array will be resized to the size of the data passed
		to it. If it is false and the internal array is larger than the size passed in any
		extra elements will be left alone. If the internal array is smaller only enough data
		will be copied to fit so you may lose the end of the data passed in.
	 */
	void copyFrom (float* data, int size, bool resizeToFit=true)
	{
		if (resizeToFit)
			setSizeQuick(size);
		
		quickCopy(data, size);
	}
	
	/**	This performs a very quick copy of some data given to it.
		No resizing is done so if the size of the data passed is less than or equal to the size of the internal array
		it will be filled, other wise elements will be left. 
	 */
	void quickCopy (float* data, int size)
	{
		if (size > bufferSize)
			size = bufferSize;
		
		memcpy(buffer, data, size);
	}
	
	/** Updates the buffer's listeners.
		Call this to explicitly tell any registerd listeners that the buffer has changed.
	 */
	void updateListeners();
	
	//==============================================================================
    /** Receives callbacks when a Buffer object changes.
		@see Buffer::addListener
	 */
    class  Listener
    {
    public:
        Listener()          {}
        virtual ~Listener() {}
		
        /** Called when a Buffer object is changed.
		 */
        virtual void bufferChanged (Buffer* buffer) = 0;
    };
	
    /** Adds a listener to receive callbacks when the buffer changes.
	 */
    void addListener (Listener* const listener);
	
    /** Removes a listener that was previously added with addListener(). */
    void removeListener (Listener* const listener);
	
    //==============================================================================
	
private:
	

	HeapBlock <float> buffer;
	int bufferSize;
	
	void callListeners();
	SortedSet <Listener*> listeners;
	
};

#endif //_DROWAUDIO_BUFFER__H_