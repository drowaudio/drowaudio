/*
  ==============================================================================

    LoopPointList.h
    Created: 11 Jul 2011 9:33:36am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __LOOPPOINTLIST_H_8C79EF1F__
#define __LOOPPOINTLIST_H_8C79EF1F__

#include "../JuceLibraryCode/JuceHeader.h"

#include "LoopPointListBoxModel.h"

class LoopPointList :   public Component,
                        public AudioFilePlayer::Listener
{
public:
    
    LoopPointList (AudioFilePlayer* filePlayer);
    
    ~LoopPointList();
    
    void resized();
    
    void fileChanged (AudioFilePlayer *player);
    
private:
        
    AudioFilePlayer* filePlayer;
    ValueTree loopPointsTree;
    LoopPointListBoxModel model;
    ListBox loopPointsList;
};


#endif  // __LOOPPOINTLIST_H_8C79EF1F__
