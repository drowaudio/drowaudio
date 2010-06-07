/*
 *  TrackInfo.cpp
 *  resampling_test
 *
 *  Created by David Rowland on 26/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "TrackInfo.h"

TrackInfo::TrackInfo(int deckNo_, FilteringAudioFilePlayer* filePlayer_)
	:	deckNo(deckNo_),
		filePlayer(filePlayer_),
		time(0.0),
		fileLength(0.0)
{
	filePlayer->addChangeListener(this);
	
	currentTime = timeToTimecodeStringLowRes(time);
	remainingTime = timeToTimecodeStringLowRes( fileLength - time );
}

TrackInfo::~TrackInfo()
{
}

//=================================================
void TrackInfo::paint(Graphics &g)
{
	const int timeWidth = 100;
	const int timeHeight = getHeight()/2;//28;

	g.fillAll(Colours::black);
	g.setColour(Colours::white);
	
	g.setFont(timeHeight);
	g.drawText(String(currentTime),
			   getWidth()-timeWidth, 0, timeWidth-4, timeHeight,
			   Justification::centredRight, true);
	
	String remainString("-");
	remainString << String(remainingTime);
	g.drawText(remainString,
			   getWidth()-timeWidth, timeHeight, timeWidth-4, timeHeight,
			   Justification::centredRight, true);

	g.setFont(22);
//	g.drawRect(0, 0, 20, timeHeight+1, 1);
	g.drawFittedText(String(deckNo),
					 0, 0, 20, timeHeight,
					 Justification::centred, 2, 1);	
	g.drawFittedText(fileName,
					 20, 0, getWidth()-timeWidth, getHeight(),
					 Justification::topLeft, 2, 1);	
	
	g.setColour(DecksLookAndFeel::getInstance()->getDecksColour(DecksLookAndFeel::panelLineColour));
	g.drawRect(0, 0, getWidth(), getHeight(), 1);
	g.drawHorizontalLine(timeHeight, 0, getWidth());
	g.drawVerticalLine(getWidth()-timeWidth, 0, getHeight());
	g.drawVerticalLine(20, 0, timeHeight);

}

void TrackInfo::resized()
{
	
}

void TrackInfo::changeListenerCallback(void* changedObject)
{
	if (changedObject == filePlayer)
	{
		AudioFormatReaderSource* readerSource = filePlayer->getAudioFormatReaderSource();
		double sampleRate = 44100.0;
		if ( readerSource != 0)
			sampleRate = readerSource->getAudioFormatReader()->sampleRate;
		
		fileLength = filePlayer->getTotalLength() / sampleRate;
		fileName = filePlayer->getFileName();
			

		if (filePlayer->isPlaying())
			startTimer(100);
		else {
			stopTimer();
			timerCallback();
		}
	}
}

void TrackInfo::timerCallback()
{
	time = filePlayer->getCurrentPosition();
	
	currentTime = timeToTimecodeStringLowRes(time);
	remainingTime = timeToTimecodeStringLowRes( fileLength - time );
	
	repaint();
}