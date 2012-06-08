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



AudioTransportCursor::AudioTransportCursor (AudioFilePlayer& sourceToBeUsed)
    : audioFilePlayer       (sourceToBeUsed),
      currentSampleRate     (44100.0),
      zoomRatio             (1.0),
      startOffsetRatio      (0.0),
      shouldStopTimer       (false),
      showTransportCursor   (true)
{
    audioFilePlayer.addListener (this);
}

AudioTransportCursor::~AudioTransportCursor()
{
    audioFilePlayer.removeListener (this);
	stopTimer();
}

void AudioTransportCursor::setZoomRatio (double newZoomRatio)
{
    if (newZoomRatio < 0.0 || newZoomRatio > 10000.0)
    {
        jassertfalse; // zoom ratio has to be > 0 && < 10000
        newZoomRatio = 1.0;
    }

    zoomRatio = newZoomRatio;
    
    repaint();
}

void AudioTransportCursor::setStartOffsetRatio (double newStartOffsetRatio)
{
    startOffsetRatio = newStartOffsetRatio;
    
    repaint();
}

void AudioTransportCursor::setCursorDisplayed (bool shouldDisplayCursor)
{
    showTransportCursor = shouldDisplayCursor;
    
    startTimerIfNeeded();
}

//====================================================================================
void AudioTransportCursor::resized()
{
    cursorImage = Image (Image::RGB, 3, jmax (1, getHeight()), true);
    Graphics g (cursorImage);
    g.fillAll (Colours::black);
    g.setColour (Colours::white);
	g.drawVerticalLine (1, 0.0f, (float) cursorImage.getHeight());
}

void AudioTransportCursor::paint (Graphics &g)
{
    if (showTransportCursor)
        g.drawImageAt (cursorImage, transportLineXCoord.getCurrent() - 1, 0);
}

//====================================================================================
void AudioTransportCursor::timerCallback()
{
    const int w = getWidth();
    const int h = getHeight();

    const int startPixel = roundToInt (w * startOffsetRatio);
    transportLineXCoord = startPixel + roundToInt ((w * oneOverFileLength * audioFilePlayer.getCurrentPosition()) / zoomRatio);
    
    // if the line has moved repaint the old and new positions of it
    if (! transportLineXCoord.areEqual())
    {
        repaint (transportLineXCoord.getPrevious() - 2, 0, 5, h);
        repaint (transportLineXCoord.getCurrent() - 2, 0, 5, h);
	}
    
    if (shouldStopTimer)
    {
        shouldStopTimer = false;
        stopTimer();
    }
}

//====================================================================================
void AudioTransportCursor::fileChanged (AudioFilePlayer* player)
{
	if (player == &audioFilePlayer)
	{
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
        
        startTimerIfNeeded();
	}
}

void AudioTransportCursor::playerStoppedOrStarted (AudioFilePlayer* player)
{
	if (player == &audioFilePlayer)
        startTimerIfNeeded();
}

//==============================================================================
void AudioTransportCursor::mouseDown (const MouseEvent &e)
{
    if (showTransportCursor)
    {
        setMouseCursor (MouseCursor::IBeamCursor);
        currentMouseX = e.x;

        const int w = getWidth();
        currentXScale = (float) ((fileLength / w) * zoomRatio);

        const int startPixel = roundToInt (w * startOffsetRatio);
        double position = currentXScale * (currentMouseX - startPixel);
        audioFilePlayer.setPosition (position, true);

        startTimer (40);
        
        repaint();
    }
}

void AudioTransportCursor::mouseUp (const MouseEvent& /*e*/)
{
    if (showTransportCursor)
    {
        setMouseCursor (MouseCursor::NormalCursor);
        
        startTimerIfNeeded();
    }
}

void AudioTransportCursor::mouseDrag (const MouseEvent& e)
{
    if (showTransportCursor)
    {
        currentMouseX = e.x;
        
        const int w = getWidth();
        const int startPixel = roundToInt (w * startOffsetRatio);
        double position = currentXScale * (currentMouseX - startPixel);
        audioFilePlayer.setPosition (position, false);
    }
}

//==============================================================================
void AudioTransportCursor::startTimerIfNeeded()
{
    if (audioFilePlayer.isPlaying() && showTransportCursor)
    {
        shouldStopTimer = false;
        startTimer (40);
    }
    else
    {
        shouldStopTimer = true;
    }
}

