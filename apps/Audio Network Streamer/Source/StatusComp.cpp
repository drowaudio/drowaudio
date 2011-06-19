/*
  ==============================================================================

    StatusComp.cpp
    Created: 11 Jun 2011 11:47:52am
    Author:  David Rowland

  ==============================================================================
*/

#include "StatusComp.h"

StatusComponent::StatusComponent()
:	currentStatus(waiting)
{
	addAndMakeVisible(&statusButton);
	addAndMakeVisible(&statusLabel);	

	setStatus(disconnected);	
}

StatusComponent::~StatusComponent()
{
}

void StatusComponent::resized()
{
	const int w = getWidth();
	const int h = getHeight();
	const int m = 5;
	
	statusButton.setBounds(2, 2, h-4, h-4);
	statusLabel.setBounds(statusButton.getRight()+m, 0, w - m - statusButton.getWidth(), h);
}

void StatusComponent::setStatus(Status newStatus)
{
	if (newStatus != currentStatus)
	{
		currentStatus = newStatus;
		Colour newColour;
		
		if (currentStatus == disconnected)
		{
			newColour = Colours::red;
			statusLabel.setText("Disconnected", false);
		}
		else if (currentStatus == waiting)
		{
			newColour = Colours::orange;
			statusLabel.setText("Waiting for connection", false);
		}
		else if (currentStatus == connectedSender)
		{
			newColour = Colours::green;
			statusLabel.setText("Sending Audio", false);
		}
		else if (currentStatus == connectedReciever)
		{
			newColour = Colours::green;
			statusLabel.setText("Recieving Audio", false);
		}

		statusButton.setColour(TextButton::buttonColourId, newColour);

		repaint();
	}
}
