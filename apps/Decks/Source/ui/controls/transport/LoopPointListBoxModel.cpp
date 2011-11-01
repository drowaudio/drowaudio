/*
  ==============================================================================

    LoopPointListBoxModel.cpp
    Created: 11 Jul 2011 9:47:51am
    Author:  David Rowland

  ==============================================================================
*/

#include "LoopPointListBoxModel.h"

//==============================================================================
class NewLoopPointItem :    public Component,
                            public Button::Listener
{
public:
    NewLoopPointItem(LoopPointListBoxModel* _ownerListBoxModel, ValueTree& treeToAddTo)
    :   ownerListBoxModel(_ownerListBoxModel),
        tree(treeToAddTo)
    {
        button.setButtonText("+");
        button.addListener(this);
        addAndMakeVisible(&button);
    }
    
    ~NewLoopPointItem()
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
            if (tree.hasType(MusicColumns::libraryItemIdentifier))
            {
                ValueTree cueTree(MusicColumns::libraryLoopIdentifier);
                tree.addChild(cueTree, -1, nullptr);
                tree = cueTree;
            }
            
            String propertyName("loop");
            propertyName << (tree.getNumProperties() + 1);
            String property(ownerListBoxModel->filePlayer->getCurrentPosition());
            property << "," << (ownerListBoxModel->filePlayer->getCurrentPosition() + 10);
            property << "," << String(Colours::white.getARGB());
            tree.setProperty (propertyName, property, nullptr);

            ownerListBoxModel->owner->updateContent();            
        }
    }
    
private:
    
    LoopPointListBoxModel* ownerListBoxModel;
    ValueTree& tree;
    TextButton button;
};

//==============================================================================
class LoopPointItem :   public Component,
                        public Button::Listener
{
public:
    LoopPointItem(LoopPointListBoxModel* _ownerListBoxModel, int _loopIndex, Identifier _propertyName,  float _startTime, float _endTime, uint32 _colour)
    : ownerListBoxModel(_ownerListBoxModel),
      loopIndex (_loopIndex),
      propertyName(_propertyName),
      startTime(_startTime),
      endTime(_endTime),
      colour(_colour)
    {
        colourButton.setColour(TextButton::buttonColourId, Colour(colour));
        colourButton.addListener(this);
        addAndMakeVisible(&colourButton);
        
        String buttonText (String (startTime, 2) + " - " + String (endTime, 2));
        timeButton.setButtonText (buttonText);
        timeButton.addListener(this);
        timeButton.setClickingTogglesState(true);
        addAndMakeVisible(&timeButton);
        
        removeButton.setButtonText("X");
        removeButton.addListener(this);
        addAndMakeVisible(&removeButton);
    }
    
    ~LoopPointItem() {}
    
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
            
            setProperties(startTime, endTime, colourSelector.getCurrentColour().getARGB());
        }
        else if (button == &timeButton)
        {
//            ownerListBoxModel->filePlayer->setActiveLoopPoint (startTime, endTime, loopIndex, timeButton.getToggleState());
            ownerListBoxModel->filePlayer->setLoopTimes (startTime, endTime);
            ownerListBoxModel->filePlayer->setLoopBetweenTimes (timeButton.getToggleState());
        }
        else if (button == &removeButton)
        {
            ownerListBoxModel->dataList.removeProperty(propertyName, nullptr);
            ownerListBoxModel->getNumRows();
            ownerListBoxModel->reorderLoopPoints();
        }
    }
    
    void setProperties (float _startTime, float _endTime, uint32 _colour)
    {
        startTime = _startTime;
        endTime = _endTime;
        colour = _colour;

        String property (startTime);
        property << "," << endTime;
        property << "," << String ((unsigned int)colour);
        ownerListBoxModel->dataList.setProperty (propertyName, property, nullptr);
        colourButton.setColour (TextButton::buttonColourId, Colour (colour));
        String buttonText (String (startTime, 2) + " - " + String (endTime, 2));
        timeButton.setButtonText (buttonText);
        
        repaint();
    }
        
private:
        
    LoopPointListBoxModel* ownerListBoxModel;
    int loopIndex;
    Identifier propertyName;
    float startTime, endTime;
    uint32 colour;
    TextButton colourButton, timeButton, removeButton;
};

//==============================================================================
LoopPointListBoxModel::LoopPointListBoxModel(ListBox* _owner, AudioFilePlayer* _filePlayer)
:   owner(_owner),
    filePlayer(_filePlayer)
{
}

LoopPointListBoxModel::~LoopPointListBoxModel()
{
}

void LoopPointListBoxModel::setValueTree(ValueTree newTree)
{
    dataList = newTree;
}

int LoopPointListBoxModel::getNumRows()
{
    if (dataList.hasType (MusicColumns::libraryLoopIdentifier))
        return dataList.getNumProperties() + 1;
    
    return 1;
}

void LoopPointListBoxModel::paintListBoxItem (int rowNumber,
                                             Graphics& g,
                                             int width, int height,
                                             bool rowIsSelected)
{
}

Component* LoopPointListBoxModel::refreshComponentForRow (int rowNumber, bool isRowSelected,
                                                         Component* existingComponentToUpdate)
{
    if (rowNumber < getNumRows() - 1)
    {
        if (dataList.hasType (MusicColumns::libraryLoopIdentifier))
        {
            double startTime, endTime;
            uint32 colour;
            LoopAndCueHelpers::getTimeAndColourFromLoopTree (dataList, rowNumber, startTime, endTime, colour);
            
            if (existingComponentToUpdate == nullptr)
            {
                return existingComponentToUpdate = new LoopPointItem (this, rowNumber, dataList.getPropertyName(rowNumber), startTime, endTime, colour);
            }
            else
            {
                LoopPointItem* cuePointItem = dynamic_cast<LoopPointItem*> (existingComponentToUpdate);
                
                if (cuePointItem != nullptr)
                {
                    cuePointItem->setProperties (startTime, endTime, colour);
                    return existingComponentToUpdate;
                }
                else
                {
                    delete existingComponentToUpdate;
                    return existingComponentToUpdate = new LoopPointItem (this, rowNumber, dataList.getPropertyName(rowNumber), startTime, endTime, colour);
                }
            }
        }
    }
    else if (rowNumber == getNumRows() - 1)
    {
        if (existingComponentToUpdate == nullptr) 
        {
            return existingComponentToUpdate = new NewLoopPointItem(this, dataList);
        }
        else
        {
            delete existingComponentToUpdate;
            return existingComponentToUpdate = new NewLoopPointItem(this, dataList);
        }
    }

    return nullptr;
}

void LoopPointListBoxModel::reorderLoopPoints()
{
    if (dataList.hasType(MusicColumns::libraryLoopIdentifier))
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
            String name("loop");
            name << i + 1;
            dataList.setProperty(name, values[i], nullptr);
        }
        
        owner->updateContent();
    }
}

//==============================================================================
