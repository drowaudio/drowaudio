/*
  ==============================================================================

    MixTypeComponent.h
    Created: 15 Mar 2011 1:14:12pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MIXTYPECOMPONENT_H_540FFBAB__
#define __MIXTYPECOMPONENT_H_540FFBAB__

#include <dRowAudio/dRowAudio.h>
#include "../../../main/Settings.h"

class MixTypeComponent : public Component
{
public:
	
	MixTypeComponent();
	
	~MixTypeComponent();

	void resized();
	
	void addButtonListener(Button::Listener *buttonListener);
	
	int getMixTypeMask();
		
private:
	
	OwnedArray <TextButton> mixButtons;
};

#endif  // __MIXTYPECOMPONENT_H_540FFBAB__
