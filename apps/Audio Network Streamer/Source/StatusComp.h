/*
  ==============================================================================

    StatusComp.h
    Created: 11 Jun 2011 11:47:52am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __STATUSCOMP_H_BD9DB0B2__
#define __STATUSCOMP_H_BD9DB0B2__

#include "../JuceLibraryCode/JuceHeader.h"

class StatusComponent : public Component
{
public:
	
	enum Status {
		disconnected = 0,
		waiting,
		connectedSender,
		connectedReciever,
	};
	
	StatusComponent();
	
	~StatusComponent();
	
	void resized();
	
	void setStatus(Status newStatus);
	
private:
	
	Status currentStatus;
	TextButton statusButton;
	Label statusLabel;
};


#endif  // __STATUSCOMP_H_BD9DB0B2__
