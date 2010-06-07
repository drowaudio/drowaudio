/*
  ==============================================================================

    MixerSettings.h
    Created: 6 May 2010 3:52:24pm
    Author:  David Rowland

  ==============================================================================
*/

#ifndef __DECKS_MIXERSETTINGS_H_F14E7836__
#define __DECKS_MIXERSETTINGS_H_F14E7836__

#include <juce/juce.h>

namespace MixerSettings
{
	static const int noChannels = 4;
	
	namespace ChannelSettings {
		static const char *SectionName = "channel";

		enum ChannelSettings{
			on,
			bypass,
			gain,
			level,
			highGain,
			midGain,
			lowGain,
			highKill,
			midKill,
			lowKill,
			cue,
			fxASend,
			fxBSend,
			noChannelSettings
		};
		
		static const char UNUSED_NOWARN *Names[] = {
			"on",
			"bypass",
			"gain",
			"level",
			"highGain",
			"midGain",
			"lowGain",
			"highKill",
			"midKill",
			"lowKill",
			"cue",
			"fxASend",
			"fxBSend"			
		};
		
		static const var Values[] = 
		{
			true,		//on
			false,		//bypass
			1.0f,		//gain
			0.0,		//level
			1.0f,		//highGain
			1.0f,		//midGain
			1.0f,		//lowGain
			false,		//highKill
			false,		//midKill
			false,		//lowKill
			true,		//cue
			0.0f,		//fxASend
			0.0f,		//fxBSend	
		};
	}
		
	namespace xFaderSettings {
		static const char *SectionName = "xFader";

		enum xFaderSettings {
			assignX,
			assignY,
			level,
			noXFaderSettings
		};
		
		static const char UNUSED_NOWARN *Names[] = {
			"assignX",
			"assignY",
			"level"
		};
		
		static const var Values[] = 
		{
			0.0f,		//assignX
			0.0f,		//assignY
			0.5f		//level
		};
	}
		
	namespace MasterSettings {
		static const char *SectionName = "master";
		
		enum MasterSettings {
			gain,
			cue,
			faderCurve,
			xFaderCurve,
			noMasterSettings
		};
		
		static const char UNUSED_NOWARN *Names[] = {
			"gain",
			"cue",
			"faderCurve",
			"xFaderCurve"
		};
		
		static const var Values[] = 
		{
			1.0f,		//gain
			false,		//cue
			1.0f,		//faderCurve
			0.5f		//xFaderCurve
		};		
	}
};


#endif  // __DECKS_MIXERSETTINGS_H_F14E7836__
