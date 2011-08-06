/*
  ==============================================================================

    LoopAndCuePoints.h
    Created: 28 Nov 2010 7:05:04am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __LOOPANDCUEPOINTS_H_1F853467__
#define __LOOPANDCUEPOINTS_H_1F853467__

#include <dRowAudio/dRowAudio.h>
#include "../../DecksLookAndFeel.h"
#include "CuePointList.h"

class LoopAndCuePoints : public Component
{
public:
    
	LoopAndCuePoints(FilteringAudioFilePlayer* filePlayer);
	
	~LoopAndCuePoints();
	
	void paint(Graphics &g);
	
	void resized();
	
	void setToggleValueToReferTo(Value &valueToReferTo);

private:
	
	DrawableButton *showHideButton;
	TabbedComponent *tabbedComponent;
};



#endif  // __LOOPANDCUEPOINTS_H_1F853467__
