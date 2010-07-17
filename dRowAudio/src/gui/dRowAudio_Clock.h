/*
  ==============================================================================

    dRowAudio_Clock.h
    Created: 1 Jul 2010 3:41:39am
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DROWAUDIO_CLOCK_H_80237209__
#define __DROWAUDIO_CLOCK_H_80237209__

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

/**	
	A handy digital graphical clock.
	
	Just add one of these to your component and it will display the time,
	continually updating itself. Set the look and feel of it as you would
	a normal label.
 */
class Clock : public Label,
			  public Timer
{
public:
	
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
	void setTimeDisplayFormat(const int newFormat);
	
	/**	Returns the width required to display all of the clock's information.
	 */
	int getRequiredWidth();
	
	/** @Internal.
	 */
	void timerCallback();
	
private:
	
	int displayFormat;
	String timeAsString;
};

#endif  // __DROWAUDIO_CLOCK_H_80237209__
