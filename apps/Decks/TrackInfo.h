/*
 *  TrackInfo.h
 *  resampling_test
 *
 *  Created by David Rowland on 26/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#ifndef _TRACKINFO_H_
#define _TRACKINFO_H_

#include <juce/juce.h>
#include <dRowAudio/dRowAudio.h>

class TrackInfo : public Component,
				  public ChangeListener,
				  public Timer/*,
				  public Thread*/
{
public:
	
	TrackInfo(FilteringAudioFilePlayer* filePlayer_);
	~TrackInfo();
	
	//=================================================
	void paint(Graphics &g);
	
	void resized();
	
	void changeListenerCallback(void* changedObject);
	
	void timerCallback();
	//=================================================
	
	void setTime(double timeInSeconds);
	
private:
	
	FilteringAudioFilePlayer *filePlayer;
	
	double time, fileLength;
	String currentTime, remainingTime, bpm, fileName;// trackName, artistName;
};

#endif //_TRACKINFO_H_