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

BEGIN_JUCE_NAMESPACE

ColouredPositionableWaveDisplay::ColouredPositionableWaveDisplay (AudioFilePlayer* sourceToBeUsed,
                                                                  MultipleAudioThumbnailCache* cacheToUse,
                                                                  ColouredAudioThumbnail* thumbnailToUse)
:	filePlayer          (sourceToBeUsed),
	currentSampleRate   (44100.0),
    oneOverSampleRate   (1.0),
    thumbnailCache      (cacheToUse, (cacheToUse == nullptr) ? true : false),
    thumbnailView       (thumbnailToUse, (thumbnailToUse == nullptr) ? true : false),
	zoomFactor          (1.0f),
    isInitialised       (false),
    lastTimeDrawn       (0.0),
    resolution          (3.0)
{
    setOpaque(true);
    waveformImage = Image (Image::RGB, 1, 1, false);

	formatManager = filePlayer->getAudioFormatManager();
	
	// instansiate the cache and the thumbnail
	if (thumbnailCache == nullptr)
    {
        OptionalScopedPointer<MultipleAudioThumbnailCache> newCache (new MultipleAudioThumbnailCache (3), true);
		thumbnailCache = newCache;
	}
    if (thumbnailToUse == nullptr)
    {
        OptionalScopedPointer<ColouredAudioThumbnail> newThumbnail (new ColouredAudioThumbnail (512, *formatManager, *thumbnailCache),
                                                                    true);
        thumbnailView = newThumbnail;
    }
	thumbnailView->addChangeListener (this);
    
	// register with the file player to recieve update messages
	filePlayer->addListener (this);
}

ColouredPositionableWaveDisplay::~ColouredPositionableWaveDisplay()
{
	stopTimer (waveformUpdated);
}

//====================================================================================
void ColouredPositionableWaveDisplay::setResolution (double newResolution)
{
    resolution = newResolution;
    
    waveformImage.clear (waveformImage.getBounds(), Colours::black);
    displayImage = Image (Image::RGB, jmax (1, roundToInt (getWidth() * resolution)), getHeight(), true);
    lastTimeDrawn = 0.0;
    refreshWaveform();
    triggerAsyncUpdate();    
}

//====================================================================================
void ColouredPositionableWaveDisplay::resized()
{
    if (! isInitialised)
    {
        timerCallback (waveformResizing);
        isInitialised = true;
    }
    else
    {
        startTimer (waveformResizing, 500);
    }
}

void ColouredPositionableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();

	g.drawImage (waveformImage, 0, 0, w, h,
                 0, 0, waveformImage.getWidth(), waveformImage.getHeight());
	
	g.setColour (Colours::black);	
	g.drawVerticalLine (transportLineXCoord.getCurrent() - 1, 0, h);
	g.drawVerticalLine (transportLineXCoord.getCurrent() + 1, 0, h);

	g.setColour (Colours::white);
	g.drawVerticalLine (transportLineXCoord.getCurrent(), 0, h);
}

//====================================================================================
void ColouredPositionableWaveDisplay::timerCallback (const int timerId)
{
	if (timerId == waveformUpdated)
	{
		const int w = getWidth();
		const int h = getHeight();

		transportLineXCoord = w * oneOverFileLength * filePlayer->getCurrentPosition();
		
		// if the line has moved repaint the old and new positions of it
		if (! transportLineXCoord.areEqual())
		{
			repaint (transportLineXCoord.getPrevious() - 2, 0, 5, h);
			repaint (transportLineXCoord.getCurrent() - 2, 0, 5, h);
		}
	}
	else if (timerId == waveformResizing)
	{
        displayImage = Image (Image::RGB, jmax (1, roundToInt (getWidth() * resolution)), getHeight(), true);
		waveformImage = Image (Image::RGB, jmax (1, getWidth()), getHeight(), true);
        waveformImage.clear (waveformImage.getBounds(), Colours::black);
        lastTimeDrawn = 0.0;
		refreshWaveform();
		stopTimer (waveformResizing);
	}
}

void ColouredPositionableWaveDisplay::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == thumbnailView)
        refreshWaveform();	
}

void ColouredPositionableWaveDisplay::handleAsyncUpdate()
{
    repaint();
}

void ColouredPositionableWaveDisplay::fileChanged (AudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;

        if (currentSampleRate > 0.0)
        {
            oneOverSampleRate = 1.0 / currentSampleRate;
            fileLength = filePlayer->getTotalLength() * oneOverSampleRate;
            oneOverFileLength = 1.0 / fileLength;
            
            waveformImage.clear (waveformImage.getBounds(), Colours::black);
            lastTimeDrawn = 0.0;
            triggerAsyncUpdate();

            File newFile (filePlayer->getFile());
            if (newFile.existsAsFile()) 
            {
                FileInputSource* fileInputSource = new FileInputSource (newFile);
                thumbnailView->setSource (fileInputSource);
            }
            else 
            {
                thumbnailView->setSource (nullptr);
            }
            
            startTimer (waveformUpdated, 40);
        }
	}
}

//==============================================================================
void ColouredPositionableWaveDisplay::mouseDown(const MouseEvent &e)
{
	currentXScale = fileLength / getWidth();
	currentMouseX = e.x;
	
	setMouseCursor (MouseCursor::IBeamCursor);
	
	double position = currentXScale * currentMouseX;
	filePlayer->setPosition (position);
    
	triggerAsyncUpdate();		
}

void ColouredPositionableWaveDisplay::mouseUp(const MouseEvent &e)
{
	setMouseCursor (MouseCursor::NormalCursor);
}

void ColouredPositionableWaveDisplay::mouseDrag(const MouseEvent &e)
{
	currentMouseX = e.x;
	
	double position = currentXScale * currentMouseX;
	filePlayer->setPosition (position);
}

//==============================================================================	
void ColouredPositionableWaveDisplay::refreshWaveform()
{
	if (waveformImage.isValid() && ! thumbnailView->isFullyLoaded())
	{
        Graphics gTemp (displayImage);
        
        const double endTime = thumbnailView->getNumSamplesFinished() * oneOverSampleRate;
        lastTimeDrawn -= (endTime - lastTimeDrawn) * 0.5; // overlap by 0.5
        const double startPixelX = (lastTimeDrawn * oneOverFileLength * waveformImage.getWidth());
        const double numPixels = ((endTime - lastTimeDrawn) * oneOverFileLength * waveformImage.getWidth());
        
        Rectangle<int> rectangleToDraw (roundToInt (startPixelX * resolution), 0, 
                                        roundToInt (numPixels * resolution), displayImage.getHeight());
        
        gTemp.setColour (Colours::black);
        gTemp.fillRect (rectangleToDraw);
        
        thumbnailView->drawColouredChannel (gTemp, rectangleToDraw,
                                            lastTimeDrawn, endTime,
                                            0, 1.0f);
        lastTimeDrawn = endTime;
        
		Graphics g (waveformImage);
        g.drawImage (displayImage,
                     roundToInt (startPixelX), 0, roundToInt (numPixels), waveformImage.getHeight(),
                     roundToInt (startPixelX * resolution), 0, roundToInt (numPixels * resolution), displayImage.getHeight());

        repaint (Rectangle<int> (roundToInt (startPixelX), 0, roundToInt (numPixels), waveformImage.getHeight()));
	}
    else if (thumbnailView->isFullyLoaded())
    {
        // hack to redraw the whole wave once it has loaded until
        // I can find out where the gaps are comping from
        Graphics gTemp (displayImage);
        thumbnailView->drawColouredChannel (gTemp, displayImage.getBounds(),
                                            0.0, filePlayer->getLengthInSeconds(),
                                            0, 1.0f);

		Graphics g (waveformImage);
        g.drawImage (displayImage,
                     0, 0, waveformImage.getWidth(), waveformImage.getHeight(),
                     0, 0, displayImage.getWidth(), displayImage.getHeight());

        lastTimeDrawn = filePlayer->getLengthInSeconds();
        repaint (waveformImage.getBounds());
    }
	else
    {
        Graphics g (waveformImage);
		waveformImage.clear (waveformImage.getBounds(), Colours::black);
    }
}

//==============================================================================

END_JUCE_NAMESPACE
