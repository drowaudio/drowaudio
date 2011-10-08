/*
  ==============================================================================

    CuePointListBoxModel.h
    Created: 11 Jul 2011 9:47:51am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __CUEPOINTLISTBOXMODEL_H_5CE7BADD__
#define __CUEPOINTLISTBOXMODEL_H_5CE7BADD__

#include "../JuceLibraryCode/JuceHeader.h"


class CuePointListBoxModel :    public ListBoxModel
{
public:
    
    CuePointListBoxModel(ListBox* owner, AudioFilePlayer* filePlayer);
    
    ~CuePointListBoxModel();
        
    void setValueTree(ValueTree newTree);
    
    int getNumRows();
    
    void paintListBoxItem (int rowNumber,
                           Graphics& g,
                           int width, int height,
                           bool rowIsSelected);
    
    Component* refreshComponentForRow (int rowNumber, bool isRowSelected,
                                       Component* existingComponentToUpdate);
    
    void reorderCuePoints();
    
private:
        
    friend class NewCuePointItem;
    friend class CuePointItem;
    
    ValueTree dataList;
    ListBox* owner;
    AudioFilePlayer* filePlayer;
};

#endif  // __CUEPOINTLISTBOXMODEL_H_5CE7BADD__
