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

InputStream* MemoryInputSource::createInputStreamFor (const String& /*relatedItemPath*/)
{
    return nullptr;
}

int64 MemoryInputSource::hashCode() const
{
    int64 h = Time::getCurrentTime().toMilliseconds();
    
    return h;
}

