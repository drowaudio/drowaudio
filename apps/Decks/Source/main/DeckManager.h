/*
 *  DeckManager.h
 *  Decks
 *
 *  Created by David Rowland on 06/03/2010.
 *  Copyright 2010 dRowAudio. All rights reserved.
 *
 */

#ifndef _DeckManager__H_
#define _DeckManager__H_

#include <dRowAudio/dRowAudio.h>
#include "Settings.h"
#include "DecksAudioFormatManager.h"

class Deck : public ValueTree::Listener
{
public:
	
	Deck(int deckNo_);
	
	~Deck();
	
	int getDeckNo()										{	return deckNo;				}
//	FilteringAudioFilePlayer* getFilePlayer()			{	return filePlayer;			}
	FilteringAudioFilePlayer* getMainFilePlayer()		{	return filePlayer;			}
	FilteringAudioFilePlayer* getMonitorFilePlayer()	{	return monitorFilePlayer;	}
		
	void startFromZero()
	{
		filePlayer->startFromZero();
		monitorFilePlayer->startFromZero();
	}
	
	void stop()
	{
		filePlayer->stop();
		monitorFilePlayer->stop();
	}
	
	void pause()
	{
		filePlayer->pause();
		monitorFilePlayer->pause();
	}
	
	void setNextReadPosition(int position)
	{
		filePlayer->setNextReadPosition(position);
		monitorFilePlayer->setNextReadPosition(position);
	}
	
	void setResamplingRatio(const double samplesInPerOutputSample)
	{
		filePlayer->setResamplingRatio(samplesInPerOutputSample);
		monitorFilePlayer->setResamplingRatio(samplesInPerOutputSample);
	}
		
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property);
	void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)	{}
	void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved) {}
	void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved) {}
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) {}
	
	void setSetting(const Identifier &setting, const var &newValue);
	const var getSetting(const Identifier &setting);
	
	//==============================================================================
    /** Receives callbacks when a Deck object changes.
	 @see Deck::addListener
	 */
//    class  Listener
//    {
//    public:
//        Listener()          {}
//        virtual ~Listener() {}
//		
//        /** Called when a Deck object is changed.
//		 */
//        virtual void deckChanged (Deck *deck) = 0;
//    };
//	
//    /** Adds a listener to receive callbacks when the deck changes.
//	 */
//    void addListener (Listener* const listener);
//	
//    /** Removes a listener that was previously added with addListener(). */
//    void removeListener (Listener* const listener);
	
	//==============================================================================

private:
	
	struct DeckSettings 
	{
		Atomic<int> number;
		Atomic<int> on;
		Atomic<int> bypass;
		Atomic<float> gain;
		Atomic<float> level;
		Atomic<float> highGain;
		Atomic<float> midGain;
		Atomic<float> lowGain;
		Atomic<int> highKill;
		Atomic<int> midKill;
		Atomic<int> lowKill;
		Atomic<int> cue;
		Atomic<float> fxASend;
		Atomic<float> fxBSend;
	} settings;
	
	CriticalSection lock;
	ValueTree deckTree;
	const int deckNo;
	ScopedPointer<FilteringAudioFilePlayer> filePlayer;
	ScopedPointer<FilteringAudioFilePlayer> monitorFilePlayer;
		
//	void callListeners();
//	SortedSet <Listener*> listeners;
};


//==============================================================================
//
//==============================================================================
class DeckManager	:	public ValueTree::Listener,
						public DeletedAtShutdown
{
public:
		
	DeckManager();
	
	~DeckManager();

	juce_DeclareSingleton (DeckManager, false)
	
	int getMaxNoDecks()				{	return decks.size();	}
	
	Deck* getDeck(int deckNo)		{	return decks[deckNo];	}
	
	/** Returns the number of currently enabled decks. */
	static const int getNoEnabledDecks()
	{
		const int noDecks = Settings::getInstance()->getPropertyOfChild("noChannels", "noChannels");
		
		int noEnabledDecks = 0;
		for (int i = 0; i < noDecks; i++)
			if (bool(Settings::getInstance()->getPropertyOfChannel(i, CHANNEL_SETTING(on))) == true)
				noEnabledDecks++;
		
		return noEnabledDecks;
	}
	
	const var getDeckSetting(const int deckNo, const Identifier &setting)
	{
		return decks[deckNo]->getSetting(setting);
	}
	
	void setMasterSetting(const Identifier &setting, const var &newValue);
	const var getMasterSetting(const Identifier &setting);

	void setXFaderSetting(const Identifier &setting, const var &newValue);
	const var getXFaderSetting(const Identifier &setting);

	//==============================================================================
	void valueTreePropertyChanged (ValueTree  &treeWhosePropertyHasChanged, const Identifier  &property);
	void valueTreeChildAdded (ValueTree &parentTree, ValueTree &childWhichHasBeenAdded)	{}
	void valueTreeChildRemoved (ValueTree &parentTree, ValueTree &childWhichHasBeenRemoved) {}
	void valueTreeChildOrderChanged (ValueTree &parentTreeWhoseChildrenHaveMoved) {}
	void valueTreeParentChanged (ValueTree &treeWhoseParentHasChanged) {}
	
	//==============================================================================
private:
	
	struct MasterSettings {
		Atomic<float> gain;
		Atomic<int> cue;
		Atomic<float> faderCurve;
		Atomic<float> xFaderCurve;
		Atomic<float> monitorGain;
	} masterSettings;

	struct XFaderSettings {
		Atomic<int> assignX;
		Atomic<int> assignY;
		Atomic<float> level;
	} xFaderSettings;
	
	CriticalSection lock;
	OwnedArray <Deck> decks;
};

#endif //_DeckManager__H_