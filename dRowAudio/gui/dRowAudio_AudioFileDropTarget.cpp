/*
  ==============================================================================

    dRowAudio_AudioFileDropTarget.cpp
    Created: 9 Jul 2011 7:35:03pm
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

AudioFileDropTarget::AudioFileDropTarget (AudioFilePlayer* audioFilePlayerToControl,
                                          Component* componentToAttachTo)
    :  audioFilePlayer (audioFilePlayerToControl),
       attachedComponent (componentToAttachTo),
       dragTested (false),
       interestedInDrag (false),
       bezelColour (Colours::orange)
{
    if (attachedComponent != nullptr)
    {
        attachedComponent->addComponentListener (this);
        attachedComponent->addMouseListener (this, false);
    }
}

AudioFileDropTarget::~AudioFileDropTarget()
{
    if (attachedComponent != nullptr)
    {
        attachedComponent->removeComponentListener (this);
        attachedComponent->removeMouseListener (this);
    }
}

void AudioFileDropTarget::setBezelColour (Colour& newColour)
{
    bezelColour = newColour;
    repaint();
}

//==============================================================================
void AudioFileDropTarget::paint (Graphics& g)
{
    if (interestedInDrag) 
    {
        g.setColour (bezelColour);
        g.drawRect (0, 0, getWidth(), getHeight(), 2);
    }
}

bool AudioFileDropTarget::hitTest (int x, int y)
{
    if (! dragTested || interestedInDrag)
        return true;

    return false;
}

void AudioFileDropTarget::mouseEnter (const MouseEvent& e)
{
    dragTested = true;

    repaint();
}

void AudioFileDropTarget::mouseExit (const MouseEvent& e)
{
    dragTested = false;
    
    repaint();
}

//==============================================================================
void AudioFileDropTarget::componentMovedOrResized (Component& component,
                                                   bool wasMoved,
                                                   bool wasResized)
{
    if (attachedComponent != nullptr)
        setBounds (attachedComponent->getBounds());
}

//==============================================================================
bool AudioFileDropTarget::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    ReferenceCountedValueTree::Ptr libraryTree (dynamic_cast<ReferenceCountedValueTree*> (dragSourceDetails.description[0].getObject()));
    
    if (libraryTree != nullptr && libraryTree->getValueTree().hasType (Columns::libraryItemIdentifier))
    {
        interestedInDrag = true;
        setMouseCursor (MouseCursor::CopyingCursor);

        repaint();

        return true;
    }
    
    return false;	
}

void AudioFileDropTarget::itemDragExit (const SourceDetails& dragSourceDetails)
{
    if (interestedInDrag)
    {
        interestedInDrag = false;
        setMouseCursor (MouseCursor::NormalCursor);	
        
        repaint();
    }
}

void AudioFileDropTarget::itemDropped (const SourceDetails& dragSourceDetails)
{
    if (interestedInDrag && dragSourceDetails.description.isArray()) 
    {
        ReferenceCountedValueTree::Ptr childTree (dynamic_cast<ReferenceCountedValueTree*> (dragSourceDetails.description[0].getObject()));
        if (childTree != nullptr) 
        {
            ValueTree itemTree (childTree->getValueTree());
            File newFile (itemTree.getProperty (Columns::columnNames[Columns::Location]));
            
            if (newFile.existsAsFile()) 
            {
                audioFilePlayer->setLibraryEntry (itemTree);
                audioFilePlayer->setFile (newFile.getFullPathName());
            }
        }
    }
    
    interestedInDrag = false;
    repaint();
}

//==============================================================================
bool AudioFileDropTarget::isInterestedInFileDrag (const StringArray &files)
{
    if (matchesAudioWildcard (File (files[0]).getFileExtension(), 
                              audioFilePlayer->getAudioFormatManager()->getWildcardForAllFormats(), true))
    {
        interestedInDrag = true;
        setMouseCursor (MouseCursor::CopyingCursor);
        
        repaint();

        return true;
    }

    return false;
}

void AudioFileDropTarget::fileDragExit (const StringArray &files)
{
    if (interestedInDrag)
    {
        interestedInDrag = false;
        setMouseCursor (MouseCursor::NormalCursor);	
        
        repaint();
    }
}

void AudioFileDropTarget::filesDropped (const StringArray &files, int x, int y)
{
    if (interestedInDrag)
    {
        interestedInDrag = false;
        audioFilePlayer->setFile (files[0]);
        setMouseCursor (MouseCursor::NormalCursor);
        
        repaint();
    }
}

//==============================================================================

END_JUCE_NAMESPACE
