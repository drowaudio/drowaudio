/*
 *  DraggableDisplay.h
 *  Decks
 *
 *  Created by David Rowland on 05/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DRAGGABLEDISPLAY__H_
#define _DRAGGABLEDISPLAY__H_

#include <dRowAudio/dRowAudio.h>
#include "../../DecksLookAndFeel.h"
#include "../../DecksColours.h"
#include "../../../main/Settings.h"
#include "../../../main/DeckManager.h"

class DraggableDisplay :	public Component,
							public SliderListener,
							public ValueTree::Listener
{
public:
	DraggableDisplay();
	
	~DraggableDisplay();
	
	//================================================================
	void resized();
	
	void paint (Graphics &g);
	
	//================================================================
	void sliderValueChanged(Slider *slider);
	
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const var::identifier  &property);
	
	void valueTreeChildrenChanged (ValueTree &treeWhoseChildHasChanged);
	
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged);
	
	//================================================================
private:

	int noDecks;
	DeckManager *settings;

	Slider *zoomSlider, *playheadPosSlider;
	ScopedPointer<AudioThumbnailCache> thumbnailCache; 
	OwnedArray <DraggableWaveDisplay> draggableWaveDisplays;
	
};

#endif //_DRAGGABLEDISPLAY__H_