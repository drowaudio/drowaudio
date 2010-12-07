/*
  ==============================================================================

    TitleBarComponent.cpp
    Created: 26 Nov 2010 10:44:21pm
    Author:  David Rowland

  ==============================================================================
*/

#include "TitleBarComponent.h"

TitleBarComponent::TitleBarComponent ()
:	showButtons(Desktop::getInstance().getKioskModeComponent() == getTopLevelComponent() ? true : false)
{
	addAndMakeVisible(buttons[closeButton] = getLookAndFeel().createDocumentWindowButton (DocumentWindow::closeButton));
	buttons[closeButton]->addButtonListener(this);
	addAndMakeVisible(buttons[maximiseButton] = getLookAndFeel().createDocumentWindowButton (DocumentWindow::maximiseButton));
	buttons[maximiseButton]->addButtonListener(this);
	
	addAndMakeVisible( clock = new Clock() );

	addAndMakeVisible(settingsButton = new TextButton("Settings"));
	settingsButton->setClickingTogglesState(true);
}

TitleBarComponent::~TitleBarComponent ()
{
	deleteAllChildren();
}

//==============================================================================
void TitleBarComponent::resized ()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	if (showButtons) {
		buttons[closeButton]->setVisible(true);
		buttons[closeButton]->setBounds(m, (h*0.5)-8, 16, 16);
		
		buttons[maximiseButton]->setVisible(true);
		buttons[maximiseButton]->setBounds(2*m + 16, (h*0.5)-8, 16, 16);
	}
	else
	{
		buttons[closeButton]->setVisible(false);
		buttons[maximiseButton]->setVisible(false);
	}

	settingsButton->changeWidthToFitText(20);
	clock->setBounds(w - clock->getRequiredWidth(), 0, clock->getRequiredWidth(), h);
	settingsButton->setBounds(clock->getX() - settingsButton->getWidth() - m, (h * 0.5) - (settingsButton->getHeight() * 0.5),
							  settingsButton->getWidth(), settingsButton->getHeight());
}

void TitleBarComponent::paint (Graphics& g)
{
	const int w = getWidth();
	const int h = getHeight();

	Font font (h * 0.65f, Font::bold);
    g.setFont (font);

	int imgSize;
	const void* iconPointer = (const void*)(BinaryData::getNamedResource("Decks_icon_png", imgSize));
	Image icon(ImageFileFormat::loadFrom(iconPointer, imgSize));
	
	int iconW = 0;
	if (icon.isValid())
		iconW = (icon.getWidth() / (float)icon.getHeight()) * h;		
	
	const int textW = font.getStringWidth (ProjectInfo::projectName) + iconW;

		g.setColour(Colours::darkgrey.contrasting(0.7));
		g.drawText (ProjectInfo::projectName, (w*0.5)-(0.5*textW), 0, textW, h, Justification::centredRight, true);
		g.drawImageWithin(icon, (w*0.5)-(0.5*textW), 0, h, h, RectanglePlacement(RectanglePlacement::fillDestination));
}

//==============================================================================
void TitleBarComponent::buttonClicked(Button *button)
{
	if (button == buttons[closeButton]) {
		JUCEApplication::getInstance()->systemRequestedQuit();
	}
	else if (button == buttons[maximiseButton]) {
		invokeDirectly(0x200f, true);
	}

}

void TitleBarComponent::showTiteBarButtons(bool shouldShowButtons)
{
	showButtons = shouldShowButtons;
}