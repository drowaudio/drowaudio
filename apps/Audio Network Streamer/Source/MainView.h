/*
  ==============================================================================

    MainView.h
    Created: 12 Jun 2011 6:06:42am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MAINVIEW_H_E96C7D0F__
#define __MAINVIEW_H_E96C7D0F__


#include "../JuceLibraryCode/JuceHeader.h"
#include "Settings.h"
#include "StatusComp.h"

//==============================================================================
class MainView  :	public Component,
					public ValueTree::Listener
{
public:
    //==============================================================================
    MainView();
	
    ~MainView();
	
	void resized();
	
	//==============================================================================
	void valueTreePropertyChanged (ValueTree& treeWhosePropertyHasChanged, const Identifier& property);
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded) {}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved) {}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved) {}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged) {}
		
	//==============================================================================

private:
	
	enum CompressionType
	{
		noCompression,
		simpleCompression,
		intCompression,
		intDerivativeCompression
	};
			
	ValueTree settingsTree;
	
	OwnedArray<Label> labels;
    ComboBox modeSelector;
    TextEditor socketNumber;
    TextEditor socketHost;
	StatusComponent status;
};

#endif  // __MAINVIEW_H_E96C7D0F__
