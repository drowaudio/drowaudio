/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-10 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_MultipleAudioThumbnailCache.h"


//==============================================================================
struct MultipleThumbnailCacheEntry
{
    int64 hash;
    uint32 lastUsed;
    MemoryBlock data;

    JUCE_LEAK_DETECTOR (MultipleThumbnailCacheEntry);
};

//==============================================================================
MultipleAudioThumbnailCache::MultipleAudioThumbnailCache (const int maxNumThumbsToStore_)
    : TimeSliceThread ("thumb cache"),
      maxNumThumbsToStore (maxNumThumbsToStore_)
{
    startThread (2);
}

MultipleAudioThumbnailCache::~MultipleAudioThumbnailCache()
{
}

MultipleThumbnailCacheEntry* MultipleAudioThumbnailCache::findThumbFor (const int64 hash) const
{
    for (int i = thumbs.size(); --i >= 0;)
        if (thumbs.getUnchecked(i)->hash == hash)
            return thumbs.getUnchecked(i);

    return 0;
}

bool MultipleAudioThumbnailCache::loadThumb (FilteredAudioThumbnail& thumb, const int64 hashCode)
{
    MultipleThumbnailCacheEntry* te = findThumbFor (hashCode);
	
    if (te != 0)
    {
        te->lastUsed = Time::getMillisecondCounter();
		
        MemoryInputStream in (te->data, false);
        thumb.loadFrom (in);
        return true;
    }
	
    return false;
}

bool MultipleAudioThumbnailCache::loadThumb (ColouredAudioThumbnail& thumb, const int64 hashCode)
{
    MultipleThumbnailCacheEntry* te = findThumbFor (hashCode);

    if (te != 0)
    {
        te->lastUsed = Time::getMillisecondCounter();

        MemoryInputStream in (te->data, false);
        thumb.loadFrom (in);
        return true;
    }

    return false;
}

void MultipleAudioThumbnailCache::storeThumb (const ColouredAudioThumbnail& thumb,
											  const int64 hashCode)
{
    MultipleThumbnailCacheEntry* te = findThumbFor (hashCode);
	
    if (te == 0)
    {
        te = new MultipleThumbnailCacheEntry();
        te->hash = hashCode;
		
        if (thumbs.size() < maxNumThumbsToStore)
        {
            thumbs.add (te);
        }
        else
        {
            int oldest = 0;
            uint32 oldestTime = Time::getMillisecondCounter() + 1;
			
            for (int i = thumbs.size(); --i >= 0;)
            {
                if (thumbs.getUnchecked(i)->lastUsed < oldestTime)
                {
                    oldest = i;
                    oldestTime = thumbs.getUnchecked(i)->lastUsed;
                }
            }
			
            thumbs.set (oldest, te);
        }
    }
	
    te->lastUsed = Time::getMillisecondCounter();
	
    MemoryOutputStream out (te->data, false);
    thumb.saveTo (out);
}

void MultipleAudioThumbnailCache::storeThumb (const FilteredAudioThumbnail& thumb, int64 hashCode)
{
    MultipleThumbnailCacheEntry* te = findThumbFor (hashCode);
	
    if (te == 0)
    {
        te = new MultipleThumbnailCacheEntry();
        te->hash = hashCode;
		
        if (thumbs.size() < maxNumThumbsToStore)
        {
            thumbs.add (te);
        }
        else
        {
            int oldest = 0;
            uint32 oldestTime = Time::getMillisecondCounter() + 1;
			
            for (int i = thumbs.size(); --i >= 0;)
            {
                if (thumbs.getUnchecked(i)->lastUsed < oldestTime)
                {
                    oldest = i;
                    oldestTime = thumbs.getUnchecked(i)->lastUsed;
                }
            }
			
            thumbs.set (oldest, te);
        }
    }
	
    te->lastUsed = Time::getMillisecondCounter();
	
    MemoryOutputStream out (te->data, false);
    thumb.saveTo (out);
}

void MultipleAudioThumbnailCache::clear()
{
    thumbs.clear();
}

END_DROWAUDIO_NAMESPACE