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
	filePlayer->getAudioTransportSource()->addChangeListener(this);
	filePlayer->addListener(this);
	
	currentBpm = String::empty;
	currentTime = timeToTimecodeStringLowRes(time);
	remainingTime = timeToTimecodeStringLowRes( fileLength - time );
	
	addAndMakeVisible(suggestButton = new TextButton("i"));
	suggestButton->addListener(this);
}

TrackInfo::~TrackInfo()
{
	filePlayer->getAudioTransportSource()->removeChangeListener(this);

	String message("TrackInfo ");
	message << deckNo << " deleted";
	DBG(message);
	
	deleteAllChildren();
}

//=================================================
void TrackInfo::paint(Graphics &g)
{
	const int h = getHeight();
	const int w = getWidth();
	const int m = 1;
	const int timeWidth = 72;
	const int bpmWidth = 60;
	const int fontSize = (h * 0.5) - (2*m);

	g.fillAll(Colours::black);
	g.setColour(Colours::white);
	
	g.setFont(fontSize);
	g.drawText(String(currentTime),
			   w-timeWidth, 0, timeWidth-(2*m), h*0.5,
			   Justification::centredRight, true);
	
	String remainString("-");
	remainString << String(remainingTime);
	g.drawText(remainString,
			   w-timeWidth, h*0.5, timeWidth-(2*m), h*0.5,
			   Justification::centredRight, true);

	g.setFont(fontSize);
	g.drawFittedText(String(deckNo+1),
					 m, m, fontSize+(2*m), (h * 0.5) - (2*m),
					 Justification::centred, 2, 1);	
	g.drawFittedText(trackName,
					 fontSize + (5*m), 0, w-timeWidth-fontSize-(7*m), h*0.5,
					 Justification::centredLeft, 1, 1);	
	g.drawFittedText(artistName,
					 fontSize + (5*m), h*0.5, w-timeWidth-bpmWidth-fontSize-(7*m), h*0.5,
					 Justification::centredLeft, 1, 1);	
	
	g.drawFittedText(currentBpm,
					 w-timeWidth-bpmWidth-4*m, h*0.5, bpmWidth, h*0.5,
					 Justification::centredRight, 1, 1);	
	
	g.setColour(DecksColours::getInstance()->getColour(DecksColours::panelLineColour));
	g.drawRect(0, 0, w, h, 1);
	g.drawHorizontalLine(fontSize+(2*m), 0, w);
	g.drawVerticalLine(fontSize+(2*m), 0, h);
	g.drawVerticalLine(w-timeWidth-bpmWidth, h*0.5, h);
	g.drawVerticalLine(w-timeWidth-(2*m), 0, h);
}

void TrackInfo::resized()
{
	const int h = getHeight();
//	const int w = getWidth();
	const int m = 1;

	suggestButton->setBounds(2*m, h - ((h * 0.5) - (2*m)), (h * 0.5) - (3*m), (h * 0.5) - (3*m));
}

void TrackInfo::buttonClicked (Button* button)
{
	if(button == suggestButton)
	{
		TrackSuggestions content(filePlayer->getLibraryEntry(), ITunesLibrary::getInstance()->getLibraryTree());
		content.setSize (400, 300);
		
		CallOutBox callOut (content, *suggestButton, getTopLevelComponent());
		callOut.runModalLoop();
	}
}

void TrackInfo::fileChanged (FilteringAudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		AudioFormatReaderSource* readerSource = filePlayer->getAudioFormatReaderSource();
		double sampleRate = 44100.0;
		if ( readerSource != 0)
			sampleRate = readerSource->getAudioFormatReader()->sampleRate;
		
		fileLength = filePlayer->getAudioTransportSource()->getTotalLength() / sampleRate;
		fileName = filePlayer->getFileName();
		
		trackName = filePlayer->getLibraryEntry().getProperty(Columns::columnNames[Columns::Song]);
		artistName = filePlayer->getLibraryEntry().getProperty(Columns::columnNames[Columns::Artist]);
		bpm = double(filePlayer->getLibraryEntry().getProperty(Columns::columnNames[Columns::BPM]));
		currentBpm = String(bpm * filePlayer->getResamplingRatio(), 2);
		
		// file was not loaded from the library
		if (trackName.isEmpty() && artistName.isEmpty())
		{
			trackName = fileName;
			currentBpm = String::empty;
		}
			
		repaint();
	}
}

void TrackInfo::resamplingRatioChanged(FilteringAudioFilePlayer *player)
{
	if (player == filePlayer)
	{
		if (filePlayer->getLibraryEntry().isValid())
			currentBpm = String(bpm * filePlayer->getResamplingRatio(), 2);
		else
			currentBpm = String::empty;
		
		repaint();
	}
}

void TrackInfo::changeListenerCallback(ChangeBroadcaster* changedObject)
{
	if (changedObject == static_cast<ChangeBroadcaster*>(filePlayer->getAudioTransportSource()))
	{		
		if (filePlayer->getAudioTransportSource()->isPlaying())
			startTimer(100);
		else {
			stopTimer();
			timerCallback();
		}
	}
}

void TrackInfo::timerCallback()
{
	time = filePlayer->getAudioTransportSource()->getCurrentPosition();
	
	currentTime = timeToTimecodeStringLowRes(time);
	remainingTime = timeToTimecodeStringLowRes( fileLength - time );
	
	repaint();
}