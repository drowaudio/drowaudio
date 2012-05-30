/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/



PositionableWaveDisplay::PositionableWaveDisplay (AudioThumbnailImage& sourceToBeUsed)
    : audioThumbnailImage   (sourceToBeUsed),
      audioFilePlayer       (*audioThumbnailImage.getAudioFilePlayer()),
      currentSampleRate     (44100.0),
      zoomRatio             (1.0),
      startOffsetRatio      (0.0),
      verticalZoomRatio     (1.0),
      backgroundColour      (Colours::black),
      waveformColour        (Colours::green),
      audioTransportCursor  (audioFilePlayer)
{
    setOpaque (true);
    
    audioThumbnailImage.addListener (this);
    
    cachedImage = Image (Image::RGB, 1, 1, false);
    cachedImage.clear (cachedImage.getBounds(), backgroundColour);
    
    addAndMakeVisible (&audioTransportCursor);
}

PositionableWaveDisplay::~PositionableWaveDisplay()
{
    audioThumbnailImage.removeListener (this);
}

void PositionableWaveDisplay::setZoomRatio (double newZoomRatio)
{
    if (newZoomRatio < 0.0 || newZoomRatio > 10000.0)
    {
        jassertfalse; // zoom ratio has to be > 0 && < 10000
        newZoomRatio = 1.0;
    }

    zoomRatio = newZoomRatio;
    audioTransportCursor.setZoomRatio (newZoomRatio);
    
    repaint();
}

void PositionableWaveDisplay::setStartOffsetRatio (double newStartOffsetRatio)
{
    startOffsetRatio = newStartOffsetRatio;
    audioTransportCursor.setStartOffsetRatio (startOffsetRatio);

    repaint();
}

void PositionableWaveDisplay::setVerticalZoomRatio (double newVerticalZoomRatio)
{
    verticalZoomRatio = newVerticalZoomRatio;
    repaint();
}

void PositionableWaveDisplay::setCursorDisplayed (bool shoudldDisplayCursor)
{
    audioTransportCursor.setCursorDisplayed (shoudldDisplayCursor);
}

void PositionableWaveDisplay::setBackgroundColour (Colour newBackgroundColour)
{
    backgroundColour = newBackgroundColour;
    audioThumbnailImage.setBackgroundColour (backgroundColour);
    repaint();
}

void PositionableWaveDisplay::setWaveformColour (Colour newWaveformColour)
{
    waveformColour = newWaveformColour;
    audioThumbnailImage.setWaveformColour (waveformColour);
    repaint();
}

//====================================================================================
void PositionableWaveDisplay::resized()
{
    if (audioThumbnailImage.hasFinishedLoading()) 
    {
        cachedImage = audioThumbnailImage.getImage();
        cachedImage.duplicateIfShared();
        cachedImage = cachedImage.rescaled (getWidth(), getHeight());
    }
    
    audioTransportCursor.setBounds (getLocalBounds());
}

void PositionableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();

    g.setColour (backgroundColour);	
    g.fillAll();
        
    const int startPixelX = roundToInt (w * startOffsetRatio);
    const int newHeight = roundToInt (verticalZoomRatio * h);
    const int startPixelY = roundToInt ((h * 0.5f) - (newHeight * 0.5f));

	g.drawImage (cachedImage,
                 startPixelX, startPixelY, w, newHeight,
                 0, 0, roundToInt (cachedImage.getWidth() * zoomRatio), cachedImage.getHeight(), 
                 false);
}

//====================================================================================
void PositionableWaveDisplay::imageChanged (AudioThumbnailImage* changedAudioThumbnailImage)
{
	if (changedAudioThumbnailImage == &audioThumbnailImage)
	{
        cachedImage = Image::null;

        AudioFormatReaderSource* readerSource = audioFilePlayer.getAudioFormatReaderSource();
        
        AudioFormatReader* reader = nullptr;
        if (readerSource != nullptr)
            reader = readerSource->getAudioFormatReader();
        
        if (reader != nullptr && reader->sampleRate > 0.0
            && audioFilePlayer.getLengthInSeconds() > 0.0)
        {
            currentSampleRate = reader->sampleRate;
            fileLength = audioFilePlayer.getLengthInSeconds();
            oneOverFileLength = 1.0 / fileLength;
        }
        else 
        {
            currentSampleRate = 44100;
            fileLength = 0.0;
            oneOverFileLength = 1.0;
        }
	}
}

void PositionableWaveDisplay::imageUpdated (AudioThumbnailImage* changedAudioThumbnailImage)
{
    if (changedAudioThumbnailImage == &audioThumbnailImage)
    {
        cachedImage = audioThumbnailImage.getImage();
        repaint();
    }
}

void PositionableWaveDisplay::imageFinished (AudioThumbnailImage* changedAudioThumbnailImage)
{
    if (changedAudioThumbnailImage == &audioThumbnailImage)
    {
        resized();
    }
}

//==============================================================================
//void PositionableWaveDisplay::mouseDown (const MouseEvent &e)
//{
//    if (showTransportCursor)
//    {
//        setMouseCursor (MouseCursor::IBeamCursor);
//        currentMouseX = e.x;
//
//        const int w = getWidth();
//        currentXScale = (float) ((fileLength / w) * zoomRatio);
//
//        const int startPixel = roundToInt (w * startOffsetRatio);
//        double position = currentXScale * (currentMouseX - startPixel);
//        audioFilePlayer->setPosition (position, true);
//
//        repaint();
//    }
//}
//
//void PositionableWaveDisplay::mouseUp (const MouseEvent& /*e*/)
//{
//    if (showTransportCursor)
//    {
//        setMouseCursor (MouseCursor::NormalCursor);
//    }
//}
//
//void PositionableWaveDisplay::mouseDrag (const MouseEvent& e)
//{
//    if (showTransportCursor)
//    {
//        currentMouseX = e.x;
//        
//        const int w = getWidth();
//        const int startPixel = roundToInt (w * startOffsetRatio);
//        double position = currentXScale * (currentMouseX - startPixel);
//        audioFilePlayer->setPosition (position, false);
//    }
////	double position = currentXScale * currentMouseX;
////	audioFilePlayer->setPositionIgnoringLoop (position);
//}