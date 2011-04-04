/*
  ==============================================================================

    TrackSuggestions.h
    Created: 6 Feb 2011 2:09:29pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __TRACKSUGGESTIONS_H_89C95328__
#define __TRACKSUGGESTIONS_H_89C95328__

#include <dRowAudio/dRowAudio.h>
#include "MixTypeComponent.h"

class TrackSuggestionTable;

class TrackSuggestions : public Component,
						 public Button::Listener
{
public:
	
	TrackSuggestions(ValueTree sourceToBaseSuggestionsOn, ValueTree libraryDatabase);
	
	~TrackSuggestions();

	void resized();
	
	void buttonClicked(Button *button);
	
	enum MixType {
		same = 1,
		plusOne = 2,
		minusOne = 4,
		moodChange = 8,
		plusTwoBoost = 16,
		numMixTypes = 5
	};
	
private:

	MixTypeComponent *mixTypeComponent;
	TrackSuggestionTable *trackSuggestionTable;
	
};

#endif  // __TRACKSUGGESTIONS_H_89C95328__
