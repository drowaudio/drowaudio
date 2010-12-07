/*
  ==============================================================================

    dRowAudio_Clock.cpp
    Created: 1 Jul 2010 3:41:54am
    Author:  David Rowland

  ==============================================================================
*/

#include "../core/dRowAudio_StandardHeader.h"

BEGIN_DROWAUDIO_NAMESPACE

#include "dRowAudio_Clock.h"

Clock::Clock()
{
	setTimeDisplayFormat(showTime + show24Hr);
	timerCallback();
}

Clock::~Clock()
{
	stopTimer();
}

int Clock::getRequiredWidth()
{
	return getFont().getStringWidth(timeAsString)+10;
}

void Clock::setTimeDisplayFormat(const int newFormat)
{	
	displayFormat = newFormat;
	
	if ((displayFormat & showDayShort) && (displayFormat & showDayLong))
		displayFormat -= showDayShort;
	if ((displayFormat & showTime))
		startTimer(5900);
	if ((displayFormat & showSeconds))
		startTimer(950);
//	if ((displayFormat & showTenthSeconds))
//		startTimer(99);
}

void Clock::timerCallback()
{
	Time currentTime = Time::getCurrentTime();	
	timeAsString = String::empty;
	
	String formatString;
	
	formatString << ((displayFormat & showDayShort) ? "%a " : "")
	<< ((displayFormat & showDayLong) ? "%A " : "");
	
	if (formatString != String::empty)
		timeAsString << currentTime.formatted(formatString);

	timeAsString << currentTime.toString(displayFormat & showDate,
										 displayFormat & showTime,
										 displayFormat & showSeconds,
										 displayFormat & show24Hr);

	setText(timeAsString, false);
}

END_DROWAUDIO_NAMESPACE