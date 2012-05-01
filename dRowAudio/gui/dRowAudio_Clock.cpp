/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/



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

