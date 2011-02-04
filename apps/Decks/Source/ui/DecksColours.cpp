/*
  ==============================================================================

    DecksColours.cpp
    Created: 28 Dec 2010 8:08:49pm
    Author:  David Rowland

  ==============================================================================
*/

#include "DecksColours.h"

juce_ImplementSingleton(DecksColours);

DecksColours::DecksColours()
{
	for (int i = 0; i < numColours; i++) {
		colours.add(Colour());
	}
	
	colours.getReference(backgroundColour) = Colours::grey;
	colours.getReference(panelColour) = Colour::greyLevel(0.2);
	colours.getReference(panelLineColour) = Colour::greyLevel(0.6);
	colours.getReference(meterLineColour) = Colour::greyLevel(0.5);
	colours.getReference(buttonColour) = Colours::darkgrey;//Colour::greyLevel(0.5);
	colours.getReference(rotarySliderColour) = Colour::greyLevel(0.5);
	colours.getReference(textColour) = Colour::greyLevel(0.4);
	
}

DecksColours::~DecksColours()
{
	colours.clear();
}

void DecksColours::setColour(int colourID, const Colour &newColour)
{
	if (colourID < numColours)
		colours.getReference(colourID) = newColour;
}

Colour DecksColours::getColour(int colourID)
{
	if (colourID < numColours)
		return colours[colourID];
	
	return Colour();
}