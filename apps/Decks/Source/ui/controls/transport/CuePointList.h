/*
  ==============================================================================

    CuePointList.h
    Created: 11 Jul 2011 9:33:36am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __CUEPOINTLIST_H_8C79EF1F__
#define __CUEPOINTLIST_H_8C79EF1F__

#include <dRowAudio/dRowAudio.h>
#include "CuePointListBoxModel.h"

class CuePointList :    public Component,
                        public FilteringAudioFilePlayer::Listener
{
public:
    
    CuePointList(FilteringAudioFilePlayer* filePlayer);
    
    ~CuePointList();
    
    void resized();
    
    void fileChanged (FilteringAudioFilePlayer *player);
    
private:
        
    FilteringAudioFilePlayer* filePlayer;
    ValueTree cuePointsTree;
    CuePointListBoxModel model;
    ListBox cuePointsList;
};


#endif  // __CUEPOINTLIST_H_8C79EF1F__
