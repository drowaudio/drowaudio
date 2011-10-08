/*
  ==============================================================================

    ClickableLoopPointComponent.cpp
    Created: 6 Aug 2011 6:39:41pm
    Author:  David Rowland

  ==============================================================================
*/

#include "ClickableLoopPointComponent.h"

/*==============================================================================
    A component to represent a single cue point that can respond to clicks.
//============================================================================*/
class LoopPointComponent : public Component
{
public:
    
    LoopPointComponent (AudioFilePlayer* playerToControl, double _startTime, double _endTime, Colour _colour)
    :   player (playerToControl),
        startTime (_startTime),
        endTime (_endTime),
        colour (_colour)
    {
    }
    
    ~LoopPointComponent()
    {
    }
    
    void resized()
    {
        const int w = jmax(1, getWidth());
        const int h = jmax(1, getHeight());
        
        iconImage = Image (Image::RGB,
                           w, h,
                           true);
        
        Path p;
        p.addRectangle(0, 0, w, h);
//        p.startNewSubPath (0.0f, h);
//        p.lineTo (w * 0.5, 0);
//        p.lineTo (w, h);
//        p.closeSubPath();
        
        Graphics g (iconImage);
        g.setColour(colour.withAlpha (0.75f));
        g.fillPath(p);
        
        g.setColour(Colours::black);
        g.strokePath(p, PathStrokeType (1.0f));        
    }
    
    void paint (Graphics& g)
    {
        g.drawImageAt(iconImage,
                      0, 0, false);
    }

    //==============================================================================
    void mouseEnter (const MouseEvent& e)
    {
        setMouseCursor(MouseCursor::PointingHandCursor);
    }

    void mouseExit (const MouseEvent& e)
    {
        setMouseCursor(MouseCursor::NormalCursor);
    }
    
    void mouseDown (const MouseEvent& e)
    {
        player->setPosition(startTime);
    }
    
    //==============================================================================

    
private:
    
    friend class ClickableLoopPointComponent;
    
    AudioFilePlayer* player;
    double startTime, endTime;
    Colour colour;
    Image iconImage;
    
    JUCE_LEAK_DETECTOR (LoopPointComponent);
};
//==============================================================================

ClickableLoopPointComponent::ClickableLoopPointComponent (AudioFilePlayer* playerToRespondTo)
:   filePlayer (playerToRespondTo)
{
    setInterceptsMouseClicks(false, true);
    filePlayer->addListener(this);
}

ClickableLoopPointComponent::~ClickableLoopPointComponent()
{
    filePlayer->removeListener(this);
    loopPointTree.removeListener(this);
    
    removeAllChildren();
    loopPoints.clear();
}

void ClickableLoopPointComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    
    if (filePlayer != nullptr)
    {
        const double fileLength = filePlayer->getLengthInSeconds();

        for (int i = 0; i < loopPoints.size(); ++i)
        {
            const int x = ((loopPoints[i]->startTime / fileLength) * w);
            const int wL = ((loopPoints[i]->endTime - loopPoints[i]->startTime) / fileLength) * w;
            loopPoints[i]->setBounds (x, 0, wL, h);
        }
    }
}

//==============================================================================
void ClickableLoopPointComponent::fileChanged (AudioFilePlayer* player)
{
    if (player == filePlayer)
    {
        loopPointTree.removeListener(this);
        loopPointTree = filePlayer->getLibraryEntry().getChildWithName(Columns::libraryLoopIdentifier);
        loopPointTree.addListener(this);
        
        updateLoopPoints();
    }
}

void ClickableLoopPointComponent::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged == loopPointTree)
        updateLoopPoints();
}

void ClickableLoopPointComponent::updateLoopPoints()
{
    removeAllChildren();
    loopPoints.clear();
    
    if (loopPointTree.isValid())
    {
        for (int i = 0; i < loopPointTree.getNumProperties(); ++i)
        {
            double start, end;
            uint32 col;
            LoopAndCueHelpers::getTimeAndColourFromLoopTree (loopPointTree, i, start, end, col);
            
            loopPoints.add(new LoopPointComponent(filePlayer, start, end, Colour(col)));
            addAndMakeVisible (loopPoints[i]);
        }
    }
    
    resized();    
}

//==============================================================================