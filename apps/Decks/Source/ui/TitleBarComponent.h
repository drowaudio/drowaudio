/*
  ==============================================================================

    TitleBarComponent.h
    Created: 26 Nov 2010 10:44:21pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __TITLEBARCOMPONENT_H_63C7CAC7__
#define __TITLEBARCOMPONENT_H_63C7CAC7__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "DecksLookAndFeel.h"

class TitleBarComponent  :	public Component,
							public ButtonListener,
							public ApplicationCommandTarget
{
public:

	TitleBarComponent ();
	
	~TitleBarComponent ();
	
	//==============================================================================
	void resized ();
	
	void paint (Graphics& g);
	
	//==============================================================================
	void buttonClicked(Button *button);
		
	//==============================================================================
	TextButton* getSettingsButton()	{	return settingsButton;	}
	
	void showTiteBarButtons(bool shouldShowButtons);

	//==============================================================================
    ApplicationCommandTarget* getNextCommandTarget()
	{
		return findFirstTargetParentComponent();
	}
	
    void getAllCommands (Array <CommandID>& commands) {}
	
	void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result) {}
	
    bool perform (const InvocationInfo& info)
	{
		return false;
	}
	
	//==============================================================================

	enum Buttons {
		closeButton = 0,
		minimiseButton,
		maximiseButton,
		numButtons
	};
	
private:
	
	Button *buttons[numButtons];
	TextButton *settingsButton;
	Clock *clock;
	
	bool showButtons;
};

#endif  // __TITLEBARCOMPONENT_H_63C7CAC7__
