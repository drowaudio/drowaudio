/*
  ==============================================================================

    QuitScreen.h
    Created: 26 Nov 2010 11:43:02pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __QUITSCREEN_H_4109308A__
#define __QUITSCREEN_H_4109308A__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "DecksLookAndFeel.h"

class QuitScreen  :	public Component,
					public ButtonListener
{
public:
	
	QuitScreen ();
	
	~QuitScreen ();
	
	//==============================================================================
	void resized ();
	
	void paint (Graphics& g);
	
	void visibilityChanged();
	
	bool keyPressed (const KeyPress& key);

	//==============================================================================
	void buttonClicked(Button *button);
	
	//==============================================================================
	
private:
	
	TextButton *yesButton, *noButton;
};

#endif  // __QUITSCREEN_H_4109308A__
