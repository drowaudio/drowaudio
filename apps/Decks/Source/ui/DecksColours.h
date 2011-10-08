/*
  ==============================================================================

    DecksColours.h
    Created: 28 Dec 2010 8:08:49pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DECKSCOLOURS_H_93DD6ACC__
#define __DECKSCOLOURS_H_93DD6ACC__

#include "../JuceLibraryCode/JuceHeader.h"


class DecksColours	:	public DeletedAtShutdown
{
public:
	
	juce_DeclareSingleton(DecksColours, false);
	
	enum ColourId {
		backgroundColour,
		panelColour,
		panelLineColour,
		meterLineColour,
		buttonColour,
		rotarySliderColour,
		textColour,
		numColours
	};
	
	DecksColours();
	
	~DecksColours();
	
	void setColour(int colourID, const Colour &newColour);

	Colour getColour(int colourID);

private:
	
	Array<Colour> colours;
};



#endif  // __DECKSCOLOURS_H_93DD6ACC__
