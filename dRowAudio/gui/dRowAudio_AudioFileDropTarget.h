/*
  ==============================================================================

    dRowAudio_AudioFileDropTarget.h
    Created: 9 Jul 2011 7:35:03pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_AUDIOFILEDROPTARGET_H__
#define __DROWAUDIO_AUDIOFILEDROPTARGET_H__

#include "../audio/dRowAudio_AudioFilePlayer.h"

/** A Component that acts as a drag and drop target for audio files and
    MusicLibraryTable drag sources. This will draw a coloured bezel if it can
    read the drag source provided.
 */
class AudioFileDropTarget : public Component,
                            public ComponentListener,
                            public DragAndDropTarget,
                            public FileDragAndDropTarget
{
public:
    /** Creates an AudioFileDropTarget, which controls an AudioFilePlayer.
        
        If you supply a component to attach itself to, the AudioFileDropTarget
        will automatically position itself around that component and pass any
        mouse events which are not drags onto it.
     
        @see AudioFilePlayer
     */
    AudioFileDropTarget (AudioFilePlayerExt* audioFilePlayerToControl,
                         Component* componentToAttachTo = nullptr);
    
    /** Destructor.
     */
    ~AudioFileDropTarget();
    
    /** Sets the colour of the bezel to be drawn.
     */
    void setBezelColour (Colour& newColour);

    /** Retruns the current bezel colour being used.
     */
    const Colour getBezelColour()            {   return bezelColour; }

    //==============================================================================
    /** @internal */
    void paint (Graphics& g);
    
    /** @internal */
    bool hitTest (int x, int y);

    /** @internal */
    void mouseEnter (const MouseEvent& e);

    /** @internal */
    void mouseExit (const MouseEvent& e);
    
    //==============================================================================
    /** @internal */
    void componentMovedOrResized (Component& component,
                                  bool wasMoved,
                                  bool wasResized);

    //==============================================================================
    /** @internal */
    bool isInterestedInDragSource (const SourceDetails& dragSourceDetails);
    /** @internal */
    void itemDragExit (const SourceDetails& dragSourceDetails);
    /** @internal */
    void itemDropped (const SourceDetails& dragSourceDetails);
    
    //==============================================================================
    /** @internal */
    bool isInterestedInFileDrag (const StringArray& files);
    /** @internal */
    void fileDragExit (const StringArray& files);
    /** @internal */
    void filesDropped (const StringArray& files, int x, int y);

    //==============================================================================

private:
    
    AudioFilePlayerExt* audioFilePlayer;
    SafePointer<Component> attachedComponent;
    bool dragTested, interestedInDrag;
    Colour bezelColour;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFileDropTarget);
};

#endif  // __DROWAUDIO_AUDIOFILEDROPTARGET_H__
