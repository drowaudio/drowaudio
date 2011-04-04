/*
  ==============================================================================

    TrackSuggestions.cpp
    Created: 6 Feb 2011 2:09:29pm
    Author:  David Rowland

  ==============================================================================
*/

#include "TrackSuggestions.h"
#include "TrackSuggestionTable.h"

TrackSuggestions::TrackSuggestions(ValueTree sourceToBaseSuggestionsOn, ValueTree libraryDatabase)
{
	addAndMakeVisible(mixTypeComponent = new MixTypeComponent());
	addAndMakeVisible(trackSuggestionTable = new TrackSuggestionTable(sourceToBaseSuggestionsOn, libraryDatabase));
	
	mixTypeComponent->addButtonListener(this);
	trackSuggestionTable->setMixTypeMask(mixTypeComponent->getMixTypeMask());
}

TrackSuggestions::~TrackSuggestions()
{
	deleteAllChildren();
}

void TrackSuggestions::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	
	mixTypeComponent->setBounds(0, 0, w, 20);
	trackSuggestionTable->setBounds(0, mixTypeComponent->getBottom()+2, w, h - mixTypeComponent->getBottom() + 2 - 4);
}

void TrackSuggestions::buttonClicked(Button *button)
{
	trackSuggestionTable->setMixTypeMask(mixTypeComponent->getMixTypeMask());
}

//==============================================================================
