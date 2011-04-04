/*
  ==============================================================================

    MixTypeComponent.cpp
    Created: 15 Mar 2011 1:14:12pm
    Author:  David Rowland

  ==============================================================================
*/

#include "MixTypeComponent.h"

MixTypeComponent::MixTypeComponent()
{
	mixButtons.add(new TextButton("Same"));
	mixButtons[0]->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChildAsValue(UISettings::SectionName, MIXTYPE_SETTING(same)));

	mixButtons.add(new TextButton("Plus One"));
	mixButtons[1]->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChildAsValue(UISettings::SectionName, MIXTYPE_SETTING(plusOne)));

	mixButtons.add(new TextButton("Minus One"));
	mixButtons[2]->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChildAsValue(UISettings::SectionName, MIXTYPE_SETTING(minusOne)));
	
	mixButtons.add(new TextButton("Mood Change"));
	mixButtons[3]->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChildAsValue(UISettings::SectionName, MIXTYPE_SETTING(moodChange)));
	
	mixButtons.add(new TextButton("Plus Two Boost"));
	mixButtons[4]->getToggleStateValue().referTo(Settings::getInstance()->getPropertyOfChildAsValue(UISettings::SectionName, MIXTYPE_SETTING(plusTwoBoost)));

	//const int mixType = int(Settings::getInstance()->getPropertyOfChild(UISettings::MixTypeSettings::SectionName, MIXTYPE_SETTING(mixType)));
	
	for (int i = 0; i < mixButtons.size(); i++) {
		mixButtons.getUnchecked(i)->setClickingTogglesState(true);
//		mixButtons.getUnchecked(i)->addListener(this);
		addAndMakeVisible(mixButtons.getUnchecked(i));
	}
}

MixTypeComponent::~MixTypeComponent()
{
}

void MixTypeComponent::resized()
{
	const float buttonWidth = (getWidth() - (mixButtons.size() - 1) * 1) / (float)mixButtons.size();
	for (int i = 0; i < mixButtons.size(); i++) {
		mixButtons[i]->setBounds(RelativeRectangle(Rectangle<float>((i*(buttonWidth+1)), 0, buttonWidth, 20)));
	}
}

void MixTypeComponent::addButtonListener(Button::Listener *buttonListener)
{
	for (int i = 0; i < mixButtons.size(); i++)
		mixButtons.getUnchecked(i)->addListener(buttonListener);
}

int MixTypeComponent::getMixTypeMask()
{
	int mask = 0;
	for (int i = 0; i < mixButtons.size(); i++)
		if (mixButtons.getUnchecked(i)->getToggleState())
			mask += (int)pow(2, i);
	
	return mask;
}