/*
  ==============================================================================

    CuePointListBoxModel.cpp
    Created: 11 Jul 2011 9:47:51am
    Author:  David Rowland

  ==============================================================================
*/

#include "CuePointListBoxModel.h"

//==============================================================================
class NewCuePointItem : public Component,
                        public Button::Listener
{
public:
    NewCuePointItem(CuePointListBoxModel* _ownerListBoxModel, ValueTree& treeToAddTo)
    :   ownerListBoxModel(_ownerListBoxModel),
        tree(treeToAddTo)
    {
        button.setButtonText("+");
        button.addListener(this);
        addAndMakeVisible(&button);
    }
    
    ~NewCuePointItem()
    {
        button.removeListener(this);
    }
    
    void resized()
    {
        Rectangle<int> buttonRect (getLocalBounds());
        buttonRect.setWidth(buttonRect.getWidth() - 4);
        buttonRect.setX(buttonRect.getX() + 2);
        button.setBounds(buttonRect);
    }
    
    void buttonClicked(Button* changedButton)
    {
        if (changedButton == &button)
        {
            if (tree.hasType(Columns::libraryItemIdentifier))
            {
                ValueTree cueTree(Columns::libraryCuePointIdentifier);
                tree.addChild(cueTree, -1, nullptr);
                tree = cueTree;
            }
            
            String propertyName("cue");
            propertyName << (tree.getNumProperties() + 1);
            String property(ownerListBoxModel->filePlayer->getCurrentPosition());
            property << "," << String(Colours::white.getARGB());
            tree.setProperty (propertyName, property, nullptr);

            ownerListBoxModel->owner->updateContent();            
        }
    }
    
private:
    
    CuePointListBoxModel* ownerListBoxModel;
    ValueTree& tree;
    TextButton button;
};

//==============================================================================
class CuePointItem :    public Component,
                        public Button::Listener
{
public:
    CuePointItem(CuePointListBoxModel* _ownerListBoxModel, Identifier _propertyName, float _time, uint32 _colour)
    : ownerListBoxModel(_ownerListBoxModel),
      propertyName(_propertyName),
      time(_time),
      colour(_colour)
    {
        colourButton.setColour(TextButton::buttonColourId, Colour(colour));
        colourButton.addListener(this);
        addAndMakeVisible(&colourButton);
        
        timeButton.setButtonText(String(time, 2));
        timeButton.addListener(this);
        addAndMakeVisible(&timeButton);
        
        removeButton.setButtonText("X");
        removeButton.addListener(this);
        addAndMakeVisible(&removeButton);
    }
    
    ~CuePointItem()
    {
        colourButton.removeListener(this);
        timeButton.removeListener(this);
        removeButton.removeListener(this);
    }
    
    void resized()
    {
        const int w = getWidth();
        const int h = getHeight();
        const int m = 2;
   
        colourButton.setBounds(m, m, h - (2 * m), h - (2 * m));
        timeButton.setBounds(colourButton.getRight() + (2 * m), m, w - (2 * h) - (2 * m), h - (2 * m));
        removeButton.setBounds(w - h + m, m, h - (2 * m), h - (2 * m));
    }
        
    void buttonClicked(Button* button)
    {
        if (button == &colourButton)
        {
            ColourSelector colourSelector (ColourSelector::showColourAtTop | ColourSelector::showColourspace);
            colourSelector.setSize (200, 200);
            
            CallOutBox callOut (colourSelector, colourButton, nullptr);
            callOut.runModalLoop();
            
            setProperties(time, colourSelector.getCurrentColour().getARGB());
        }
        else if (button == &timeButton)
        {
            ownerListBoxModel->filePlayer->setPosition(time);
        }
        else if (button == &removeButton)
        {
            ownerListBoxModel->dataList.removeProperty(propertyName, nullptr);
            ownerListBoxModel->getNumRows();
            ownerListBoxModel->reorderCuePoints();
        }
    }
    
    void setProperties (float _time, uint32 _colour)
    {
        time = _time;
        colour = _colour;

        String property(time);
        property << "," << String((unsigned int)colour);
        ownerListBoxModel->dataList.setProperty(propertyName, property, nullptr);
        colourButton.setColour(TextButton::buttonColourId, Colour(colour));
        timeButton.setButtonText(String(time, 2));
        
        repaint();
    }
        
private:
        
    CuePointListBoxModel* ownerListBoxModel;
    Identifier propertyName;
    float time;
    uint32 colour;
    TextButton colourButton, timeButton, removeButton;
};

//==============================================================================
CuePointListBoxModel::CuePointListBoxModel(ListBox* _owner, AudioFilePlayer* _filePlayer)
:   owner(_owner),
    filePlayer(_filePlayer)
{
}

CuePointListBoxModel::~CuePointListBoxModel()
{
}

void CuePointListBoxModel::setValueTree(ValueTree newTree)
{
    dataList = newTree;
}

int CuePointListBoxModel::getNumRows()
{
    if (dataList.hasType (Columns::libraryCuePointIdentifier))
        return dataList.getNumProperties() + 1;
    
    return 1;
}

void CuePointListBoxModel::paintListBoxItem (int rowNumber,
                                             Graphics& g,
                                             int width, int height,
                                             bool rowIsSelected)
{
}

Component* CuePointListBoxModel::refreshComponentForRow (int rowNumber, bool isRowSelected,
                                                         Component* existingComponentToUpdate)
{
    if (rowNumber < getNumRows() - 1)
    {
        if (dataList.hasType (Columns::libraryCuePointIdentifier))
        {
            float time = LoopAndCueHelpers::getTimeFromCueTree (dataList, rowNumber);
            uint32 colour = LoopAndCueHelpers::getColourFromCueTree (dataList, rowNumber);
            
            if (existingComponentToUpdate == nullptr)
            {
                return existingComponentToUpdate = new CuePointItem (this, dataList.getPropertyName(rowNumber), time, colour);
            }
            else
            {
                CuePointItem* cuePointItem = dynamic_cast<CuePointItem*> (existingComponentToUpdate);
                
                if (cuePointItem != nullptr)
                {
                    cuePointItem->setProperties (time, colour);
                    return existingComponentToUpdate;
                }
                else
                {
                    delete existingComponentToUpdate;
                    return existingComponentToUpdate = new CuePointItem (this, dataList.getPropertyName(rowNumber), time, colour);
                }
            }
        }
    }
    else if (rowNumber == getNumRows() - 1)
    {
        if (existingComponentToUpdate == nullptr) 
        {
            return existingComponentToUpdate = new NewCuePointItem (this, dataList);
        }
        else
        {
            delete existingComponentToUpdate;
            return existingComponentToUpdate = new NewCuePointItem (this, dataList);
        }
    }

    return nullptr;
}

void CuePointListBoxModel::reorderCuePoints()
{
    if (dataList.hasType(Columns::libraryCuePointIdentifier))
    {
        const int numProperties = dataList.getNumProperties();
        var values[numProperties];
        for (int i = 0; i < numProperties; i++)
        {
            values[i] = dataList.getProperty(dataList.getPropertyName(i));
        }

        dataList.removeAllProperties(nullptr);
        
        for (int i = 0; i < numProperties; i++)
        {
            String name("cue");
            name<<i+1;
            dataList.setProperty(name, values[i], nullptr);
        }
        
        owner->updateContent();
    }
}

//==============================================================================
