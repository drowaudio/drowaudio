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

#ifndef __DROWAUDIO_CLOCK_H__
#define __DROWAUDIO_CLOCK_H__

//==============================================================================
/**	A handy digital graphical clock.
	
	Just add one of these to your component and it will display the time,
	continually updating itself. Set the look and feel of it as you would
	a normal label.
 */
class Clock : public Label,
			  public Timer
{
public:
    //==============================================================================
	/**	A number of flags to set what sort of clock is displayed
	 */
	enum TimeDisplayFormat
	{
		showDate = 1,
		showTime = 2,
		showSeconds = 4,
		show24Hr = 8,
		showDayShort = 16,
		showDayLong = 32,
	};
	
    //==============================================================================
	/**	Constructor.
		Just add and make visible one of these to your component and it
		will display the current time and continually update itself.
	 */
	Clock();
	
	/**	Destructor.
	 */
	~Clock();

	/**	Sets the display format of the clock.
     
		To specify what sort of clock to display pass in a number of the
		TimeDisplayFormat flags. This is semi-inteligent so may choose to
		ignore certain flags such as the short day name if you have also
		specified the long day name.
	 */
	void setTimeDisplayFormat (const int newFormat);
	
	/**	Returns the width required to display all of the clock's information.
	 */
	int getRequiredWidth();
	
    //==============================================================================
	/** @internal */
	void timerCallback();
	
private:
    //==============================================================================
	int displayFormat;
	String timeAsString;
	
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Clock);
};

#endif  // __DROWAUDIO_CLOCK_H__