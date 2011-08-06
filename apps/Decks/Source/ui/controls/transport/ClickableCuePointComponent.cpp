/*
  ==============================================================================

    ClickableCuePointComponent.cpp
    Created: 6 Aug 2011 6:39:41pm
    Author:  David Rowland

  ==============================================================================
*/

#include "ClickableCuePointComponent.h"

/*==============================================================================
    A component to represent a single cue point that can respond to clicks.
//============================================================================*/
class CuePointComponent : public Component
{
public:
    
    CuePointComponent (FilteringAudioFilePlayer* playerToControl, double _time, Colour _colour)
    :   player (playerToControl),
        time (_time),
        colour (_colour)
    {
    }
    
    ~CuePointComponent()
    {
    }
    
    void resized()
    {
        const int w = jmax(1, getWidth() * 4);
        const int h = jmax(1, getHeight() * 4);
        
        iconImage = Image (Image::RGB,
                           w, h,
                           true);
        
        Path p;
        p.startNewSubPath (0.0f, h);
        p.lineTo (w * 0.5, 0);
        p.lineTo (w, h);
        p.closeSubPath();
        
        Graphics g (iconImage);
        g.setColour(colour);
        g.fillPath(p);
        
        g.setColour(Colours::black);
        g.strokePath(p, PathStrokeType (2));        
    }
    
    void paint (Graphics& g)
    {
        g.drawImage(iconImage,
                    0, 0, getWidth(), getHeight(),
                    0, 0, iconImage.getWidth(), iconImage.getHeight(),
                    false);
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
        player->getAudioTransportSource()->setPosition(time);
    }
    
    bool hitTest (int x, int y)
    {
        const int w = getWidth();
        
        if ((x < (0.5 * w)) && (y > -(2 * x - w)))
            return true;
        else if ((x > (0.5 * w)) && (y > -(2 * x + w)))
            return true;
            
        return false;
    }

    //==============================================================================
    inline double getTime()    {   return time;    }
    
    //==============================================================================

    
private:
    
    FilteringAudioFilePlayer* player;
    double time;
    Colour colour;
    Image iconImage;
    
    JUCE_LEAK_DETECTOR (CuePointComponent);
};
//==============================================================================

ClickableCuePointComponent::ClickableCuePointComponent(FilteringAudioFilePlayer* playerToRespondTo)
:   filePlayer (playerToRespondTo)
{
    setInterceptsMouseClicks(false, true);
    filePlayer->addListener(this);
}

ClickableCuePointComponent::~ClickableCuePointComponent()
{
    filePlayer->removeListener(this);
    cuePointTree.removeListener(this);
}

void ClickableCuePointComponent::resized()
{
    const int w = getWidth();
    const int h = getHeight();
    
    if (filePlayer != nullptr)
    {
        const double fileLength = filePlayer->getAudioTransportSource()->getLengthInSeconds();

        for (int i = 0; i < cuePoints.size(); ++i)
        {
            const int x = ((cuePoints[i]->getTime() / fileLength) * w) - (h * 0.5);
            cuePoints[i]->setBounds (x, 0, h, h);
        }
    }
}

//==============================================================================
void ClickableCuePointComponent::fileChanged (FilteringAudioFilePlayer* player)
{
    if (player == filePlayer)
    {
        cuePointTree.removeListener(this);
        cuePointTree = filePlayer->getLibraryEntry().getChildWithName(Columns::libraryCuePointIdentifier);
        cuePointTree.addListener(this);
        
        updateCuePoints();
    }
}

void ClickableCuePointComponent::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged == cuePointTree)
        updateCuePoints();
}

void ClickableCuePointComponent::updateCuePoints()
{
    removeAllChildren();
    cuePoints.clear();
    
    if (cuePointTree.isValid())
    {
        for (int i = 0; i < cuePointTree.getNumProperties(); ++i)
        {
            cuePoints.add(new CuePointComponent(filePlayer,
                                                LoopAndCueHelpers::getTimeFromCueString (cuePointTree, i),
                                                Colour (LoopAndCueHelpers::getColourFromCueString (cuePointTree, i))));
            addAndMakeVisible(cuePoints[i]);
        }
    }
    
    resized();    
}

//==============================================================================