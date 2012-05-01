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



DraggableWaveDisplay::DraggableWaveDisplay (AudioThumbnailImage& sourceToBeUsed)
    : audioThumbnailImage           (sourceToBeUsed),
	  filePlayer                    (sourceToBeUsed.getAudioFilePlayer()),
	  currentSampleRate             (44100),
      oneOverSampleRate             (1.0 / currentSampleRate),
	  timePerPixel                  (1.0),
	  zoomRatio                     (1.0f),
      oneOverZoomRatio              (1.0f),
      playheadPos                   (0.5f),
	  isDraggable                   (true),
	  mouseShouldTogglePlay         (true)
{
    setOpaque (true);

    audioThumbnailImage.addListener (this);
    
	timePerPixel = audioThumbnailImage.getNumSourceSamplesPerThumbnailSamples() * oneOverSampleRate;
}

DraggableWaveDisplay::~DraggableWaveDisplay()
{
    audioThumbnailImage.removeListener (this);
	stopTimer (waveformUpdated);
}

//====================================================================================
void DraggableWaveDisplay::setHorizontalZoom (float newZoomFactor)
{
	jassert (newZoomFactor > 0.0f);
	
	zoomRatio = newZoomFactor;
    oneOverZoomRatio = 1.0f / zoomRatio;
    
    repaint();
}

void DraggableWaveDisplay::setPlayheadPosition (float newPlayheadPosition)
{
	playheadPos = jlimit (0.0f, 1.0f, newPlayheadPosition);
    
    repaint();
}

void DraggableWaveDisplay::setDraggable (bool isWaveformDraggable)
{
	isDraggable = isWaveformDraggable;
}

//====================================================================================
void DraggableWaveDisplay::resized()
{
    // redraw playhead image
    playheadImage = Image (Image::RGB, 3, getHeight(), false);

    Graphics g (playheadImage);
    g.fillAll (Colours::black);
    g.setColour (Colours::white);
    g.drawVerticalLine (1, 0.0f, (float) playheadImage.getHeight());
}

void DraggableWaveDisplay::paint (Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();
	
	g.fillAll (Colours::darkgrey);
	    
    const int playHeadXPos = roundToInt (playheadPos * w);
    const double timeToPlayHead = pixelsToTime (playHeadXPos);
    const double startTime = filePlayer->getAudioTransportSource()->getCurrentPosition() - timeToPlayHead;
    const double duration = filePlayer->getAudioTransportSource()->getLengthInSeconds();
    const double timeToDisplay = pixelsToTime (w);
    
    Image clippedImage (audioThumbnailImage.getImageAtTime (startTime, timeToDisplay));

    int padLeft = 0, padRight = 0;
    if (startTime < 0.0)
        padLeft = roundToInt (timeToPixels (abs (startTime)));
    if ((startTime + timeToDisplay) > duration)
        padRight = roundToInt (timeToPixels (abs (duration - (startTime + timeToDisplay))));
    
    g.drawImage (clippedImage,
                 padLeft, 0, w - padLeft - padRight, h,
                 0, 0, clippedImage.getWidth(), clippedImage.getHeight(),
                 false);
    
    g.drawImageAt (playheadImage, playHeadXPos - 1, 0);
}

void DraggableWaveDisplay::mouseDown (const MouseEvent &e)
{
	mouseX.setBoth (e.x);
	isMouseDown = true;
	
	if (isDraggable)
	{
		if (mouseShouldTogglePlay)
		{
			if (filePlayer->getAudioTransportSource()->isPlaying())
			{
				shouldBePlaying = true;
				filePlayer->getAudioTransportSource()->stop();
			}
			else
            {
				shouldBePlaying = false;
            }
		}
        
		setMouseCursor (MouseCursor::DraggingHandCursor);
		
		startTimer (waveformMoved, 40);
	}
}

void DraggableWaveDisplay::mouseUp (const MouseEvent& /*e*/)
{
	isMouseDown = false;
	
	if (isDraggable)
	{
		if (mouseShouldTogglePlay)
		{
			if (shouldBePlaying && ! filePlayer->getAudioTransportSource()->isPlaying())
				filePlayer->getAudioTransportSource()->start();
			else if (! shouldBePlaying && filePlayer->getAudioTransportSource()->isPlaying())
				filePlayer->getAudioTransportSource()->stop();
		}
		
		setMouseCursor (MouseCursor::NormalCursor);
		
		stopTimer (waveformMoved);
	}
}

//====================================================================================
void DraggableWaveDisplay::timerCallback (const int timerId)
{
	if (timerId == waveformUpdated) //moved due to file position changing
	{
		movedX = roundToInt (timeToPixels (filePlayer->getAudioTransportSource()->getCurrentPosition()));

		if (! movedX.areEqual())
			repaint();
	}
	else if (timerId == waveformMoved) // being dragged
	{
		if (isMouseDown)
		{
			mouseX = getMouseXYRelative().getX();;
			const int currentXDrag = mouseX.getDifference();
			
			if (currentXDrag != 0)
			{
				double position = (filePlayer->getAudioTransportSource()->getCurrentPosition() - pixelsToTime (currentXDrag));

				filePlayer->getAudioTransportSource()->setPosition (position);

				repaint();
			}
		}
	}
}

void DraggableWaveDisplay::imageChanged (AudioThumbnailImage* changedAudioThumbnailImage)
{
	if (changedAudioThumbnailImage == &audioThumbnailImage)
	{
        currentSampleRate = 0.0;
        
        if (filePlayer->getAudioFormatReaderSource() != nullptr
            && filePlayer->getAudioFormatReaderSource()->getAudioFormatReader() != nullptr)
            currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
        
        if (currentSampleRate > 0.0)
        {
            oneOverSampleRate = 1.0 / currentSampleRate;
            oneOverFileLength = 1.0 / filePlayer->getAudioTransportSource()->getLengthInSeconds();
            timePerPixel = audioThumbnailImage.getNumSourceSamplesPerThumbnailSamples() * oneOverSampleRate;
            
            startTimer (waveformUpdated, 15);
        }
        else 
        {
            stopTimer (waveformUpdated);
        }
	}
}

//==============================================================================	
double DraggableWaveDisplay::pixelsToTime (double numPixels)
{
	return numPixels * timePerPixel * oneOverZoomRatio;
}

double DraggableWaveDisplay::timeToPixels (double timeInSecs)
{
	return timeInSecs / (timePerPixel * oneOverZoomRatio);
}


