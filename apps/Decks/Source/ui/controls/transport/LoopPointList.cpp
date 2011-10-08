/*
  ==============================================================================

    LoopPointList.cpp
    Created: 11 Jul 2011 9:33:36am
    Author:  David Rowland

  ==============================================================================
*/

#include "LoopPointList.h"

LoopPointList::LoopPointList(AudioFilePlayer* _filePlayer)
:   filePlayer(_filePlayer),
    model(&loopPointsList, filePlayer)
{
    loopPointsList.getViewport()->setScrollBarThickness(10);
    loopPointsList.setModel(&model);
    addAndMakeVisible(&loopPointsList);
    
    filePlayer->addListener(this);
}

LoopPointList::~LoopPointList()
{
    filePlayer->removeListener(this);
}

void LoopPointList::resized()
{
    loopPointsList.setBounds(getLocalBounds());
}

void LoopPointList::fileChanged (AudioFilePlayer *player)
{
    if (player == filePlayer)
    {
        loopPointsTree = filePlayer->getLibraryEntry().getChildWithName (Columns::libraryLoopIdentifier);
        if (! loopPointsTree.isValid())
            loopPointsTree = filePlayer->getLibraryEntry();
        
        model.setValueTree(loopPointsTree);
        loopPointsList.updateContent();
    }
}