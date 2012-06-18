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



AudioThumbnailImage::AudioThumbnailImage (AudioFilePlayer* sourceToBeUsed,
                                          TimeSliceThread& backgroundThread_,
                                          AudioThumbnailCache* cacheToUse,
                                          AudioThumbnailBase* thumbnailToUse,
                                          int sourceSamplesPerThumbnailSample_)
    : filePlayer            (sourceToBeUsed),
	  currentSampleRate     (44100.0),
      oneOverSampleRate     (1.0),
      backgroundThread      (backgroundThread_),
      audioThumbnailCache   (cacheToUse, (cacheToUse == nullptr) ? true : false),
      audioThumbnail        (thumbnailToUse, (thumbnailToUse == nullptr) ? true : false),
      sourceSamplesPerThumbnailSample (sourceSamplesPerThumbnailSample_),
      backgroundColour      (Colours::black),
      waveformColour        (Colours::green),
      lastTimeDrawn         (0.0),
      resolution            (3.0),
      renderComplete        (true)
{
    jassert (filePlayer != nullptr);
    
    waveformImage = Image (Image::RGB, 1, 1, false);

	// instansiate the cache and the thumbnail if needed
	if (audioThumbnailCache == nullptr)
    {
        OptionalScopedPointer<AudioThumbnailCache> newCache (new AudioThumbnailCache (3), true);
		audioThumbnailCache = newCache;
	}
    if (thumbnailToUse == nullptr)
    {
        OptionalScopedPointer<AudioThumbnailBase> newThumbnail (new AudioThumbnail (sourceSamplesPerThumbnailSample,
                                                                                *filePlayer->getAudioFormatManager(),
                                                                                *audioThumbnailCache),
                                                            true);
        newThumbnail->clear();
        audioThumbnail = newThumbnail;
    }
    
	// register with the file player to recieve update messages
	filePlayer->addListener (this);
}

AudioThumbnailImage::~AudioThumbnailImage()
{
	filePlayer->removeListener (this);
    
    for (int i = 0; i < backgroundThread.getNumClients(); i++) 
    {
        if (backgroundThread.getClient (i) == this) 
        {
            backgroundThread.removeTimeSliceClient (this);
        }
    }
	
    stopTimer();
}

void AudioThumbnailImage::setBackgroundColour (Colour newBackgroundColour)
{
    backgroundColour = newBackgroundColour;
    setResolution (resolution);
}

void AudioThumbnailImage::setWaveformColour (Colour newWaveformColour)
{
    waveformColour = newWaveformColour;
    setResolution (resolution);
}

//====================================================================================
const Image AudioThumbnailImage::getImageAtTime (double startTime, double duration)
{
    const int startPixel = roundToInt (startTime * oneOverFileLength * waveformImage.getWidth());
    const int numPixels = roundToInt (duration * oneOverFileLength * waveformImage.getWidth());
    
    return waveformImage.getClippedImage (juce::Rectangle<int> (startPixel, 0, numPixels, waveformImage.getHeight()));
}

void AudioThumbnailImage::setResolution (double newResolution)
{
    resolution = newResolution;
    
    triggerWaveformRefresh();
}

//====================================================================================
void AudioThumbnailImage::timerCallback ()
{
    const ScopedReadLock sl (imageLock);

    if (! renderComplete)
    {
        listeners.call (&Listener::imageUpdated, this);
    }
    else
    {
        listeners.call (&Listener::imageUpdated, this);
        listeners.call (&Listener::imageFinished, this);
        stopTimer();
    }
}

int AudioThumbnailImage::useTimeSlice()
{
    refreshWaveform();
    
    return 50;
}

void AudioThumbnailImage::fileChanged (AudioFilePlayer *player)
{
	if (player == filePlayer)
	{
        if (filePlayer->getAudioFormatReaderSource() != nullptr)
        {
            currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;

            if (currentSampleRate > 0.0)
            {
                oneOverSampleRate = 1.0 / currentSampleRate;
                fileLength = filePlayer->getLengthInSeconds();
                oneOverFileLength = 1.0 / fileLength;
                
                const ScopedWriteLock sl (imageLock);

                const int imageWidth = roundToInt (filePlayer->getTotalLength() / sourceSamplesPerThumbnailSample);
                waveformImage = Image (Image::RGB, jmax (1, imageWidth), 100, true);

                waveformImage.clear (waveformImage.getBounds(), backgroundColour);
                
                File newFile (filePlayer->getFile());
                if (newFile.existsAsFile()) 
                {
                    FileInputSource* fileInputSource = new FileInputSource (newFile);
                    audioThumbnail->setSource (fileInputSource);
                    renderComplete = false;
                }
                else if (filePlayer->sourceIsMemoryBlock())
                {
                    MemoryInputSource* inputSource = new MemoryInputSource (filePlayer->getInputStream());
                    audioThumbnail->setSource (inputSource);
                    renderComplete = false;
                }
                else 
                {
                    audioThumbnail->setSource (nullptr);
                    renderComplete = true;
                }
            }
        }
        else 
        {
            audioThumbnail->setSource (nullptr);
            renderComplete = true;
        }

        triggerWaveformRefresh();
    }
}

//==============================================================================
void AudioThumbnailImage::addListener (AudioThumbnailImage::Listener* const listener)
{
    listeners.add (listener);
}

void AudioThumbnailImage::removeListener (AudioThumbnailImage::Listener* const listener)
{
    listeners.remove (listener);
}

//==============================================================================	
void AudioThumbnailImage::triggerWaveformRefresh()
{
    {
        const ScopedWriteLock sl (imageLock);

        lastTimeDrawn = 0.0;
        waveformImage.clear (waveformImage.getBounds(), backgroundColour);
        renderComplete = false;
    }
    
    listeners.call (&Listener::imageChanged, this);
    
    backgroundThread.addTimeSliceClient (this);
    if (! backgroundThread.isThreadRunning())
        backgroundThread.startThread (4);
    
    startTimer (100);
}

void AudioThumbnailImage::refreshWaveform()
{
	if (audioThumbnail->getNumSamplesFinished() > 0)
	{
        const double endTime = audioThumbnail->getNumSamplesFinished() * oneOverSampleRate;
        double timeToDraw = endTime - lastTimeDrawn;
        if (lastTimeDrawn > timeToDraw)
        {
            lastTimeDrawn -= timeToDraw * 0.1; // overlap by 0.1
            timeToDraw = endTime - lastTimeDrawn;
        }

        //        const ScopedReadLock sl (imageLock);
        imageLock.enterRead();
        const int startPixelX = roundToInt (lastTimeDrawn * oneOverFileLength * waveformImage.getWidth());
        const int numPixels = roundToInt (timeToDraw * oneOverFileLength * waveformImage.getWidth());
        const int numTempPixels = roundToInt (numPixels * resolution);
        imageLock.exitRead();
        
        if (numTempPixels > 0)
        {
            if (tempSectionImage.getWidth() < numTempPixels)
            {
                tempSectionImage = Image (Image::RGB, 
                                          numTempPixels, waveformImage.getHeight(), 
                                          false);
            }
            
            juce::Rectangle<int> rectangleToDraw (0, 0, numTempPixels, waveformImage.getHeight());
            
            Graphics gTemp (tempSectionImage);
            tempSectionImage.clear(tempSectionImage.getBounds(), backgroundColour);
            gTemp.setColour (waveformColour);
            audioThumbnail->drawChannel (gTemp, rectangleToDraw,
                                         lastTimeDrawn, endTime,
                                         0, 1.0f);
            lastTimeDrawn = endTime;
            
            const ScopedWriteLock sl (imageLock);
            
            Graphics g (waveformImage);
            g.drawImage (tempSectionImage,
                         startPixelX, 0, numPixels, waveformImage.getHeight(),
                         0, 0, numTempPixels, tempSectionImage.getHeight());
        }
	}

    if (renderComplete)
        backgroundThread.removeTimeSliceClient (this);

    if (audioThumbnail->isFullyLoaded())
        renderComplete = true;
}


