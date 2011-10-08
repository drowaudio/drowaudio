/*
  ==============================================================================

    CuePointDisplayComponent.cpp
    Created: 7 Aug 2011 4:57:11am
    Author:  David Rowland

  ==============================================================================
*/

#include "CuePointDisplayComponent.h"

//==============================================================================
class CuePointMarker
{
public:
    CuePointMarker(double _time, uint32 _colour)
    :   time (_time),
        colour (_colour)
    {
    }
    
    ~CuePointMarker() {}
    
    void refreshImage (const int w, const int h)
    {
        cuePointImage = Image (Image::RGB,
                               w, h,
                               true);
        
        Path p;
        p.addTriangle (0, 0,
                        w * 0.5, w,
                        w, 0);
        p.addTriangle (0, h,
                        w * 0.5, h - w,
                        w, h);
        
        Graphics g (cuePointImage);
        g.setColour (colour);
        g.fillPath(p);
        
        g.setColour(Colours::black);
        g.strokePath(p, PathStrokeType (1.0f));
    }
        
private:
    
    friend class CuePointDisplayComponent;
    
    Image cuePointImage;
    double time;
    Colour colour;
    
    JUCE_LEAK_DETECTOR (CuePointMarker);
};

//==============================================================================

//==============================================================================
CuePointDisplayComponent::CuePointDisplayComponent(AudioFilePlayer* playerToRespondTo, int numSamplesPerThumbnailSample)
:   filePlayer (playerToRespondTo),
    scale (numSamplesPerThumbnailSample),
    zoomRatio (1.0f),
    currentSampleRate (44100.0),
    timePerPixel (1.0),
    oneOverTimePerPixel (1.0)
{
    setInterceptsMouseClicks(false, false);
    filePlayer->addListener(this);
}

CuePointDisplayComponent::~CuePointDisplayComponent()
{
    filePlayer->removeListener(this);
    cuePointTree.removeListener(this);
}

void CuePointDisplayComponent::resized()
{
    const int h = getHeight();
    
    for (int i = 0; i < markerArray.size(); ++i)
        markerArray[i]->refreshImage (15, h);
}

void CuePointDisplayComponent::paint (Graphics& g)
{
    const int w = getWidth();
    const double currentTime = filePlayer->getCurrentPosition();
    const double centrePos = playheadPosition * w;
    
    const double timeAtOrigin = currentTime - (centrePos * timePerPixel);
    const double timeAtEnd = timeAtOrigin + (w * timePerPixel);
    
    for (int i = 0; i < markerArray.size(); ++i)
    {
        const double time = markerArray[i]->time;
        if (time > timeAtOrigin && time < timeAtEnd)
        {
            const double xPos = timeToPixels (time - currentTime) + centrePos;
            g.drawImageAt(markerArray[i]->cuePointImage,
                          roundToInt(xPos - (markerArray[i]->cuePointImage.getWidth() * 0.5)), 0);
        }        
    }
}

void CuePointDisplayComponent::fileChanged (AudioFilePlayer *player)
{
    if (player == filePlayer)
    {
        currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
        timePerPixel = scale / currentSampleRate / zoomRatio;
        oneOverTimePerPixel = 1.0 / timePerPixel;

        cuePointTree.removeListener(this);
        cuePointTree = filePlayer->getLibraryEntry().getChildWithName (Columns::libraryCuePointIdentifier);
        if (cuePointTree.isValid())
        {
            cuePointTree.addListener(this);
            DBG("adding cue point");
        }
        else
        {
            filePlayer->getLibraryEntry().addListener(this);
            DBG("adding item tree listener");
        }
            
        updateCuePoints();
    }
}

void CuePointDisplayComponent::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    DBG("valueTreePropertyChanged");
    if (treeWhosePropertyHasChanged == cuePointTree)
        updateCuePoints();
}

void CuePointDisplayComponent::valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded)
{
    DBG("valueTreeChildAdded");
    if (childWhichHasBeenAdded.hasType(Columns::libraryCuePointIdentifier))
    {
        cuePointTree.removeListener(this);
        cuePointTree = childWhichHasBeenAdded;
        cuePointTree.addListener(this);
    }
}

void CuePointDisplayComponent::timerCallback()
{
    repaint();
}

void CuePointDisplayComponent::setZoomFactor (float newZoomFactor)
{
    zoomRatio = newZoomFactor;
    timePerPixel = scale / currentSampleRate / zoomRatio;
    oneOverTimePerPixel = 1.0 / timePerPixel;
}

void CuePointDisplayComponent::setPlayheadPosition (float newPlayHeadPosition)
{
    playheadPosition = newPlayHeadPosition;
}

void CuePointDisplayComponent::updateCuePoints()
{
    if (cuePointTree.isValid())
    {
        markerArray.clear();

        for (int i = 0; i < cuePointTree.getNumProperties(); ++i)
        {
            markerArray.add (new CuePointMarker (LoopAndCueHelpers::getTimeFromCueTree (cuePointTree, i),
                                                 LoopAndCueHelpers::getColourFromCueTree (cuePointTree, i)));
        }
        
        resized();
        startTimer (15);
    }
    else
    {
        stopTimer();
    }
}
//==============================================================================
