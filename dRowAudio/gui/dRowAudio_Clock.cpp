/*
  ==============================================================================

    dRowAudio_Clock.cpp
    Created: 1 Jul 2010 3:41:54am
    Author:  David Rowland

  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

Clock::Clock()
{
	setTimeDisplayFormat (showTime + show24Hr);
}

Clock::~Clock()
{
	stopTimer();
}

int Clock::getRequiredWidth()
{
	return getFont().getStringWidth (timeAsString) + 10;
}

void Clock::setTimeDisplayFormat(const int newFormat)
{	
	displayFormat = newFormat;
	
	if ((displayFormat & showDayShort) && (displayFormat & showDayLong))
		displayFormat -= showDayShort;
	if ((displayFormat & showTime))
		startTimer (5900);
	if ((displayFormat & showSeconds))
		startTimer (950);
//	if ((displayFormat & showTenthSeconds))
//		startTimer(99);
	
	timerCallback();
}

void Clock::timerCallback()
{
	Time currentTime = Time::getCurrentTime();	
	timeAsString = String::empty;
	
	String formatString;
	formatString    << ((displayFormat & showDayShort)  ? "%a " : "")
                    << ((displayFormat & showDayLong)   ? "%A " : "")
                    << ((displayFormat & showDate)      ? "%x " : "")
                    << ((displayFormat & showTime)      ? ((displayFormat & show24Hr) ? "%H:%M" : "%I:%M") : "")
                    << ((displayFormat & showSeconds)   ? ":%S " : "");
	
	if (formatString != String::empty)
		timeAsString << currentTime.formatted (formatString);
	
	setText (timeAsString, false);
}

END_JUCE_NAMESPACE