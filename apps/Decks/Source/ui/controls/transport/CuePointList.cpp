/*
  ==============================================================================

    CuePointList.cpp
    Created: 11 Jul 2011 9:33:36am
    Author:  David Rowland

  ==============================================================================
*/

#include "CuePointList.h"

CuePointList::CuePointList(AudioFilePlayer* _filePlayer)
:   filePlayer(_filePlayer),
    model(&cuePointsList, filePlayer)
{
    cuePointsList.getViewport()->setScrollBarThickness(10);
    cuePointsList.setModel(&model);
    addAndMakeVisible(&cuePointsList);
    
    filePlayer->addListener(this);
}

CuePointList::~CuePointList()
{
    filePlayer->removeListener(this);
}

void CuePointList::resized()
{
    cuePointsList.setBounds(getLocalBounds());
}

void CuePointList::fileChanged (AudioFilePlayer *player)
{
    if (player == filePlayer)
    {
        cuePointsTree = filePlayer->getLibraryEntry().getChildWithName (MusicColumns::libraryCuePointIdentifier);
        if (! cuePointsTree.isValid())
            cuePointsTree = filePlayer->getLibraryEntry();
        
        model.setValueTree(cuePointsTree);
        cuePointsList.updateContent();
    }
}