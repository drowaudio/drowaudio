/*
  ==============================================================================

    MemoryInputSource.cpp
    Created: 20 Nov 2011 4:11:08am
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

//==============================================================================
MemoryInputSource::MemoryInputSource (MemoryInputStream* stream, bool useFileTimeInHashGeneration_)
    : memoryInputStream (stream),
      useFileTimeInHashGeneration (useFileTimeInHashGeneration_)
{
}

MemoryInputSource::~MemoryInputSource()
{
}

InputStream* MemoryInputSource::createInputStream()
{
    return memoryInputStream;
}

InputStream* MemoryInputSource::createInputStreamFor (const String& relatedItemPath)
{
    return nullptr;
}

int64 MemoryInputSource::hashCode() const
{
    int64 h = Time::getCurrentTime().toMilliseconds();
    
    return h;
}

END_JUCE_NAMESPACE