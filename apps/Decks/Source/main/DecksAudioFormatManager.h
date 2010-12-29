/*
  ==============================================================================

    DecksAudioFormatManager.h
    Created: 22 Dec 2010 10:21:30pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DECKSAUDIOFORMATMANAGER_H_70B8BF54__
#define __DECKSAUDIOFORMATMANAGER_H_70B8BF54__

#include <dRowAudio/dRowAudio.h>

class DecksAudioFormatManager : public AudioFormatManager
{
public:
		
	DecksAudioFormatManager();
	
	~DecksAudioFormatManager();

	juce_DeclareSingleton(DecksAudioFormatManager, false)

private:
};



#endif  // __DECKSAUDIOFORMATMANAGER_H_70B8BF54__
