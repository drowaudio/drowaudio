/*
  ==============================================================================

    dRowAudio_ColouredPositionableWaveDisplay.cpp
    Created: 9 Jul 2011 7:35:03pm
    Author:  David Rowland

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
	isMouseDown         (false),
    lastTimeDrawn       (0.0),
    ratio               (3.0)
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
void ColouredPositionableWaveDisplay::setRatio (double newRatio)
{
    ratio = newRatio;
    
    waveformImage.clear (waveformImage.getBounds(), Colours::black);
    displayImage = Image (Image::RGB, jmax (1, roundToInt (getWidth() * ratio)), getHeight(), true);
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
	
//	if (interestedInDrag)
//    {
//		g.setColour (Colours::darkorange);
//		g.drawRect (0, 0, w, h, 2);
//	}
}
//====================================================================================
void ColouredPositionableWaveDisplay::timerCallback(const int timerId)
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
        displayImage = Image (Image::RGB, jmax (1, roundToInt (getWidth() * ratio)), getHeight(), true);
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
    {
        if (thumbnailView->isFullyLoaded()) {
            DBG("ColouredPositionableWaveDisplay fully loaded");
        }
   
        refreshWaveform();	
    }
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

            File newFile (filePlayer->getPath());
            if (newFile.existsAsFile()) {
                FileInputSource* fileInputSource = new FileInputSource (newFile);
                thumbnailView->setSource (fileInputSource);
            }
            else {
                thumbnailView->setSource (nullptr);
            }
            
            startTimer (waveformUpdated, 40);
        }
	}
}

//==============================================================================
void ColouredPositionableWaveDisplay::mouseDown(const MouseEvent &e)
{
	// update scale
	currentXScale = fileLength / getWidth();
	
	currentMouseX = e.x;
	isMouseDown = true;
	
	setMouseCursor (MouseCursor::IBeamCursor);
	
	double position = currentXScale * currentMouseX;
	filePlayer->setPosition (position);
    
	triggerAsyncUpdate();		
}

void ColouredPositionableWaveDisplay::mouseUp(const MouseEvent &e)
{
	isMouseDown = false;
	
	setMouseCursor (MouseCursor::NormalCursor);
}

void ColouredPositionableWaveDisplay::mouseDrag(const MouseEvent &e)
{
	currentMouseX = e.x;
	
	double position = currentXScale * currentMouseX;
	filePlayer->setPosition (position);
}
//==============================================================================
/*bool ColouredPositionableWaveDisplay::isInterestedInFileDrag (const StringArray &files)
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
//	if (dragSourceDetails.description.toString().startsWith("<ITEMS>")) {
//		return true;
//	}

//    ValueTree* libraryTree = dynamic_cast<ValueTree*> ((ValueTree*)int(dragSourceDetails.description[0]));
  
    ReferenceCountedValueTree::Ptr libraryTree (dynamic_cast<ReferenceCountedValueTree*> (dragSourceDetails.description[0].getObject()));

    if (libraryTree != nullptr && libraryTree->getValueTree().hasType (Columns::libraryItemIdentifier))
    {
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
    if (dragSourceDetails.description.isArray()) 
    {
//        ValueTree* libraryTree = dynamic_cast<ValueTree*> ((ValueTree*)(dragSourceDetails.description[0]));
//        
//        if (libraryTree != nullptr && libraryTree->isValid())
//        {
//            DBG("dropped tree is valid");
//            ValueTree itemTree(libraryTree->getChildWithProperty(Columns::columnNames[Columns::ID], dragSourceDetails.description[1]));
//            File newFile(itemTree.getProperty(Columns::columnNames[Columns::Location]));

        ReferenceCountedValueTree::Ptr childTree (dynamic_cast<ReferenceCountedValueTree*> (dragSourceDetails.description[0].getObject()));
        if (childTree != nullptr) 
        {
            ValueTree itemTree (childTree->getValueTree());
            File newFile (itemTree.getProperty(Columns::columnNames[Columns::Location]));
            
            if (newFile.existsAsFile()) 
            {
                filePlayer->setLibraryEntry(itemTree);
                filePlayer->setFile(newFile.getFullPathName());
            }
        }
    }
    
//	ScopedPointer<XmlElement> newTracks (XmlDocument::parse(dragSourceDetails.description.toString()));
//	
//	if (newTracks->getNumChildElements() > 0) {
//		File newFile(newTracks->getChildElement(0)->getStringAttribute("Location"));
//		
//		if (newFile.existsAsFile()) {
//			filePlayer->setLibraryEntry(ValueTree::fromXml(*newTracks->getChildElement(0)));
//			filePlayer->setFile(newFile.getFullPathName());
//		}
//	}
	
	interestedInDrag = false;
	triggerAsyncUpdate();
}*/

//==============================================================================	
void ColouredPositionableWaveDisplay::refreshWaveform()
{
	if (waveformImage.isValid() && filePlayer->getPath().isNotEmpty())
	{
        Graphics gTemp (displayImage);
        
        const double endTime = thumbnailView->getNumSamplesFinished() * oneOverSampleRate;
        const double startPixelX = (lastTimeDrawn * oneOverFileLength * waveformImage.getWidth());
        const double numPixels = ((endTime - lastTimeDrawn) * oneOverFileLength * waveformImage.getWidth());

        Rectangle<int> rectangleToDraw (roundToInt (startPixelX * ratio), 0, 
                                        roundToInt (numPixels * ratio), displayImage.getHeight());
        
        gTemp.setColour (Colours::black);
        gTemp.fillRect (rectangleToDraw);
        
        thumbnailView->drawColouredChannel (gTemp, rectangleToDraw,
                                            lastTimeDrawn, endTime,
                                            0, 1.0f);
        lastTimeDrawn = endTime;
        
		Graphics g (waveformImage);
        g.drawImage (displayImage,
                     roundToInt (startPixelX), 0, roundToInt (numPixels), waveformImage.getHeight(),
                     roundToInt (startPixelX * ratio), 0, roundToInt (numPixels * ratio), displayImage.getHeight());
		
        repaint (Rectangle<int> (roundToInt (startPixelX), 0, roundToInt (numPixels), waveformImage.getHeight()));
	}
	else
    {
        Graphics g (waveformImage);
		waveformImage.clear (waveformImage.getBounds(), Colours::black);
    }
}

//==============================================================================

END_JUCE_NAMESPACE
