/*
 *  TrackInfo.cpp
 *  resampling_test
 *
 *  Created by David Rowland on 26/03/2009.
 *  Copyright 2009 UWE. All rights reserved.
 *
 */

#include "TrackInfo.h"

TrackInfo::TrackInfo(FilteringAudioFilePlayer* filePlayer_)
	:	filePlayer(filePlayer_),
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
	const int timeWidth = 120;
	const int timeHeight = 28;

	g.fillAll(Colours::black);
	g.setColour(Colours::white);
	
	g.setFont(timeHeight);
	g.drawText(String(currentTime),
			   getWidth()-timeWidth, 0, timeWidth, timeHeight,
			   Justification::topRight, true);
	
	g.drawText(String(T("-"))<<remainingTime,
			   getWidth()-timeWidth, timeHeight, timeWidth, timeHeight,
			   Justification::bottomRight, true);

	g.setFont(22);
	g.drawFittedText(fileName,
					 0, 0, getWidth()-timeWidth, getHeight(),
					 Justification::topLeft, 2, 1);	
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
		else
			stopTimer();
		
		repaint();
	}
}

void TrackInfo::timerCallback()
{
	time = filePlayer->getCurrentPosition();
	
	currentTime = timeToTimecodeStringLowRes(time);
	remainingTime = timeToTimecodeStringLowRes( fileLength - time );
	
	repaint();
}