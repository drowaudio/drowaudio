/*
  ==============================================================================

    dRowAudio_ColouredPositionableWaveDisplay.cpp
    Created: 9 Jul 2011 7:35:03pm
    Author:  David Rowland

  ==============================================================================
*/

#include "dRowAudio_ColouredPositionableWaveDisplay.h"

ColouredPositionableWaveDisplay::ColouredPositionableWaveDisplay (FilteringAudioFilePlayer *sourceToBeUsed,
                                                                  MultipleAudioThumbnailCache *cacheToUse,
                                                                  ColouredAudioThumbnail *thumbnailToUse)
:	filePlayer (sourceToBeUsed),
	currentSampleRate (44100.0),
    thumbnailCache (cacheToUse, (cacheToUse == nullptr) ? true : false),
    thumbnailView (thumbnailToUse, (thumbnailToUse == nullptr) ? true : false),
	zoomFactor (1.0f),
    isInitialised (false),
	isMouseDown (false),
	interestedInDrag (false)
{
    waveformImage = Image(Image::RGB, 1, 1, false);

	formatManager = filePlayer->getAudioFormatManager();
	
	// instansiate the cache and the thumbnail
	// instansiate the cache and the thumbnail
	if (thumbnailCache == nullptr)
    {
        OptionalScopedPointer<MultipleAudioThumbnailCache> newCache(new MultipleAudioThumbnailCache(3), true);
		thumbnailCache = newCache;
	}
    if (thumbnailToUse == nullptr)
    {
        OptionalScopedPointer<ColouredAudioThumbnail> newThumbnail(new ColouredAudioThumbnail(512, *formatManager, *thumbnailCache),
                                                                   true);
        thumbnailView = newThumbnail;
    }
	thumbnailView->addChangeListener(this);
    
	// register with the file player to recieve update messages
	filePlayer->addListener(this);
}

ColouredPositionableWaveDisplay::~ColouredPositionableWaveDisplay()
{
	stopTimer(waveformUpdated);
}

//====================================================================================
void ColouredPositionableWaveDisplay::resized()
{
    if (! isInitialised)
    {
        timerCallback(waveformResizing);
        isInitialised = true;
    }
    else
    {
        startTimer(waveformResizing, 500);
    }
}

void ColouredPositionableWaveDisplay::paint(Graphics &g)
{
	const int w = getWidth();
	const int h = getHeight();

	g.drawImage(waveformImage, 0, 0, w, h,
				0, 0, waveformImage.getWidth(), waveformImage.getHeight());
	
	g.setColour(Colours::black);	
	g.drawVerticalLine(transportLineXCoord.getCurrent() - 1, 0, h);
	g.drawVerticalLine(transportLineXCoord.getCurrent() + 1, 0, h);

	g.setColour(Colours::white);
	g.drawVerticalLine(transportLineXCoord.getCurrent(), 0, h);
	
	if (interestedInDrag) {
		g.setColour(Colours::darkorange);
		g.drawRect(0, 0, w, h, 2);
	}
}
//====================================================================================
void ColouredPositionableWaveDisplay::timerCallback(const int timerId)
{
	if (timerId == waveformUpdated)
	{
		const int w = getWidth();
		const int h = getHeight();

		transportLineXCoord = w * oneOverFileLength * filePlayer->getAudioTransportSource()->getCurrentPosition();
		
		// if the line has moved repaint the old and new positions of it
		if (!transportLineXCoord.areEqual())
		{
			repaint(transportLineXCoord.getPrevious() - 2, 0, 5, h);
			repaint(transportLineXCoord.getCurrent() - 2, 0, 5, h);
		}
	}
	else if (timerId == waveformResizing)
	{
        tempImage = Image(Image::RGB, getWidth() * 3, getHeight(), true);
		waveformImage = Image(Image::RGB, getWidth(), getHeight(), true);
		refreshWaveform();
		stopTimer(waveformResizing);
	}

}

void ColouredPositionableWaveDisplay::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == thumbnailView)
    {
        if (thumbnailView->isFullyLoaded()) {
            DBG("ColouredPositionableWaveDisplay finished");
        }
   
        refreshWaveform();	
    }
}

void ColouredPositionableWaveDisplay::handleAsyncUpdate()
{
    repaint();
}

void ColouredPositionableWaveDisplay::fileChanged (FilteringAudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		currentSampleRate = filePlayer->getAudioFormatReaderSource()->getAudioFormatReader()->sampleRate;
		fileLength = filePlayer->getAudioTransportSource()->getTotalLength() / currentSampleRate;
		oneOverFileLength = 1.0 / fileLength;
		
		File newFile(filePlayer->getFilePath());
		if (newFile.existsAsFile()) {
			FileInputSource* fileInputSource = new FileInputSource (newFile);
			thumbnailView->setSource(fileInputSource);
		}
		else {
			thumbnailView->setSource(0);
		}
		
		//startTimer(waveformLoading, 25);
		startTimer(waveformUpdated, 40);		
	}
}

//====================================================================================
void ColouredPositionableWaveDisplay::setZoomFactor (float newZoomFactor)
{
	const int w = getWidth();

	zoomFactor = newZoomFactor;
	currentXScale = ( (zoomFactor)*fileLength ) / w;
	
	triggerAsyncUpdate();
}

//==============================================================================
void ColouredPositionableWaveDisplay::mouseDown(const MouseEvent &e)
{
	// update scale
	const int w = getWidth();

	currentXScale = ( (zoomFactor)*fileLength ) / w;
	
	currentMouseX = e.x;
	isMouseDown = true;
	
	setMouseCursor(MouseCursor::IBeamCursor);
	
	double position = currentXScale * currentMouseX;
	filePlayer->getAudioTransportSource()->setPosition(position);
	triggerAsyncUpdate();		
}

void ColouredPositionableWaveDisplay::mouseUp(const MouseEvent &e)
{
	isMouseDown = false;
	
	setMouseCursor(MouseCursor::NormalCursor);
}

void ColouredPositionableWaveDisplay::mouseDrag(const MouseEvent &e)
{
	currentMouseX = e.x;
	
	double position = currentXScale * currentMouseX;
	filePlayer->getAudioTransportSource()->setPosition(position);
}
//==============================================================================
bool ColouredPositionableWaveDisplay::isInterestedInFileDrag (const StringArray &files)
{
	File droppedFile(files[0]);
	if (matchesAudioWildcard(droppedFile.getFileExtension(), formatManager->getWildcardForAllFormats(), true))
		return true;
	else
		return false;
}
void ColouredPositionableWaveDisplay::fileDragEnter (const StringArray &files, int x, int y)
{
	interestedInDrag = true;
	setMouseCursor(MouseCursor::CopyingCursor);
	
	triggerAsyncUpdate();
}
void ColouredPositionableWaveDisplay::fileDragExit (const StringArray &files)
{
	interestedInDrag = false;
	setMouseCursor(MouseCursor::NormalCursor);	

	triggerAsyncUpdate();
}
void ColouredPositionableWaveDisplay::filesDropped (const StringArray &files, int x, int y)
{
	interestedInDrag = false;
	filePlayer->setFile(files[0]);
	setMouseCursor(MouseCursor::NormalCursor);

	triggerAsyncUpdate();
}

//==============================================================================
bool ColouredPositionableWaveDisplay::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
	if (dragSourceDetails.description.toString().startsWith("<ITEMS>")) {
		return true;
	}
	
	return false;	
}

void ColouredPositionableWaveDisplay::itemDragEnter (const SourceDetails& dragSourceDetails)
{
	interestedInDrag = true;
	triggerAsyncUpdate();
}

void ColouredPositionableWaveDisplay::itemDragExit (const SourceDetails& dragSourceDetails)
{
	interestedInDrag = false;
	triggerAsyncUpdate();
}

void ColouredPositionableWaveDisplay::itemDropped (const SourceDetails& dragSourceDetails)
{
	ScopedPointer<XmlElement> newTracks (XmlDocument::parse(dragSourceDetails.description.toString()));
	
	if (newTracks->getNumChildElements() > 0) {
		File newFile(newTracks->getChildElement(0)->getStringAttribute("Location"));
		
		if (newFile.existsAsFile()) {
			filePlayer->setLibraryEntry(ValueTree::fromXml(*newTracks->getChildElement(0)));
			filePlayer->setFile(newFile.getFullPathName());
		}
	}
	
	interestedInDrag = false;
	triggerAsyncUpdate();
}


//==============================================================================	
void ColouredPositionableWaveDisplay::refreshWaveform()
{
	if(waveformImage.isValid())
	{
        Graphics gTemp(tempImage);
        gTemp.fillAll(Colours::black);
        
        if (filePlayer->getFileName().isNotEmpty())
		{
			thumbnailView->drawColouredChannel(gTemp, Rectangle<int> (0, 0, tempImage.getWidth(), tempImage.getHeight()),
                                               0.0, fileLength,
                                               0, 1.0f);
		}
        
        
		Graphics g(waveformImage);
		waveformImage.clear(waveformImage.getBounds(), Colours::black);
        g.drawImage(tempImage,
                    0, 0, waveformImage.getWidth(), waveformImage.getHeight(),
                    0, 0, tempImage.getWidth(), tempImage.getHeight());
		
		triggerAsyncUpdate();
	}	
}

//==============================================================================