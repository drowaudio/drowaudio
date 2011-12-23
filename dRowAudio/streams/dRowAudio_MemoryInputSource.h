/*
  ==============================================================================

    MemoryInputSource.h
    Created: 20 Nov 2011 4:11:08am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_MEMORYINPUTSOURCE_H__
#define __DROWAUDIO_MEMORYINPUTSOURCE_H__

//==============================================================================
/**
 A type of InputSource that represents a memory input stream.
 
 @see InputSource
 */
class MemoryInputSource :   public InputSource
{
public:
    //==============================================================================
    MemoryInputSource (MemoryInputStream* stream, bool useFileTimeInHashGeneration = false);
    ~MemoryInputSource();
    
    InputStream* createInputStream();
    InputStream* createInputStreamFor (const String& relatedItemPath);
    int64 hashCode() const;
    
private:
    //==============================================================================
    MemoryInputStream* memoryInputStream;
    bool useFileTimeInHashGeneration;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MemoryInputSource);
};

#endif  // __DROWAUDIO_MEMORYINPUTSOURCE_H__
