/*
  ==============================================================================

    LoopPointListBoxModel.h
    Created: 11 Jul 2011 9:47:51am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __LOOPPOINTLISTBOXMODEL_H_5CE7BADD__
#define __LOOPPOINTLISTBOXMODEL_H_5CE7BADD__

#include "../JuceLibraryCode/JuceHeader.h"


class LoopPointListBoxModel :    public ListBoxModel
{
public:
    
    LoopPointListBoxModel (ListBox* owner, AudioFilePlayer* filePlayer);
    
    ~LoopPointListBoxModel();
        
    void setValueTree (ValueTree newTree);
    
    int getNumRows();
    
    void paintListBoxItem (int rowNumber,
                           Graphics& g,
                           int width, int height,
                           bool rowIsSelected);
    
    Component* refreshComponentForRow (int rowNumber, bool isRowSelected,
                                       Component* existingComponentToUpdate);
    
    void reorderLoopPoints();
    
private:
        
    friend class NewLoopPointItem;
    friend class LoopPointItem;
    
    ValueTree dataList;
    ListBox* owner;
    AudioFilePlayer* filePlayer;
};

#endif  // __LOOPPOINTLISTBOXMODEL_H_5CE7BADD__
