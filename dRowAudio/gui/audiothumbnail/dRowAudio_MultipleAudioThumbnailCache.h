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

#ifndef __DROWAUDIO_MULTIPLEAUDIOTHUMBNAILCACHE_H__
#define __DROWAUDIO_MULTIPLEAUDIOTHUMBNAILCACHE_H__

#include "dRowAudio_ColouredAudioThumbnail.h"
#include "dRowAudio_FilteredAudioThumbnail.h"

struct MultipleThumbnailCacheEntry;

//==============================================================================
/**
    An instance of this class is used to manage multiple AudioThumbnail objects.

    The cache runs a single background thread that is shared by all the thumbnails
    that need it, and it maintains a set of low-res previews in memory, to avoid
    having to re-scan audio files too often.

    @see AudioThumbnail
*/
class MultipleAudioThumbnailCache   : public TimeSliceThread
{
public:
    //==============================================================================
    /** Creates a cache object.

        The maxNumThumbsToStore parameter lets you specify how many previews should
        be kept in memory at once.
    */
    explicit MultipleAudioThumbnailCache (int maxNumThumbsToStore);

    /** Destructor. */
    ~MultipleAudioThumbnailCache();

    //==============================================================================
    /** Clears out any stored thumbnails.
    */
    void clear();

    /** Reloads the specified thumb if this cache contains the appropriate stored
        data.

        This is called automatically by the AudioThumbnail class, so you shouldn't
        normally need to call it directly.
    */
    bool loadThumb (ColouredAudioThumbnail& thumb, int64 hashCode);

    bool loadThumb (FilteredAudioThumbnail& thumb, int64 hashCode);

    /** Stores the cachable data from the specified thumb in this cache.

        This is called automatically by the AudioThumbnail class, so you shouldn't
        normally need to call it directly.
    */
    void storeThumb (const ColouredAudioThumbnail& thumb, int64 hashCode);

	void storeThumb (const FilteredAudioThumbnail& thumb, int64 hashCode);


private:
    //==============================================================================
    OwnedArray <MultipleThumbnailCacheEntry> thumbs;
    int maxNumThumbsToStore;

    MultipleThumbnailCacheEntry* findThumbFor (int64 hash) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultipleAudioThumbnailCache);
};

#endif   // __DROWAUDIO_MULTIPLEAUDIOTHUMBNAILCACHE_H__
