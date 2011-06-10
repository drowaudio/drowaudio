/*
  ==============================================================================

    MainComponent.h
    Created: 20 May 2011 5:08:02am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_CCF3F3A9__
#define __MAINCOMPONENT_H_CCF3F3A9__

#include "../JuceLibraryCode/JuceHeader.h"

/**	@todo	Add option to compress
	@todo	Improve UI
	@todo	Keep settings in a more convenient place
	@todo	Repitition of buffer in listener mode
	@todo	Sleep prevention when disconnected (possibly add and remove audio callback on connection)
	@todo	Set a flag to determine if stream is compressed
 */

class MainComponent :	public Component
{
public:
	MainComponent();
	
	~MainComponent();
	
	void resized();
	
private:
	
	TabbedComponent tabbedComponent;
};

#endif  // __MAINCOMPONENT_H_CCF3F3A9__
