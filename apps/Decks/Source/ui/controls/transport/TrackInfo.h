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

#include <dRowAudio/dRowAudio.h>
#include "../../DecksColours.h"
#include "../../DecksLookAndFeel.h"
#include "TrackSuggestions.h"

class TrackInfo : public Component,
				  public Button::Listener,
				  public ChangeListener,
				  public Timer/*,
				  public Thread*/
{
public:
	
	TrackInfo(int deckNo_, FilteringAudioFilePlayer* filePlayer_);
	~TrackInfo();
	
	//=================================================
	void paint(Graphics &g);
	
	void resized();
	
	//=================================================
	void buttonClicked (Button* button);
	
	void changeListenerCallback(ChangeBroadcaster* changedObject);
	
	void timerCallback();

	//=================================================
	
	void setTime(double timeInSeconds);
	
private:
	
	const int deckNo;
	FilteringAudioFilePlayer *filePlayer;
	
	double time, fileLength;
	String currentTime, remainingTime, bpm, fileName, trackName, artistName;
	TextButton *suggestButton;
};

#endif //_TRACKINFO_H_