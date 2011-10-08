/*
  ==============================================================================

    CuePointList.h
    Created: 11 Jul 2011 9:33:36am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __CUEPOINTLIST_H_8C79EF1F__
#define __CUEPOINTLIST_H_8C79EF1F__

#include "../JuceLibraryCode/JuceHeader.h"

#include "CuePointListBoxModel.h"

class CuePointList :    public Component,
                        public AudioFilePlayer::Listener
{
public:
    
    CuePointList(AudioFilePlayer* filePlayer);
    
    ~CuePointList();
    
    void resized();
    
    void fileChanged (AudioFilePlayer *player);
    
private:
        
    AudioFilePlayer* filePlayer;
    ValueTree cuePointsTree;
    CuePointListBoxModel model;
    ListBox cuePointsList;
};


#endif  // __CUEPOINTLIST_H_8C79EF1F__
