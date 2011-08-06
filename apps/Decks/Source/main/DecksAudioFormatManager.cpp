/*
  ==============================================================================

    DecksAudioFormatManager.cpp
    Created: 22 Dec 2010 10:21:30pm
    Author:  David Rowland

  ==============================================================================
*/

#include "DecksAudioFormatManager.h"

juce_ImplementSingleton(DecksAudioFormatManager)

DecksAudioFormatManager::DecksAudioFormatManager()
:	AudioFormatManager()
{
	registerBasicFormats();
#ifdef JUCE_QUICKTIME
//	registerFormat(new QuickTimeAudioFormat(), false);
#endif
}

DecksAudioFormatManager::~DecksAudioFormatManager()
{
	
}
