/*
  ==============================================================================

    Library.h
    Created: 29 Jan 2011 9:07:43pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __LIBRARY_H_84EB1D3D__
#define __LIBRARY_H_84EB1D3D__

#include "../JuceLibraryCode/JuceHeader.h"

#include "PrepareTable.h"

class Library : public Component,
				public ButtonListener,
				public TextEditor::Listener
{
public:
	Library();
	
	~Library();
	
	void resized();
	
	void paint(Graphics &g);

	//================================================================================
	void buttonClicked(Button* button);
	
	//================================================================================
	void textEditorTextChanged (TextEditor& editor);
	
	void textEditorReturnKeyPressed (TextEditor& editor) {}
	
	void textEditorEscapeKeyPressed (TextEditor& editor) {}
	
	void textEditorFocusLost (TextEditor& editor) {}
	
	//================================================================================
	
private:
	TextButton *prepareButton;
	TextEditor *filterEditor;
	
	StretchableLayoutManager layoutManager;
	StretchableLayoutResizerBar *resizeBar;
	
	PrepareTable *prepareTable;
	MusicLibraryTable *libraryTable;
};


#endif  // __LIBRARY_H_84EB1D3D__
