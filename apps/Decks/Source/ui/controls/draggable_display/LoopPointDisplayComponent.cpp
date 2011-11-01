/*
  ==============================================================================

    LoopPointDisplayComponent.cpp
    Created: 7 Aug 2011 4:57:11am
    Author:  David Rowland

  ==============================================================================
*/

#include "LoopPointDisplayComponent.h"

//==============================================================================
class LoopPointMarker
{
public:
    LoopPointMarker(double _startTime, double _endTime, uint32 _colour)
    :   startTime (_startTime),
        endTime (_endTime),
        colour (_colour)
    {
    }
    
    ~LoopPointMarker() {}
    
    void refreshImage (const int w, const int h, const bool isEnabled =false)
    {
        loopPointImage = Image (Image::RGB,
                                w, h,
                                true);
        
        Path p;
        p.addRectangle(0.0f, 0.0f, w, h);
        
        Graphics g (loopPointImage);
        g.setColour (colour.withAlpha (isEnabled ? 0.75f : 0.5f));
        g.fillPath(p);
        
        g.setColour(Colours::black);
        g.strokePath(p, PathStrokeType (1.0f));
    }
        
private:
    
    friend class LoopPointDisplayComponent;
    
    Image loopPointImage;
    double startTime, endTime;
    Colour colour;
    
    JUCE_LEAK_DETECTOR (LoopPointMarker);
};

//==============================================================================

//==============================================================================
LoopPointDisplayComponent::LoopPointDisplayComponent(AudioFilePlayer* playerToRespondTo, int numSamplesPerThumbnailSample)
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

LoopPointDisplayComponent::~LoopPointDisplayComponent()
{
    filePlayer->removeListener(this);
    loopPointTree.removeListener(this);
}

void LoopPointDisplayComponent::resized()
{
    const int h = getHeight();
    
    for (int i = 0; i < markerArray.size(); ++i)
        markerArray[i]->refreshImage (roundToInt(timeToPixels (markerArray[i]->endTime - markerArray[i]->startTime)), h);
}

void LoopPointDisplayComponent::paint (Graphics& g)
{
    const int w = getWidth();
    const double currentTime = filePlayer->getCurrentPosition();
    const double centrePos = playheadPosition * w;
    
    const double timeAtOrigin = currentTime - (centrePos * timePerPixel);
    
    for (int i = 0; i < markerArray.size(); ++i)
    {
        if (markerArray[i]->endTime > timeAtOrigin)
        {
            const double xPos = timeToPixels (markerArray[i]->startTime - currentTime) + centrePos;
            g.drawImageAt(markerArray[i]->loopPointImage,
                          roundToInt(xPos), 0);
        }        
    }
}

void LoopPointDisplayComponent::fileChanged (AudioFilePlayer *player)
{
    if (player == filePlayer)
    {
        currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
        timePerPixel = scale / currentSampleRate / zoomRatio;
        oneOverTimePerPixel = 1.0 / timePerPixel;

        loopPointTree.removeListener(this);
        loopPointTree = filePlayer->getLibraryEntry().getChildWithName(MusicColumns::libraryLoopIdentifier);
        loopPointTree.addListener(this);

        updateLoopPoints();
    }
}

void LoopPointDisplayComponent::loopStateChanged (AudioFilePlayer *player, int loopIndex, bool isEnabled)
{
    if (player == filePlayer)
    {
        if (loopIndex >= 0 && loopIndex < markerArray.size())
            markerArray[loopIndex]->refreshImage (roundToInt(timeToPixels (markerArray[loopIndex]->endTime - markerArray[loopIndex]->startTime)), getHeight(), isEnabled);
    }
}

void LoopPointDisplayComponent::valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property)
{
    if (treeWhosePropertyHasChanged == loopPointTree)
        updateLoopPoints();
}

void LoopPointDisplayComponent::timerCallback()
{
    repaint();
}

void LoopPointDisplayComponent::setZoomFactor (float newZoomFactor)
{
    zoomRatio = newZoomFactor;
    timePerPixel = scale / currentSampleRate / zoomRatio;
    oneOverTimePerPixel = 1.0 / timePerPixel;
    resized();
}

void LoopPointDisplayComponent::setPlayheadPosition (float newPlayHeadPosition)
{
    playheadPosition = newPlayHeadPosition;
}

void LoopPointDisplayComponent::updateLoopPoints()
{
    if (loopPointTree.isValid())
    {
        markerArray.clear();

        for (int i = 0; i < loopPointTree.getNumProperties(); ++i)
        {
            double starTime, endTime;
            uint32 col;
            LoopAndCueHelpers::getTimeAndColourFromLoopTree (loopPointTree, i, starTime, endTime, col);
            markerArray.add (new LoopPointMarker (starTime, endTime, col));
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
